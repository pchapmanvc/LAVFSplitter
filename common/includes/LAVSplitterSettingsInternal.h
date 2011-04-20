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

#include "LAVSplitterSettings.h"
#include <set>

class FormatInfo {
public:
  FormatInfo() : strName(NULL), strDescription(NULL) {}
  FormatInfo(const char *name, const char *desc) : strName(name), strDescription(desc) {}
  const char *strName;
  const char *strDescription;

  // Comparison operators for sorting (NULL safe)
  bool FormatInfo::operator < (const FormatInfo& rhs) const { return strName ? (rhs.strName ? _stricmp(strName, rhs.strName) < 0 : false) : true; }
  bool FormatInfo::operator > (const FormatInfo& rhs) const { return !(*this < rhs); }
  bool FormatInfo::operator == (const FormatInfo& rhs) const { return (strName == rhs.strName) || (strName && rhs.strName && (_stricmp(strName, rhs.strName) == 0)); }
};

// GUID: 9536a80d-81cb-4ddf-b1be-0848aa8a5462
DEFINE_GUID(IID_ILAVFSettingsInternal, 0x9536a80d, 0x81cb, 
0x4ddf, 0xb1, 0xbe, 0x8, 0x48, 0xaa, 0x8a, 0x54, 0x62);

[uuid("9536a80d-81cb-4ddf-b1be-0848aa8a5462")]
interface ILAVFSettingsInternal : public ILAVFSettings
{
  // Query if the current filter graph requires VC1 Correction
  STDMETHOD_(BOOL,IsVC1CorrectionRequired)() = 0;

  STDMETHOD_(const char*, GetInputFormat)() = 0;
  STDMETHOD_(std::set<FormatInfo>&, GetInputFormats)() = 0;
};
