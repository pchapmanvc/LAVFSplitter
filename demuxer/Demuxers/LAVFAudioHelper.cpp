/*
 *      Copyright (C) 2010 Hendrik Leppkes
 *      http://www.1f0.de
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 *  http://www.gnu.org/copyleft/gpl.html
 *
 *  Contributions by Ti-BEN from the XBMC DSPlayer Project, also under GPLv2
 */

#include "stdafx.h"
#include "moreuuids.h"
#include "BaseDemuxer.h"
#include "LAVFAudioHelper.h"
#include "LAVFUtils.h"

#include "ExtradataParser.h"

CLAVFAudioHelper g_AudioHelper;

CMediaType CLAVFAudioHelper::initAudioType(CodecID codecId, unsigned int &codecTag)
{
  CMediaType mediaType;
  mediaType.InitMediaType();
  mediaType.majortype = MEDIATYPE_Audio;
  mediaType.subtype = FOURCCMap(codecTag);
  mediaType.formattype = FORMAT_WaveFormatEx; //default value
  mediaType.SetSampleSize(256000);

  // special cases
  switch(codecId)
  {
  case CODEC_ID_AC3:
    mediaType.subtype = MEDIASUBTYPE_DOLBY_AC3;
    break;
  case CODEC_ID_AAC:
    mediaType.subtype = MEDIASUBTYPE_AAC;
    codecTag = WAVE_FORMAT_AAC;
    break;
  case CODEC_ID_AAC_LATM:
    mediaType.subtype = MEDIASUBTYPE_LATM_AAC;
    codecTag = WAVE_FORMAT_LATM_AAC;
    break;
  case CODEC_ID_DTS:
    mediaType.subtype = MEDIASUBTYPE_DTS;
    codecTag = WAVE_FORMAT_DTS;
    break;
  case CODEC_ID_EAC3:
    mediaType.subtype = MEDIASUBTYPE_DOLBY_DDPLUS;
    break;
  case CODEC_ID_TRUEHD:
    mediaType.subtype = MEDIASUBTYPE_DOLBY_TRUEHD;
    break;
  case CODEC_ID_VORBIS:
    //TODO
    mediaType.formattype = FORMAT_VorbisFormat;
    mediaType.subtype = MEDIASUBTYPE_Vorbis;
    break;
  case CODEC_ID_MP1:
  case CODEC_ID_MP2:
    mediaType.subtype = MEDIASUBTYPE_MPEG1AudioPayload;
    break;
  case CODEC_ID_MP3:
    mediaType.subtype = MEDIASUBTYPE_MP3;
    break;
  case CODEC_ID_PCM_BLURAY:
    mediaType.subtype = MEDIASUBTYPE_BD_LPCM_AUDIO;
    break;
  case CODEC_ID_PCM_DVD:
    mediaType.subtype = MEDIASUBTYPE_DVD_LPCM_AUDIO;
    break;
  }
  return mediaType;
}

WAVEFORMATEX *CLAVFAudioHelper::CreateWVFMTEX(const AVStream *avstream, ULONG *size) {
  WAVEFORMATEX *wvfmt = (WAVEFORMATEX *)CoTaskMemAlloc(sizeof(WAVEFORMATEX) + avstream->codec->extradata_size);
  memset(wvfmt, 0, sizeof(WAVEFORMATEX));

  wvfmt->wFormatTag = avstream->codec->codec_tag;

  wvfmt->nChannels = avstream->codec->channels;
  wvfmt->nSamplesPerSec = avstream->codec->sample_rate;
  wvfmt->nAvgBytesPerSec = avstream->codec->bit_rate / 8;

  if(avstream->codec->codec_id == CODEC_ID_AAC || avstream->codec->codec_id == CODEC_ID_AAC_LATM) {
    wvfmt->wBitsPerSample = 0;
    wvfmt->nBlockAlign = 1;
  } else {
    wvfmt->wBitsPerSample = get_bits_per_sample(avstream->codec);

    if ( avstream->codec->block_align > 0 ) {
      wvfmt->nBlockAlign = avstream->codec->block_align;
    } else {
      if ( wvfmt->wBitsPerSample == 0 ) {
        DbgOutString(L"BitsPerSample is 0, no good!");
      }
      wvfmt->nBlockAlign = (WORD)((wvfmt->nChannels * av_get_bits_per_sample_format(avstream->codec->sample_fmt)) / 8);
    }
  }

  if (avstream->codec->extradata_size > 0) {
    wvfmt->cbSize = avstream->codec->extradata_size;
    memcpy((BYTE *)wvfmt + sizeof(WAVEFORMATEX), avstream->codec->extradata, avstream->codec->extradata_size);
  }

  *size = sizeof(WAVEFORMATEX) + avstream->codec->extradata_size;
  return wvfmt;
}

WAVEFORMATEX_HDMV_LPCM *CLAVFAudioHelper::CreateWVFMTEX_LPCM(const AVStream *avstream, ULONG *size) {
  WAVEFORMATEX *wvfmt = CreateWVFMTEX(avstream, size);

  WAVEFORMATEX_HDMV_LPCM *lpcm = (WAVEFORMATEX_HDMV_LPCM *)CoTaskMemAlloc(sizeof(WAVEFORMATEX_HDMV_LPCM));
  memset(lpcm, 0, sizeof(WAVEFORMATEX_HDMV_LPCM));
  memcpy(lpcm, wvfmt, sizeof(WAVEFORMATEX));

  lpcm->cbSize = sizeof(WAVEFORMATEX_HDMV_LPCM) - sizeof(WAVEFORMATEX);
  BYTE channel_conf = 0;
  switch (avstream->codec->channel_layout) {
  case AV_CH_LAYOUT_MONO:
    channel_conf = 1;
    break;
  case AV_CH_LAYOUT_STEREO:
    channel_conf = 3;
    break;
  case AV_CH_LAYOUT_SURROUND:
    channel_conf = 4;
    break;
  case AV_CH_LAYOUT_2_1:
    channel_conf = 5;
    break;
  case AV_CH_LAYOUT_4POINT0:
    channel_conf = 6;
    break;
  case AV_CH_LAYOUT_2_2:
    channel_conf = 7;
    break;
  case AV_CH_LAYOUT_5POINT0:
    channel_conf = 8;
    break;
  case AV_CH_LAYOUT_5POINT1:
    channel_conf = 9;
    break;
  case AV_CH_LAYOUT_7POINT0:
    channel_conf = 10;
    break;
  case AV_CH_LAYOUT_7POINT1:
    channel_conf = 11;
    break;
  default:
    channel_conf = 0;
  }
  lpcm->channel_conf = channel_conf;

  CoTaskMemFree(wvfmt);

  *size = sizeof(WAVEFORMATEX_HDMV_LPCM);
  return lpcm;
}

MPEG1WAVEFORMAT *CLAVFAudioHelper::CreateMP1WVFMT(const AVStream *avstream, ULONG *size) {
  WAVEFORMATEX *wvfmt = CreateWVFMTEX(avstream, size);

  MPEG1WAVEFORMAT *mpwvfmt = (MPEG1WAVEFORMAT *)CoTaskMemAlloc(sizeof(MPEG1WAVEFORMAT));
  memset(mpwvfmt, 0, sizeof(MPEG1WAVEFORMAT));
  memcpy(&mpwvfmt->wfx, wvfmt, sizeof(WAVEFORMATEX));

  mpwvfmt->dwHeadBitrate = avstream->codec->bit_rate;
  mpwvfmt->fwHeadMode = avstream->codec->channels == 1 ? ACM_MPEG_SINGLECHANNEL : ACM_MPEG_DUALCHANNEL;
  mpwvfmt->fwHeadLayer = (avstream->codec->codec_id == CODEC_ID_MP1) ? ACM_MPEG_LAYER1 : ACM_MPEG_LAYER2;

  mpwvfmt->wfx.wFormatTag = WAVE_FORMAT_MPEG;
  mpwvfmt->wfx.nBlockAlign = (avstream->codec->codec_id == CODEC_ID_MP1)
        ? (12 * avstream->codec->bit_rate / avstream->codec->sample_rate) * 4
        : 144 * avstream->codec->bit_rate / avstream->codec->sample_rate;

  mpwvfmt->wfx.cbSize = sizeof(MPEG1WAVEFORMAT) - sizeof(WAVEFORMATEX);

  CoTaskMemFree(wvfmt);

  *size = sizeof(MPEG1WAVEFORMAT);
  return mpwvfmt;
}

VORBISFORMAT *CLAVFAudioHelper::CreateVorbis(const AVStream *avstream, ULONG *size) {
  VORBISFORMAT *vfmt = (VORBISFORMAT *)CoTaskMemAlloc(sizeof(VORBISFORMAT));
  memset(vfmt, 0, sizeof(VORBISFORMAT));

  vfmt->nChannels = avstream->codec->channels;
  vfmt->nSamplesPerSec = avstream->codec->sample_rate;
  vfmt->nAvgBitsPerSec = avstream->codec->bit_rate;
  vfmt->nMinBitsPerSec = vfmt->nMaxBitsPerSec = (DWORD)-1;

  *size = sizeof(VORBISFORMAT);
  return vfmt;
}

VORBISFORMAT2 *CLAVFAudioHelper::CreateVorbis2(const AVStream *avstream, ULONG *size) {
  BYTE *p = avstream->codec->extradata;
  std::vector<int> sizes;
  // read the number of blocks, and then the sizes of the individual blocks
  for(BYTE n = *p++; n > 0; n--) {
    int size = 0;
    // Xiph Lacing
    do { size = *p; } while (*p++ == 0xFF);
    sizes.push_back(size);
  }
  
  int totalsize = 0;
  for(unsigned int i = 0; i < sizes.size(); i++)
    totalsize += sizes[i];

  // Get the size of the last block
  sizes.push_back(avstream->codec->extradata_size - (int)(p - avstream->codec->extradata) - totalsize);
  totalsize += sizes[sizes.size()-1];

  // 3 blocks is the currently valid Vorbis format
  if(sizes.size() == 3) {
    VORBISFORMAT2* pvf2 = (VORBISFORMAT2*)CoTaskMemAlloc(sizeof(VORBISFORMAT2) + totalsize);
    memset(pvf2, 0, sizeof(VORBISFORMAT2));
    
    pvf2->Channels = avstream->codec->channels;
    pvf2->SamplesPerSec = avstream->codec->sample_rate;
    pvf2->BitsPerSample = get_bits_per_sample(avstream->codec);
    
    BYTE *p2 = (BYTE *)pvf2 + sizeof(VORBISFORMAT2);
    for(unsigned int i = 0; i < sizes.size(); p += sizes[i], p2 += sizes[i], i++) {
      memcpy(p2, p, pvf2->HeaderSize[i] = sizes[i]);
    }

    *size = sizeof(VORBISFORMAT2) + totalsize;
    return pvf2;
  }
  *size = 0;
  return NULL;
}