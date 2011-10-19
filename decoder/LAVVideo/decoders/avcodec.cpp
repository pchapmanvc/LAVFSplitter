/*
 *      Copyright (C) 2011 Hendrik Leppkes
 *      http://www.1f0.de
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "stdafx.h"
#include "avcodec.h"

#include "moreuuids.h"
#include "parsers/MPEG2HeaderParser.h"

////////////////////////////////////////////////////////////////////////////////
// Constructor
////////////////////////////////////////////////////////////////////////////////

ILAVDecoder *CreateDecoderAVCodec() {
  return new CDecAvcodec();
}

////////////////////////////////////////////////////////////////////////////////
// Multi-threaded decoding configuration
////////////////////////////////////////////////////////////////////////////////
static struct {
  CodecID codecId;
  int     threadFlags;
} ff_thread_codecs[] = {
  { CODEC_ID_H264,       FF_THREAD_FRAME|FF_THREAD_SLICE },
  { CODEC_ID_MPEG2VIDEO,                 FF_THREAD_SLICE },
  { CODEC_ID_DVVIDEO,                    FF_THREAD_SLICE },
  { CODEC_ID_VP8,        FF_THREAD_FRAME                 },
  { CODEC_ID_VP3,        FF_THREAD_FRAME                 },
  { CODEC_ID_THEORA,     FF_THREAD_FRAME                 },
  { CODEC_ID_HUFFYUV,    FF_THREAD_FRAME                 },
  { CODEC_ID_MPEG4,      FF_THREAD_FRAME                 },
  { CODEC_ID_PRORES,                     FF_THREAD_SLICE },
};

static int getThreadFlags(CodecID codecId)
{
  for(int i = 0; i < countof(ff_thread_codecs); ++i) {
    if (ff_thread_codecs[i].codecId == codecId) {
      return ff_thread_codecs[i].threadFlags;
    }
  }
  return 0;
}

////////////////////////////////////////////////////////////////////////////////
// Create DXVA2 Extended Flags from a AVFrame and AVCodecContext
////////////////////////////////////////////////////////////////////////////////

static DXVA2_ExtendedFormat GetDXVA2ExtendedFlags(AVCodecContext *ctx, AVFrame *frame)
{
  DXVA2_ExtendedFormat fmt;
  ZeroMemory(&fmt, sizeof(fmt));

  // Sample format, progressive vs. interlaced
  if (frame->interlaced_frame) {
    fmt.SampleFormat = frame->top_field_first ? DXVA2_SampleFieldInterleavedEvenFirst : DXVA2_SampleFieldInterleavedOddFirst;
  } else
    fmt.SampleFormat = DXVA2_SampleProgressiveFrame;

  // Chroma location
  switch(ctx->chroma_sample_location) {
  case AVCHROMA_LOC_LEFT:
    fmt.VideoChromaSubsampling = DXVA2_VideoChromaSubsampling_MPEG2;
    break;
  case AVCHROMA_LOC_CENTER:
    fmt.VideoChromaSubsampling = DXVA2_VideoChromaSubsampling_MPEG1;
    break;
  case AVCHROMA_LOC_TOPLEFT:
    fmt.VideoChromaSubsampling = DXVA2_VideoChromaSubsampling_DV_PAL;
    break;
  }

  // Color Range, 0-255 or 16-235
  BOOL ffFullRange = (ctx->color_range == AVCOL_RANGE_JPEG) || ctx->pix_fmt == PIX_FMT_YUVJ420P || ctx->pix_fmt == PIX_FMT_YUVJ422P || ctx->pix_fmt == PIX_FMT_YUVJ444P || ctx->pix_fmt == PIX_FMT_YUVJ440P;
  fmt.NominalRange = ffFullRange ? DXVA2_NominalRange_0_255 : DXVA2_NominalRange_16_235;

  // Color Space / Transfer Matrix
  switch (ctx->colorspace) {
  case AVCOL_SPC_BT709:
    fmt.VideoTransferMatrix = DXVA2_VideoTransferMatrix_BT709;
    break;
  case AVCOL_SPC_BT470BG:
  case AVCOL_SPC_SMPTE170M:
    fmt.VideoTransferMatrix = DXVA2_VideoTransferMatrix_BT601;
    break;
  case AVCOL_SPC_SMPTE240M:
    fmt.VideoTransferMatrix = DXVA2_VideoTransferMatrix_SMPTE240M;
    break;
  case AVCOL_SPC_YCGCO:
    fmt.VideoTransferMatrix = (DXVA2_VideoTransferMatrix)7;
    break;
  }

  // Color Primaries
  switch(ctx->color_primaries) {
  case AVCOL_PRI_BT709:
    fmt.VideoPrimaries = DXVA2_VideoPrimaries_BT709;
    break;
  case AVCOL_PRI_BT470M:
    fmt.VideoPrimaries = DXVA2_VideoPrimaries_BT470_2_SysM;
    break;
  case AVCOL_PRI_BT470BG:
    fmt.VideoPrimaries = DXVA2_VideoPrimaries_BT470_2_SysBG;
    break;
  case AVCOL_PRI_SMPTE170M:
    fmt.VideoPrimaries = DXVA2_VideoPrimaries_SMPTE170M;
    break;
  case AVCOL_PRI_SMPTE240M:
    fmt.VideoPrimaries = DXVA2_VideoPrimaries_SMPTE240M;
    break;
  }

  // Color Transfer Function
  switch(ctx->color_trc) {
  case AVCOL_TRC_BT709:
    fmt.VideoTransferFunction = DXVA2_VideoTransFunc_709;
    break;
  case AVCOL_TRC_GAMMA22:
    fmt.VideoTransferFunction = DXVA2_VideoTransFunc_22;
    break;
  case AVCOL_TRC_GAMMA28:
    fmt.VideoTransferFunction = DXVA2_VideoTransFunc_28;
    break;
  case AVCOL_TRC_SMPTE240M:
    fmt.VideoTransferFunction = DXVA2_VideoTransFunc_240M;
    break;
  }

  return fmt;
}

////////////////////////////////////////////////////////////////////////////////
// avcodec -> LAV codec mappings
////////////////////////////////////////////////////////////////////////////////

// This mapping table should contain all pixel formats, except hardware formats (VDPAU, XVMC, DXVA, etc)
// A format that is not listed will be converted to YUV420
static struct PixelFormatMapping {
  PixelFormat    ffpixfmt;
  LAVPixelFormat lavpixfmt;
  BOOL           conversion;
  int            bpp;
} ff_pix_map[] = {
  { PIX_FMT_YUV420P,   LAVPixFmt_YUV420, FALSE },
  { PIX_FMT_YUYV422,   LAVPixFmt_YUY2,   FALSE },
  { PIX_FMT_RGB24,     LAVPixFmt_RGB32,  TRUE  },
  { PIX_FMT_BGR24,     LAVPixFmt_RGB24,  FALSE },
  { PIX_FMT_YUV422P,   LAVPixFmt_YUV422, FALSE },
  { PIX_FMT_YUV444P,   LAVPixFmt_YUV444, FALSE },
  { PIX_FMT_YUV410P,   LAVPixFmt_YUV420, TRUE  },
  { PIX_FMT_YUV411P,   LAVPixFmt_YUV422, TRUE  },
  { PIX_FMT_GRAY8,     LAVPixFmt_YUV420, TRUE  },
  { PIX_FMT_MONOWHITE, LAVPixFmt_YUV420, TRUE  },
  { PIX_FMT_MONOBLACK, LAVPixFmt_YUV420, TRUE  },
  { PIX_FMT_PAL8,      LAVPixFmt_RGB32,  TRUE  },
  { PIX_FMT_YUVJ420P,  LAVPixFmt_YUV420, FALSE },
  { PIX_FMT_YUVJ422P,  LAVPixFmt_YUV422, FALSE },
  { PIX_FMT_YUVJ444P,  LAVPixFmt_YUV444, FALSE },
  { PIX_FMT_UYVY422,   LAVPixFmt_YUV422, TRUE  },
  { PIX_FMT_UYYVYY411, LAVPixFmt_YUV422, TRUE  },
  { PIX_FMT_BGR8,      LAVPixFmt_RGB32,  TRUE  },
  { PIX_FMT_BGR4,      LAVPixFmt_RGB32,  TRUE  },
  { PIX_FMT_BGR4_BYTE, LAVPixFmt_RGB32,  TRUE  },
  { PIX_FMT_RGB8,      LAVPixFmt_RGB32,  TRUE  },
  { PIX_FMT_RGB4,      LAVPixFmt_RGB32,  TRUE  },
  { PIX_FMT_RGB4_BYTE, LAVPixFmt_RGB32,  TRUE  },
  { PIX_FMT_NV12,      LAVPixFmt_NV12,   FALSE },
  { PIX_FMT_NV21,      LAVPixFmt_NV12,   TRUE  },

  { PIX_FMT_ARGB,      LAVPixFmt_ARGB32, TRUE  },
  { PIX_FMT_RGBA,      LAVPixFmt_ARGB32, TRUE  },
  { PIX_FMT_ABGR,      LAVPixFmt_ARGB32, TRUE  },
  { PIX_FMT_BGRA,      LAVPixFmt_ARGB32, FALSE },

  { PIX_FMT_GRAY16BE,  LAVPixFmt_YUV420, TRUE  },
  { PIX_FMT_GRAY16LE,  LAVPixFmt_YUV420, TRUE  },
  { PIX_FMT_YUV440P,   LAVPixFmt_YUV444, TRUE  },
  { PIX_FMT_YUVJ440P,  LAVPixFmt_YUV444, TRUE  },
  { PIX_FMT_YUVA420P,  LAVPixFmt_YUV420, TRUE  },
  { PIX_FMT_RGB48BE,   LAVPixFmt_RGB32,  TRUE  },
  { PIX_FMT_RGB48LE,   LAVPixFmt_RGB32,  TRUE  },

  { PIX_FMT_RGB565BE,  LAVPixFmt_RGB32,  TRUE  },
  { PIX_FMT_RGB565LE,  LAVPixFmt_RGB32,  TRUE  },
  { PIX_FMT_RGB555BE,  LAVPixFmt_RGB32,  TRUE  },
  { PIX_FMT_RGB555LE,  LAVPixFmt_RGB32,  TRUE  },

  { PIX_FMT_BGR565BE,  LAVPixFmt_RGB32,  TRUE  },
  { PIX_FMT_BGR565LE,  LAVPixFmt_RGB32,  TRUE  },
  { PIX_FMT_BGR555BE,  LAVPixFmt_RGB32,  TRUE  },
  { PIX_FMT_BGR555LE,  LAVPixFmt_RGB32,  TRUE  },

  { PIX_FMT_YUV420P16LE, LAVPixFmt_YUV420bX, FALSE, 16 },
  { PIX_FMT_YUV420P16BE, LAVPixFmt_YUV420bX, TRUE,  16 },
  { PIX_FMT_YUV422P16LE, LAVPixFmt_YUV422bX, FALSE, 16 },
  { PIX_FMT_YUV422P16BE, LAVPixFmt_YUV422bX, TRUE,  16 },
  { PIX_FMT_YUV444P16LE, LAVPixFmt_YUV444bX, FALSE, 16 },
  { PIX_FMT_YUV444P16BE, LAVPixFmt_YUV444bX, TRUE,  16 },

  { PIX_FMT_RGB444LE,  LAVPixFmt_RGB32,  TRUE  },
  { PIX_FMT_RGB444BE,  LAVPixFmt_RGB32,  TRUE  },
  { PIX_FMT_BGR444LE,  LAVPixFmt_RGB32,  TRUE  },
  { PIX_FMT_BGR444BE,  LAVPixFmt_RGB32,  TRUE  },
  { PIX_FMT_GRAY8A,    LAVPixFmt_YUV420, TRUE  },
  { PIX_FMT_BGR48BE,   LAVPixFmt_RGB32,  TRUE  },
  { PIX_FMT_BGR48LE,   LAVPixFmt_RGB32,  TRUE  },

  { PIX_FMT_YUV420P9BE,  LAVPixFmt_YUV420bX, TRUE,  9 },
  { PIX_FMT_YUV420P9LE,  LAVPixFmt_YUV420bX, FALSE, 9 },
  { PIX_FMT_YUV420P10BE, LAVPixFmt_YUV420bX, TRUE,  10 },
  { PIX_FMT_YUV420P10LE, LAVPixFmt_YUV420bX, FALSE, 10 },

  { PIX_FMT_YUV422P10BE, LAVPixFmt_YUV422bX, TRUE,  10 },
  { PIX_FMT_YUV422P10LE, LAVPixFmt_YUV422bX, FALSE, 10 },

  { PIX_FMT_YUV444P9BE,  LAVPixFmt_YUV444bX, TRUE,  9 },
  { PIX_FMT_YUV444P9LE,  LAVPixFmt_YUV444bX, FALSE, 9 },
  { PIX_FMT_YUV444P10BE, LAVPixFmt_YUV444bX, TRUE,  10 },
  { PIX_FMT_YUV444P10LE, LAVPixFmt_YUV444bX, FALSE, 10 },
};

static struct PixelFormatMapping getPixFmtMapping(PixelFormat pixfmt) {
  const PixelFormatMapping def = { pixfmt, LAVPixFmt_YUV420, TRUE, 8 };
  PixelFormatMapping result = def;
  for (int i = 0; i < countof(ff_pix_map); i++) {
    if (ff_pix_map[i].ffpixfmt == pixfmt) {
      result = ff_pix_map[i];
    }
  }
  if (result.lavpixfmt != LAVPixFmt_YUV420bX && result.lavpixfmt != LAVPixFmt_YUV422bX && result.lavpixfmt != LAVPixFmt_YUV444bX)
    result.bpp = 8;

  return result;
}

////////////////////////////////////////////////////////////////////////////////
// AVCodec decoder implementation
////////////////////////////////////////////////////////////////////////////////

CDecAvcodec::CDecAvcodec(void)
  : CDecBase()
  , m_pAVCodec(NULL)
  , m_pAVCtx(NULL)
  , m_pParser(NULL)
  , m_pFrame(NULL)
  , m_pFFBuffer(NULL), m_nFFBufferSize(0)
  , m_pSwsContext(NULL)
  , m_nCodecId(CODEC_ID_NONE)
  , m_rtStartCache(AV_NOPTS_VALUE)
  , m_bWaitingForKeyFrame(FALSE)
  , m_bBFrameDelay(TRUE)
  , m_nBFramePos(0)
{
}

CDecAvcodec::~CDecAvcodec(void)
{
  DestroyDecoder();
}

// ILAVDecoder
STDMETHODIMP CDecAvcodec::Init()
{
  avcodec_register_all();
  return S_OK;
}

STDMETHODIMP CDecAvcodec::InitDecoder(CodecID codec, const CMediaType *pmt)
{
  DestroyDecoder();
  DbgLog((LOG_TRACE, 10, L"Initializing ffmpeg for codec %d", codec));

  BITMAPINFOHEADER *pBMI = NULL;
  videoFormatTypeHandler((const BYTE *)pmt->Format(), pmt->FormatType(), &pBMI);

  m_pAVCodec = avcodec_find_decoder(codec);
  CheckPointer(m_pAVCodec, VFW_E_UNSUPPORTED_VIDEO);

  m_pAVCtx = avcodec_alloc_context3(m_pAVCodec);
  CheckPointer(m_pAVCtx, E_POINTER);

  if(codec == CODEC_ID_MPEG1VIDEO || codec == CODEC_ID_MPEG2VIDEO || pmt->subtype == FOURCCMap(MKTAG('H','2','6','4')) || pmt->subtype == FOURCCMap(MKTAG('h','2','6','4'))) {
    m_pParser = av_parser_init(codec);
  }

  LONG biRealWidth = pBMI->biWidth, biRealHeight = pBMI->biHeight;
  if (pmt->formattype == FORMAT_VideoInfo || pmt->formattype == FORMAT_MPEGVideo) {
    VIDEOINFOHEADER *vih = (VIDEOINFOHEADER *)pmt->Format();
    if (vih->rcTarget.right != 0 && vih->rcTarget.bottom != 0) {
      biRealWidth  = vih->rcTarget.right;
      biRealHeight = vih->rcTarget.bottom;
    }
  } else if (pmt->formattype == FORMAT_VideoInfo2 || pmt->formattype == FORMAT_MPEG2Video) {
    VIDEOINFOHEADER2 *vih2 = (VIDEOINFOHEADER2 *)pmt->Format();
    if (vih2->rcTarget.right != 0 && vih2->rcTarget.bottom != 0) {
      biRealWidth  = vih2->rcTarget.right;
      biRealHeight = vih2->rcTarget.bottom;
    }
  }

  m_pAVCtx->codec_id              = codec;
  m_pAVCtx->codec_tag             = pBMI->biCompression;
  m_pAVCtx->coded_width           = pBMI->biWidth;
  m_pAVCtx->coded_height          = abs(pBMI->biHeight);
  m_pAVCtx->bits_per_coded_sample = pBMI->biBitCount;
  m_pAVCtx->error_concealment     = FF_EC_GUESS_MVS | FF_EC_DEBLOCK;
  m_pAVCtx->error_recognition     = FF_ER_CAREFUL;
  m_pAVCtx->workaround_bugs       = FF_BUG_AUTODETECT;

  if (codec == CODEC_ID_H264)
    m_pAVCtx->flags2             |= CODEC_FLAG2_SHOW_ALL;

  // Setup threading
  int thread_type = getThreadFlags(codec);
  if (thread_type) {
    // Thread Count. 0 = auto detect
    int thread_count = m_pSettings->GetNumThreads();
    if (thread_count == 0) {
      SYSTEM_INFO systemInfo;
      GetSystemInfo(&systemInfo);
      thread_count = systemInfo.dwNumberOfProcessors * 3 / 2;
    }

    m_pAVCtx->thread_count = max(1, min(thread_count, AVCODEC_MAX_THREADS));
    m_pAVCtx->thread_type = thread_type;
  } else {
    m_pAVCtx->thread_count = 1;
  }

  m_pFrame = avcodec_alloc_frame();
  CheckPointer(m_pFrame, E_POINTER);

  m_h264RandomAccess.SetAVCNALSize(0);
  m_h264RandomAccess.flush(m_pAVCtx->thread_count);

  // Process Extradata
  BYTE *extra = NULL;
  unsigned int extralen = 0;
  getExtraData((const BYTE *)pmt->Format(), pmt->FormatType(), pmt->FormatLength(), NULL, &extralen);

  if (extralen > 0) {
    DbgLog((LOG_TRACE, 10, L"-> Processing extradata of %d bytes", extralen));
    // Reconstruct AVC1 extradata format
    if (pmt->formattype == FORMAT_MPEG2Video && (m_pAVCtx->codec_tag == MAKEFOURCC('a','v','c','1') || m_pAVCtx->codec_tag == MAKEFOURCC('A','V','C','1') || m_pAVCtx->codec_tag == MAKEFOURCC('C','C','V','1'))) {
      MPEG2VIDEOINFO *mp2vi = (MPEG2VIDEOINFO *)pmt->Format();
      extralen += 7;
      extra = (uint8_t *)av_mallocz(extralen + FF_INPUT_BUFFER_PADDING_SIZE);
      extra[0] = 1;
      extra[1] = (BYTE)mp2vi->dwProfile;
      extra[2] = 0;
      extra[3] = (BYTE)mp2vi->dwLevel;
      extra[4] = (BYTE)(mp2vi->dwFlags ? mp2vi->dwFlags : 2) - 1;

      // Actually copy the metadata into our new buffer
      unsigned int actual_len;
      getExtraData((const BYTE *)pmt->Format(), pmt->FormatType(), pmt->FormatLength(), extra+6, &actual_len);

      // Count the number of SPS/PPS in them and set the length
      // We'll put them all into one block and add a second block with 0 elements afterwards
      // The parsing logic does not care what type they are, it just expects 2 blocks.
      BYTE *p = extra+6, *end = extra+6+actual_len;
      int count = 0;
      while (p+1 < end) {
        unsigned len = (((unsigned)p[0] << 8) | p[1]) + 2;
        if (p + len > end) {
          break;
        }
        count++;
        p += len;
      }
      extra[5] = count;
      extra[extralen-1] = 0;

      m_h264RandomAccess.SetAVCNALSize(mp2vi->dwFlags);
    } else {
      // Just copy extradata for other formats
      extra = (uint8_t *)av_mallocz(extralen + FF_INPUT_BUFFER_PADDING_SIZE);
      getExtraData((const BYTE *)pmt->Format(), pmt->FormatType(), pmt->FormatLength(), extra, NULL);
    }
    m_pAVCtx->extradata = extra;
    m_pAVCtx->extradata_size = extralen;
  } else {
    if (codec == CODEC_ID_VP6 || codec == CODEC_ID_VP6A || codec == CODEC_ID_VP6F) {
      int cropH = pBMI->biWidth - biRealWidth;
      int cropV = pBMI->biHeight - biRealHeight;
      if (cropH > 0 && cropH < 0x0f && cropV > 0 && cropV < 0x0f) {
        m_pAVCtx->extradata = (uint8_t *)av_mallocz(1 + FF_INPUT_BUFFER_PADDING_SIZE);
        m_pAVCtx->extradata_size = 1;
        m_pAVCtx->extradata[0] = (cropH << 4) | cropV;
      }
    }
  }

  // Setup codec-specific timing logic
  LPWSTR pszExtension = m_pCallback->GetFileExtension();
  DbgLog((LOG_TRACE, 10, L"-> File extension: %s", pszExtension));

  BOOL bVC1IsPTS = (codec == CODEC_ID_VC1 && !m_pCallback->VC1IsDTS());

  // Use ffmpegs logic to reorder timestamps
  // This is required for H264 content (except AVI), and generally all codecs that use frame threading
  // VC-1 is also a special case. Its required for splitters that deliver PTS timestamps (see bVC1IsPTS above)
  m_bFFReordering        = ((codec == CODEC_ID_H264 && (_wcsicmp(pszExtension, L".avi") != 0)) || codec == CODEC_ID_VP8 || codec == CODEC_ID_VP3 || codec == CODEC_ID_THEORA || codec == CODEC_ID_HUFFYUV || codec == CODEC_ID_MPEG2VIDEO || codec == CODEC_ID_MPEG1VIDEO || bVC1IsPTS);

  // Stop time is unreliable, drop it and calculate it
  m_bCalculateStopTime   = (codec == CODEC_ID_H264 || bVC1IsPTS);

  // Real Video content has some odd timestamps
  // LAV Splitter does them allright with RV30/RV40, everything else screws them up
  m_bRVDropBFrameTimings = (codec == CODEC_ID_RV10 || codec == CODEC_ID_RV20 || (_wcsicmp(pszExtension, L".mkv") == 0) || ((codec == CODEC_ID_RV30 || codec == CODEC_ID_RV40) && !(FilterInGraph(CLSID_LAVSplitter) || FilterInGraph(CLSID_LAVSplitterSource))));

  // Enable B-Frame delay handling
  m_bBFrameDelay = !m_bFFReordering;

  SAFE_CO_FREE(pszExtension);

  // Open the decoder
  int ret = avcodec_open2(m_pAVCtx, m_pAVCodec, NULL);
  if (ret >= 0) {
    DbgLog((LOG_TRACE, 10, L"-> ffmpeg codec opened successfully (ret: %d)", ret));
    m_nCodecId = codec;
  } else {
    DbgLog((LOG_TRACE, 10, L"-> ffmpeg codec failed to open (ret: %d)", ret));
    DestroyDecoder();
    return VFW_E_UNSUPPORTED_VIDEO;
  }

  // Detect MPEG-2 chroma format
  if (codec == CODEC_ID_MPEG2VIDEO && m_pAVCtx->extradata && m_pAVCtx->extradata_size) {
    CMPEG2HeaderParser mpeg2Parser(extra, extralen);
    if (mpeg2Parser.hdr.chroma < 2) {
      m_pAVCtx->pix_fmt = PIX_FMT_YUV420P;
    } else if (mpeg2Parser.hdr.chroma == 2) {
      m_pAVCtx->pix_fmt = PIX_FMT_YUV422P;
    } else {
      m_pAVCtx->pix_fmt = PIX_FMT_YUV444P;
    }
  }

  return S_OK;
}

STDMETHODIMP CDecAvcodec::DestroyDecoder()
{
  DbgLog((LOG_TRACE, 10, L"Shutting down ffmpeg..."));
  m_pAVCodec	= NULL;

  if (m_pParser) {
    av_parser_close(m_pParser);
    m_pParser = NULL;
  }

  if (m_pAVCtx) {
    avcodec_close(m_pAVCtx);
    av_freep(&m_pAVCtx->extradata);
    av_freep(&m_pAVCtx);
  }
  av_freep(&m_pFrame);

  av_freep(&m_pFFBuffer);
  m_nFFBufferSize = 0;

  if (m_pSwsContext) {
    sws_freeContext(m_pSwsContext);
    m_pSwsContext = NULL;
  }

  m_nCodecId = CODEC_ID_NONE;

  return S_OK;
}

STDMETHODIMP CDecAvcodec::Decode(const BYTE *buffer, int buflen, REFERENCE_TIME rtStartIn, REFERENCE_TIME rtStopIn, BOOL bSyncPoint, BOOL bDiscontinuity)
{
  HRESULT hr = S_OK;
  int     got_picture = 0;
  int     used_bytes  = 0;
  BOOL    bParserFrame = FALSE;
  BOOL    bFlush = (buffer == NULL);

  AVPacket avpkt;
  av_init_packet(&avpkt);

  if (m_pAVCtx->active_thread_type & FF_THREAD_FRAME) {
    if (!m_bFFReordering) {
      m_tcThreadBuffer[m_CurrentThread].rtStart = rtStartIn;
      m_tcThreadBuffer[m_CurrentThread].rtStop  = rtStopIn;
    }

    m_CurrentThread = (m_CurrentThread + 1) % m_pAVCtx->thread_count;
  } else if (m_bBFrameDelay) {
    m_tcBFrameDelay[m_nBFramePos].rtStart = rtStartIn;
    m_tcBFrameDelay[m_nBFramePos].rtStop = rtStopIn;
    m_nBFramePos = !m_nBFramePos;
  }

  if (m_nCodecId == CODEC_ID_H264 && !bFlush) {
    BOOL bRecovered = m_h264RandomAccess.searchRecoveryPoint(buffer, buflen);
    if (!bRecovered) {
      return S_OK;
    }
  }

  while (buflen > 0 || bFlush) {
    REFERENCE_TIME rtStart = rtStartIn, rtStop = rtStopIn;

    // Copy bitstream into temporary buffer to ensure overread protection
    if (!bFlush) {
      // Verify buffer size
      if (buflen > m_nFFBufferSize) {
        m_nFFBufferSize	= buflen;
        m_pFFBuffer = (BYTE *)av_realloc_f(m_pFFBuffer, m_nFFBufferSize + FF_INPUT_BUFFER_PADDING_SIZE, 1);
        if (!m_pFFBuffer) {
          m_nFFBufferSize = 0;
          return E_FAIL;
        }
      }

      memcpy(m_pFFBuffer, buffer, buflen);
      memset(m_pFFBuffer+buflen, 0, FF_INPUT_BUFFER_PADDING_SIZE);

      avpkt.data = m_pFFBuffer;
      avpkt.size = buflen;
      avpkt.pts = rtStartIn;
      avpkt.dts = rtStopIn;
      avpkt.flags = AV_PKT_FLAG_KEY;
    } else {
      avpkt.data = NULL;
      avpkt.size = 0;
    }

    // Parse the data if a parser is present
    // This is mandatory for MPEG-1/2
    if (m_pParser) {
      BYTE *pOut = NULL;
      int pOut_size = 0;

      used_bytes = av_parser_parse2(m_pParser, m_pAVCtx, &pOut, &pOut_size, avpkt.data, avpkt.size, AV_NOPTS_VALUE, AV_NOPTS_VALUE, 0);

      if (used_bytes == 0 && pOut_size == 0 && !bFlush) {
        DbgLog((LOG_TRACE, 50, L"::Decode() - could not process buffer, starving?"));
        break;
      }

      // Update start time cache
      // If more data was read then output, update the cache (incomplete frame)
      // If output is bigger, a frame was completed, update the actual rtStart with the cached value, and then overwrite the cache
      if (used_bytes > pOut_size) {
        if (rtStartIn != AV_NOPTS_VALUE)
          m_rtStartCache = rtStartIn;
      } else if (pOut_size >= used_bytes) {
        rtStart = m_rtStartCache;
        m_rtStartCache = rtStartIn;
        // The value was used once, don't use it for multiple frames, that ends up in weird timings
        rtStartIn = AV_NOPTS_VALUE;
      }

       bParserFrame = (pOut_size > 0);

      if (pOut_size > 0 || bFlush) {

        if (pOut_size > 0) {
          // Copy output data into the work buffer
          if (pOut_size > m_nFFBufferSize) {
            m_nFFBufferSize	= pOut_size;
            BYTE *tmp = (BYTE *)av_realloc(m_pFFBuffer, m_nFFBufferSize + FF_INPUT_BUFFER_PADDING_SIZE);
            if (!tmp) {
              av_freep(&m_pFFBuffer);
              m_nFFBufferSize = 0;
              return E_FAIL;
            }
            m_pFFBuffer = tmp;
          }

          memcpy(m_pFFBuffer, pOut, pOut_size);
          memset(m_pFFBuffer+pOut_size, 0, FF_INPUT_BUFFER_PADDING_SIZE);

          avpkt.data = m_pFFBuffer;
          avpkt.size = pOut_size;
          avpkt.pts = rtStart;
          avpkt.dts = AV_NOPTS_VALUE;
        }

        int ret2 = avcodec_decode_video2 (m_pAVCtx, m_pFrame, &got_picture, &avpkt);
        if (ret2 < 0) {
          DbgLog((LOG_TRACE, 50, L"::Decode() - decoding failed despite successfull parsing"));
          continue;
        }
      } else {
        got_picture = 0;
      }
    } else {
      used_bytes = avcodec_decode_video2 (m_pAVCtx, m_pFrame, &got_picture, &avpkt);
    }

    // Decoding of this frame failed ... oh well!
    if (used_bytes < 0) {
      return S_OK;
    }

    // When Frame Threading, we won't know how much data has been consumed, so it by default eats everything.
    // In addition, if no data got consumed, and no picture was extracted, the frame probably isn't all that useufl.
    // The MJPEB decoder is somewhat buggy and doesn't let us know how much data was consumed really...
    if ((!m_pParser && (m_pAVCtx->active_thread_type & FF_THREAD_FRAME || (!got_picture && used_bytes == 0))) || m_nCodecId == CODEC_ID_MJPEGB || bFlush) {
      buflen = 0;
    } else {
      buflen -= used_bytes;
      buffer += used_bytes;
    }

    // Judge frame usability
    // This determines if a frame is artifact free and can be delivered
    // For H264 this does some wicked magic hidden away in the H264RandomAccess class
    // MPEG-2 and VC-1 just wait for a keyframe..
    if (m_nCodecId == CODEC_ID_H264 && (bParserFrame || !m_pParser || got_picture)) {
      m_h264RandomAccess.judgeFrameUsability(m_pFrame, &got_picture);
    } else if (m_nCodecId == CODEC_ID_MPEG2VIDEO || m_nCodecId == CODEC_ID_VC1) {
      if (m_bWaitingForKeyFrame && got_picture) {
        if (m_pFrame->key_frame) {
          m_bWaitingForKeyFrame = FALSE;
        } else {
          got_picture = 0;
        }
      }
    }

    // Handle B-frame delay for frame threading codecs
    if ((m_pAVCtx->active_thread_type & FF_THREAD_FRAME) && m_bBFrameDelay) {
      m_tcBFrameDelay[m_nBFramePos] = m_tcThreadBuffer[m_CurrentThread];
      m_nBFramePos = !m_nBFramePos;
    }

    if (!got_picture || !m_pFrame->data[0]) {
      bFlush = FALSE; // End flushing, no more frames
      continue;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Determine the proper timestamps for the frame, based on different possible flags.
    ///////////////////////////////////////////////////////////////////////////////////////////////
    if (m_bFFReordering) {
      rtStart = m_pFrame->pkt_pts;
      rtStop = m_pFrame->pkt_dts;
    } else if (m_nCodecId == CODEC_ID_RV10 || m_nCodecId == CODEC_ID_RV20 || m_nCodecId == CODEC_ID_RV30 || m_nCodecId == CODEC_ID_RV40) {
      if (m_bRVDropBFrameTimings && m_pFrame->pict_type == AV_PICTURE_TYPE_B)
        rtStart = AV_NOPTS_VALUE;
    } else if (m_bBFrameDelay && m_pAVCtx->has_b_frames) {
      rtStart = m_tcBFrameDelay[m_nBFramePos].rtStart;
      rtStop  = m_tcBFrameDelay[m_nBFramePos].rtStop;
    } else if (m_pAVCtx->active_thread_type & FF_THREAD_FRAME) {
      unsigned index = m_CurrentThread;
      rtStart = m_tcThreadBuffer[index].rtStart;
      rtStop  = m_tcThreadBuffer[index].rtStop;
    }

    if (m_bCalculateStopTime)
      rtStop = AV_NOPTS_VALUE;

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // All required values collected, deliver the frame
    ///////////////////////////////////////////////////////////////////////////////////////////////
    LAVFrame *pOutFrame = NULL;
    AllocateFrame(&pOutFrame);

    AVRational display_aspect_ratio;
    int64_t num = (int64_t)m_pAVCtx->sample_aspect_ratio.num * m_pAVCtx->width;
    int64_t den = (int64_t)m_pAVCtx->sample_aspect_ratio.den * m_pAVCtx->height;
    av_reduce(&display_aspect_ratio.num, &display_aspect_ratio.den, num, den, 255);

    pOutFrame->width        = m_pAVCtx->width;
    pOutFrame->height       = m_pAVCtx->height;
    pOutFrame->aspect_ratio = display_aspect_ratio;
    pOutFrame->repeat       = m_pFrame->repeat_pict;
    pOutFrame->key_frame    = m_pFrame->key_frame;
    pOutFrame->ext_format   = GetDXVA2ExtendedFlags(m_pAVCtx, m_pFrame);

    pOutFrame->rtStart      = rtStart;
    pOutFrame->rtStop       = rtStop;

    PixelFormatMapping map  = getPixFmtMapping(m_pAVCtx->pix_fmt);
    pOutFrame->format       = map.lavpixfmt;
    pOutFrame->bpp          = map.bpp;

    if (m_nCodecId == CODEC_ID_MPEG2VIDEO)
      pOutFrame->avgFrameDuration = GetFrameDuration();

    if (map.conversion) {
      ConvertPixFmt(m_pFrame, pOutFrame);
    } else {
      for (int i = 0; i < 4; i++) {
        pOutFrame->data[i]   = m_pFrame->data[i];
        pOutFrame->stride[i] = m_pFrame->linesize[i];
      }
    }

    Deliver(pOutFrame);

    if (bFlush) {
      m_CurrentThread = (m_CurrentThread + 1) % m_pAVCtx->thread_count;
    }
  }

  return S_OK;
}

STDMETHODIMP CDecAvcodec::Flush()
{
  if (m_pAVCtx) {
    avcodec_flush_buffers (m_pAVCtx);
    m_h264RandomAccess.flush(m_pAVCtx->thread_count);
  }

  m_CurrentThread = 0;
  m_rtStartCache = AV_NOPTS_VALUE;
  m_bWaitingForKeyFrame = TRUE;

  m_nBFramePos = 0;
  m_tcBFrameDelay[0].rtStart = m_tcBFrameDelay[0].rtStop = AV_NOPTS_VALUE;
  m_tcBFrameDelay[1].rtStart = m_tcBFrameDelay[1].rtStop = AV_NOPTS_VALUE;


  return S_OK;
}

STDMETHODIMP CDecAvcodec::EndOfStream()
{
  Decode(NULL, 0, AV_NOPTS_VALUE, AV_NOPTS_VALUE, FALSE, FALSE);
  return S_OK;
}

STDMETHODIMP CDecAvcodec::GetPixelFormat(LAVPixelFormat *pPix, int *pBpp)
{
  PixelFormat pixfmt = m_pAVCtx ? m_pAVCtx->pix_fmt : PIX_FMT_NONE;
  PixelFormatMapping mapping = getPixFmtMapping(pixfmt);
  if (pPix)
    *pPix = mapping.lavpixfmt;
  if (pBpp)
    *pBpp = mapping.bpp;
  return S_OK;
}

STDMETHODIMP CDecAvcodec::ConvertPixFmt(AVFrame *pFrame, LAVFrame *pOutFrame)
{
  // Allocate the buffers to write into
  AllocLAVFrameBuffers(pOutFrame);

  // Map to swscale compatible format
  PixelFormat dstFormat = getFFPixelFormatFromLAV(pOutFrame->format, pOutFrame->bpp);

  // Get a context
  SwsContext *pContext = sws_getCachedContext(m_pSwsContext, pFrame->width, pFrame->height, (PixelFormat)pFrame->format, pFrame->width, pFrame->height, dstFormat, SWS_BILINEAR, NULL, NULL, NULL);

  // Perform conversion
  int ret = sws_scale(pContext, pFrame->data, pFrame->linesize, 0, pFrame->height, pOutFrame->data, pOutFrame->stride);

  return S_OK;
}

STDMETHODIMP_(REFERENCE_TIME) CDecAvcodec::GetFrameDuration()
{
  if (m_pAVCtx->time_base.den && m_pAVCtx->time_base.num)
    return (REF_SECOND_MULT * m_pAVCtx->time_base.num / m_pAVCtx->time_base.den) * m_pAVCtx->ticks_per_frame;
  return 0;
}
