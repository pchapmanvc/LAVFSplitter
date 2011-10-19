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
 *
 *  Contributions by Ti-BEN from the XBMC DSPlayer Project, also under GPLv2
 */

#pragma once

#include "dvdmedia.h"
#include "moreuuids.h"

extern "C" {
#include "libavformat/riff.h"
}

const AVCodecTag ff2_codec_wav_tags[] = {
  { CODEC_ID_PCM_S16LE,       0x0001 },
  { CODEC_ID_PCM_U8,          0x0001 }, /* must come after s16le in this list */
  { CODEC_ID_PCM_S24LE,       0x0001 },
  { CODEC_ID_PCM_S32LE,       0x0001 },
  { CODEC_ID_ADPCM_MS,        0x0002 },
  { CODEC_ID_PCM_F32LE,       0x0003 },
  { CODEC_ID_PCM_F64LE,       0x0003 }, /* must come after f32le in this list */
  { CODEC_ID_PCM_ALAW,        0x0006 },
  { CODEC_ID_PCM_MULAW,       0x0007 },
  { CODEC_ID_WMAVOICE,        0x000A },
  { CODEC_ID_ADPCM_IMA_WAV,   0x0011 },
  { CODEC_ID_PCM_ZORK,        0x0011 }, /* must come after adpcm_ima_wav in this list */
  { CODEC_ID_ADPCM_YAMAHA,    0x0020 },
  { CODEC_ID_TRUESPEECH,      0x0022 },
  { CODEC_ID_GSM_MS,          0x0031 },
  { CODEC_ID_ADPCM_G726,      0x0045 },
  { CODEC_ID_MP2,             0x0050 },
  { CODEC_ID_MP3,             0x0055 },
  { CODEC_ID_AMR_NB,          0x0057 },
  { CODEC_ID_AMR_WB,          0x0058 },
  { CODEC_ID_ADPCM_IMA_DK4,   0x0061 },  /* rogue format number */
  { CODEC_ID_ADPCM_IMA_DK3,   0x0062 },  /* rogue format number */
  { CODEC_ID_ADPCM_IMA_WAV,   0x0069 },
  { CODEC_ID_VOXWARE,         0x0075 },
  { CODEC_ID_AAC,             0x00ff },
  { CODEC_ID_SIPR,            0x0130 },
  { CODEC_ID_WMAV1,           0x0160 },
  { CODEC_ID_WMAV2,           0x0161 },
  { CODEC_ID_WMAPRO,          0x0162 },
  { CODEC_ID_WMALOSSLESS,     0x0163 },
  { CODEC_ID_ADPCM_CT,        0x0200 },
  { CODEC_ID_ATRAC3,          0x0270 },
  { CODEC_ID_IMC,             0x0401 },
  { CODEC_ID_GSM_MS,          0x1500 },
  { CODEC_ID_TRUESPEECH,      0x1501 },
  { CODEC_ID_AC3,             0x2000 },
  { CODEC_ID_TRUEHD,          0x2000 }, /* HACK: TrueHD is being send as AC3 to make ffdshow happy */
  { CODEC_ID_DTS,             0x2001 },
  { CODEC_ID_SONIC,           0x2048 },
  { CODEC_ID_SONIC_LS,        0x2048 },
  { CODEC_ID_PCM_MULAW,       0x6c75 },
  { CODEC_ID_AAC,             0x706d },
  { CODEC_ID_AAC,             0x4143 },
  { CODEC_ID_FLAC,            0xF1AC },
  { CODEC_ID_ADPCM_SWF,       ('S'<<8)+'F' },
  { CODEC_ID_VORBIS,          0x6771 }, //HACK/FIXME, does vorbis in WAV/AVI have an (in)official id?
  { CODEC_ID_WAVPACK,         0x5756 }, // WAVE_FORMAT_WAVPACK4
  { CODEC_ID_TTA,             0x77A1 }, // WAVE_FORMAT_TTA1

  /* FIXME: All of the IDs below are not 16 bit and thus illegal. */
  // for NuppelVideo (nuv.c)
  { CODEC_ID_PCM_S16LE, MKTAG('R', 'A', 'W', 'A') },
  { CODEC_ID_MP3,       MKTAG('L', 'A', 'M', 'E') },
  { CODEC_ID_MP3,       MKTAG('M', 'P', '3', ' ') },
  { CODEC_ID_NONE,      0 },
};

const AVCodecTag mp_wav_tags[] = {
  { CODEC_ID_ADPCM_4XM,         MKTAG('4', 'X', 'M', 'A')},
  { CODEC_ID_ADPCM_ADX,         MKTAG('S', 'a', 'd', 'x')},
  { CODEC_ID_ADPCM_EA,          MKTAG('A', 'D', 'E', 'A')},
  { CODEC_ID_ADPCM_EA_MAXIS_XA, MKTAG('A', 'D', 'X', 'A')},
  { CODEC_ID_ADPCM_IMA_WS,      MKTAG('A', 'I', 'W', 'S')},
  { CODEC_ID_ADPCM_THP,         MKTAG('T', 'H', 'P', 'A')},
  { CODEC_ID_ADPCM_XA,          MKTAG('P', 'S', 'X', 'A')},
  { CODEC_ID_AMR_NB,            MKTAG('n', 'b',   0,   0)},
  { CODEC_ID_BINKAUDIO_DCT,     MKTAG('B', 'A', 'U', '1')},
  { CODEC_ID_BINKAUDIO_RDFT,    MKTAG('B', 'A', 'U', '2')},
  { CODEC_ID_COOK,              MKTAG('c', 'o', 'o', 'k')},
  { CODEC_ID_DSICINAUDIO,       MKTAG('D', 'C', 'I', 'A')},
  { CODEC_ID_EAC3,              MKTAG('E', 'A', 'C', '3')},
  { CODEC_ID_INTERPLAY_DPCM,    MKTAG('I', 'N', 'P', 'A')},
  { CODEC_ID_MLP,               MKTAG('M', 'L', 'P', ' ')},
  { CODEC_ID_MP1,               0x50},
  { CODEC_ID_MP4ALS,            MKTAG('A', 'L', 'S', ' ')},
  { CODEC_ID_MUSEPACK7,         MKTAG('M', 'P', 'C', ' ')},
  { CODEC_ID_MUSEPACK8,         MKTAG('M', 'P', 'C', '8')},
  { CODEC_ID_NELLYMOSER,        MKTAG('N', 'E', 'L', 'L')},
  { CODEC_ID_QCELP,             MKTAG('Q', 'c', 'l', 'p')},
  { CODEC_ID_QDM2,              MKTAG('Q', 'D', 'M', '2')},
  { CODEC_ID_RA_144,            MKTAG('1', '4', '_', '4')},
  { CODEC_ID_RA_288,            MKTAG('2', '8', '_', '8')},
  { CODEC_ID_ROQ_DPCM,          MKTAG('R', 'o', 'Q', 'A')},
  { CODEC_ID_SHORTEN,           MKTAG('s', 'h', 'r', 'n')},
  { CODEC_ID_SPEEX,             MKTAG('s', 'p', 'x', ' ')},
  { CODEC_ID_TWINVQ,            MKTAG('T', 'W', 'I', '2')},
  { CODEC_ID_WESTWOOD_SND1,     MKTAG('S', 'N', 'D', '1')},
  { CODEC_ID_XAN_DPCM,          MKTAG('A', 'x', 'a', 'n')},

};

const struct AVCodecTag * const mp_wav_taglists[] = {ff2_codec_wav_tags, mp_wav_tags, 0};

class CLAVFAudioHelper
{
public:
  CLAVFAudioHelper() {};
  CMediaType initAudioType(CodecID codecId, unsigned int &codecTag, std::string container);

  WAVEFORMATEX *CreateWVFMTEX(const AVStream *avstream, ULONG *size);
  WAVEFORMATEXFFMPEG *CreateWVFMTEX_FF(const AVStream *avstream, ULONG *size);
  WAVEFORMATEX_HDMV_LPCM *CreateWVFMTEX_LPCM(const AVStream *avstream, ULONG *size);
  WAVEFORMATEXTENSIBLE *CreateWFMTEX_RAW_PCM(const AVStream *avstream, ULONG *size, const GUID subtype);
  MPEG1WAVEFORMAT *CreateMP1WVFMT(const AVStream *avstream, ULONG *size);
  VORBISFORMAT *CreateVorbis(const AVStream *avstream, ULONG *size);
  VORBISFORMAT2 *CreateVorbis2(const AVStream *avstream, ULONG *size);
};

extern CLAVFAudioHelper g_AudioHelper;
