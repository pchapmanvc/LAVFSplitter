/*
 *      Copyright (C) 2011 Hendrik Leppkes
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

#include <Unknwn.h>       // IUnknown and GUID Macros

// {3E114919-E6F7-41EA-91D6-929821EB0993}
DEFINE_GUID(IID_ILAVFSettings, 
0x3e114919, 0xe6f7, 0x41ea, 0x91, 0xd6, 0x92, 0x98, 0x21, 0xeb, 0x9, 0x93);

[uuid("3E114919-E6F7-41EA-91D6-929821EB0993")]
interface ILAVFSettings : public IUnknown
{
  // Switch to Runtime Config mode. This will reset all settings to default, and no changes to the settings will be saved
  // You can use this to programmatically configure LAV Splitter without interfering with the users settings in the registry.
  // Subsequent calls to this function will reset all settings back to defaults, even if the mode does not change.
  //
  // Note that calling this function during playback is not supported and may exhibit undocumented behaviour. 
  // For smooth operations, it must be called before LAV Splitter opens a file.
  STDMETHOD(SetRuntimeConfig)(BOOL bRuntimeConfig) = 0;

  // Retrieve the preferred languages as ISO 639-2 language codes, comma seperated
  // If the result is NULL, no language has been set
  // Memory for the string will be allocated, and has to be free'ed by the caller with CoTaskMemFree
  STDMETHOD(GetPreferredLanguages)(WCHAR **ppLanguages) = 0;

  // Set the preferred languages as ISO 639-2 language codes, comma seperated
  // To reset to no preferred language, pass NULL or the empty string
  STDMETHOD(SetPreferredLanguages)(WCHAR *pLanguages) = 0;
  
  // Retrieve the preferred subtitle languages as ISO 639-2 language codes, comma seperated
  // If the result is NULL, no language has been set
  // If no subtitle language is set, the main language preference is used.
  // Memory for the string will be allocated, and has to be free'ed by the caller with CoTaskMemFree
  STDMETHOD(GetPreferredSubtitleLanguages)(WCHAR **ppLanguages) = 0;

  // Set the preferred subtitle languages as ISO 639-2 language codes, comma seperated
  // To reset to no preferred language, pass NULL or the empty string
  // If no subtitle language is set, the main language preference is used.
  STDMETHOD(SetPreferredSubtitleLanguages)(WCHAR *pLanguages) = 0;

  // Get the current subtitle mode
  // 0 = No Subs; 1 = Forced Subs; 2 = All subs
  STDMETHOD_(DWORD,GetSubtitleMode)() = 0;

  // Set the current subtitle mode
  // 0 = No Subs; 1 = Forced Subs; 2 = All subs
  STDMETHOD(SetSubtitleMode)(DWORD dwMode) = 0;

  // Get the subtitle matching language flag
  // TRUE = Only subtitles with a language in the preferred list will be used; FALSE = All subtitles will be used
  STDMETHOD_(BOOL,GetSubtitleMatchingLanguage)() = 0;

  // Set the subtitle matching language flag
  // TRUE = Only subtitles with a language in the preferred list will be used; FALSE = All subtitles will be used
  STDMETHOD(SetSubtitleMatchingLanguage)(BOOL dwMode) = 0;

  // Control wether a special "Forced Subtitles" stream will be created for PGS subs
  STDMETHOD_(BOOL,GetPGSForcedStream)() = 0;

  // Control wether a special "Forced Subtitles" stream will be created for PGS subs
  STDMETHOD(SetPGSForcedStream)(BOOL bFlag) = 0;

  // Get the PGS forced subs config
  // TRUE = only forced PGS frames will be shown, FALSE = all frames will be shown
  STDMETHOD_(BOOL,GetPGSOnlyForced)() = 0;

  // Set the PGS forced subs config
  // TRUE = only forced PGS frames will be shown, FALSE = all frames will be shown
  STDMETHOD(SetPGSOnlyForced)(BOOL bForced) = 0;

  // Get the VC-1 Timestamp Processing mode
  // 0 - No Timestamp Correction, 1 - Always Timestamp Correction, 2 - Auto (Correction for Decoders that need it)
  STDMETHOD_(int,GetVC1TimestampMode)() = 0;
  
  // Set the VC-1 Timestamp Processing mode
  // 0 - No Timestamp Correction, 1 - Always Timestamp Correction, 2 - Auto (Correction for Decoders that need it)
  STDMETHOD(SetVC1TimestampMode)(int iMode) = 0;

  // Set whether substreams (AC3 in TrueHD, for example) should be shown as a seperate stream
  STDMETHOD(SetSubstreamsEnabled)(BOOL bSubStreams) = 0;

  // Check whether substreams (AC3 in TrueHD, for example) should be shown as a seperate stream
  STDMETHOD_(BOOL,GetSubstreamsEnabled)() = 0;

  // Set if the ffmpeg parsers should be used for video streams
  STDMETHOD(SetVideoParsingEnabled)(BOOL bEnabled) = 0;
  
  // Query if the ffmpeg parsers are being used for video streams
  STDMETHOD_(BOOL,GetVideoParsingEnabled)() = 0;

  // Set if LAV Splitter should try to fix broken HD-PVR streams
  STDMETHOD(SetFixBrokenHDPVR)(BOOL bEnabled) = 0;

  // Query if LAV Splitter should try to fix broken HD-PVR streams
  STDMETHOD_(BOOL,GetFixBrokenHDPVR)() = 0;

  // Control wether the givne format is enabled
  STDMETHOD_(HRESULT,SetFormatEnabled)(const char *strFormat, BOOL bEnabled) = 0;

  // Check if the given format is enabled
  STDMETHOD_(BOOL,IsFormatEnabled)(const char *strFormat) = 0;

  // Set if LAV Splitter should always completely remove the filter connected to its Audio Pin when the audio stream is changed
  STDMETHOD(SetStreamSwitchRemoveAudio)(BOOL bEnabled) = 0;

  // Query if LAV Splitter should always completely remove the filter connected to its Audio Pin when the audio stream is changed
  STDMETHOD_(BOOL,GetStreamSwitchRemoveAudio)() = 0;
};
