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

#pragma once

// {CFEE44C0-7BCA-49EA-8FE1-B52915D5BEA4}
DEFINE_GUID(IID_ILAVAudioSettings, 
0xcfee44c0, 0x7bca, 0x49ea, 0x8f, 0xe1, 0xb5, 0x29, 0x15, 0xd5, 0xbe, 0xa4);

// {41A703AD-EAEA-4782-A4C6-E83B57DC0AED}
DEFINE_GUID(IID_ILAVAudioStatus,
0x41a703ad, 0xeaea, 0x4782, 0xa4, 0xc6, 0xe8, 0x3b, 0x57, 0xdc, 0xa, 0xed);

// Codecs supported in the LAV Audio configuration
// Codecs not listed here cannot be turned off. You can request codecs to be added to this list, if you wish.
typedef enum LAVAudioCodec {
  Codec_AAC,
  Codec_AC3,
  Codec_EAC3,
  Codec_DTS,
  Codec_MP2,
  Codec_MP3,
  Codec_TRUEHD,
  Codec_FLAC,
  Codec_VORBIS,
  Codec_LPCM,
  Codec_PCM,
  Codec_WAVPACK,
  Codec_TTA,
  Codec_WMA2,
  Codec_WMAPRO,

  Codec_NB            // Number of entrys (do not use when dynamically linking)
};

// Bitstreaming Codecs supported in LAV Audio
typedef enum LAVBitstreamCodec {
  Bitstream_AC3,
  Bitstream_EAC3,
  Bitstream_TRUEHD,
  Bitstream_DTS,
  Bitstream_DTSHD,

  Bitstream_NB        // Number of entrys (do not use when dynamically linking)
};


// Supported Sample Formats in LAV Audio
typedef enum LAVAudioSampleFormat {
  SampleFormat_16,
  SampleFormat_24,
  SampleFormat_32,
  SampleFormat_U8,
  SampleFormat_FP32,
  SampleFormat_Bitstream,

  SampleFormat_NB     // Number of entrys (do not use when dynamically linking)
};

// LAV Audio configuration interface
[uuid("CFEE44C0-7BCA-49EA-8FE1-B52915D5BEA4")]
interface ILAVAudioSettings : public IUnknown
{
  // Switch to Runtime Config mode. This will reset all settings to default, and no changes to the settings will be saved
  // You can use this to programmatically configure LAV Audio without interfering with the users settings in the registry.
  // Subsequent calls to this function will reset all settings back to defaults, even if the mode does not change.
  //
  // Note that calling this function during playback is not supported and may exhibit undocumented behaviour. 
  // For smooth operations, it must be called before LAV Audio is connected to other filters.
  STDMETHOD(SetRuntimeConfig)(BOOL bRuntimeConfig) = 0;

  // Dynamic Range Compression
  // pbDRCEnabled: The state of DRC
  // piDRCLevel:   The DRC strength (0-100, 100 is maximum)
  STDMETHOD(GetDRC)(BOOL *pbDRCEnabled, int *piDRCLevel) = 0;
  STDMETHOD(SetDRC)(BOOL bDRCEnabled, int iDRCLevel) = 0;
  
  // Configure which codecs are enabled
  // If aCodec is invalid (possibly a version difference), Get will return FALSE, and Set E_FAIL.
  STDMETHOD_(BOOL,GetFormatConfiguration)(LAVAudioCodec aCodec) = 0;
  STDMETHOD(SetFormatConfiguration)(LAVAudioCodec aCodec, BOOL bEnabled) = 0;

  // Control Bitstreaming
  // If bsCodec is invalid (possibly a version difference), Get will return FALSE, and Set E_FAIL.
  STDMETHOD_(BOOL, GetBitstreamConfig)(LAVBitstreamCodec bsCodec) = 0;
  STDMETHOD(SetBitstreamConfig)(LAVBitstreamCodec bsCodec, BOOL bEnabled) = 0;
  
  // Should "normal" DTS frames be encapsulated in DTS-HD frames when bitstreaming?
  STDMETHOD_(BOOL,GetDTSHDFraming)() = 0;
  STDMETHOD(SetDTSHDFraming)(BOOL bHDFraming) = 0;

  // Control Auto A/V syncing
  STDMETHOD_(BOOL,GetAutoAVSync)() = 0;
  STDMETHOD(SetAutoAVSync)(BOOL bAutoSync) = 0;

  // Convert all Channel Layouts to standard layouts
  // Standard are: Mono, Stereo, 5.1, 6.1, 7.1
  STDMETHOD_(BOOL,GetOutputStandardLayout)() = 0;
  STDMETHOD(SetOutputStandardLayout)(BOOL bStdLayout) = 0;
  
  // Expand Mono to Stereo by simply doubling the audio
  STDMETHOD_(BOOL,GetExpandMono)() = 0;
  STDMETHOD(SetExpandMono)(BOOL bExpandMono) = 0;

  // Expand 6.1 to 7.1 by doubling the back center
  STDMETHOD_(BOOL,GetExpand61)() = 0;
  STDMETHOD(SetExpand61)(BOOL bExpand61) = 0;

  // Allow Raw PCM and SPDIF encoded input
  STDMETHOD_(BOOL,GetAllowRawSPDIFInput)() = 0;
  STDMETHOD(SetAllowRawSPDIFInput)(BOOL bAllow) = 0;

  // Configure which sample formats are enabled
  // Note: SampleFormat_Bitstream cannot be controlled by this
  STDMETHOD_(BOOL,GetSampleFormat)(LAVAudioSampleFormat format) = 0;
  STDMETHOD(SetSampleFormat)(LAVAudioSampleFormat format, BOOL bEnabled) = 0;

  // Configure a delay for the audio
  STDMETHOD(GetAudioDelay)(BOOL *pbEnabled, int *pDelay) = 0;
  STDMETHOD(SetAudioDelay)(BOOL bEnabled, int delay) = 0;
};

// LAV Audio Status Interface
// Get the current playback stats
[uuid("41A703AD-EAEA-4782-A4C6-E83B57DC0AED")]
interface ILAVAudioStatus : public IUnknown
{
  // Check if the given sample format is supported by the current playback chain
  STDMETHOD_(BOOL,IsSampleFormatSupported)(LAVAudioSampleFormat sfCheck) = 0;

  // Get details about the current decoding format
  STDMETHOD(GetDecodeDetails)(const char **pCodec, const char **pDecodeFormat, int *pnChannels, int *pSampleRate, DWORD *pChannelMask) = 0;
  
  // Get details about the current output format
  STDMETHOD(GetOutputDetails)(const char **pOutputFormat, int *pnChannels, int *pSampleRate, DWORD *pChannelMask) = 0;
  
  // Enable Volume measurements
  STDMETHOD(EnableVolumeStats)() = 0;

  // Disable Volume measurements
  STDMETHOD(DisableVolumeStats)() = 0;

  // Get Volume Average for the given channel
  STDMETHOD(GetChannelVolumeAverage)(WORD nChannel, float *pfDb) = 0;
};
