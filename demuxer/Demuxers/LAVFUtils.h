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
 */

#pragma once

const char *get_stream_language(AVStream *pStream);
HRESULT lavf_describe_stream(AVStream *pStream, WCHAR **ppszName);



inline int get_bits_per_sample(AVCodecContext *ctx)
{
  int bits = av_get_bits_per_sample(ctx->codec_id);
  if (!bits) {
    if (ctx->codec_id == CODEC_ID_PCM_DVD || ctx->codec_id == CODEC_ID_PCM_BLURAY) {
      bits = ctx->bits_per_coded_sample;
    }
    if(!bits) {
      if (ctx->sample_fmt == AV_SAMPLE_FMT_S32 && ctx->bits_per_raw_sample) {
        bits = ctx->bits_per_raw_sample;
      } else {
        bits = av_get_bits_per_sample_fmt(ctx->sample_fmt);
      }
    }
  }
  return bits;
}

extern "C" {
extern URLProtocol ufile_protocol;
#ifdef DEBUG
void lavf_log_callback(void* ptr, int level, const char* fmt, va_list vl);
#endif
}