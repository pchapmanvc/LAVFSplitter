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

// Based on the SampleParser Template by GDCL
// --------------------------------------------------------------------------------
// Copyright (c) GDCL 2004. All Rights Reserved. 
// You are free to re-use this as the basis for your own filter development,
// provided you retain this copyright notice in the source.
// http://www.gdcl.co.uk
// --------------------------------------------------------------------------------

#include "stdafx.h"

// Initialize the GUIDs
#include <InitGuid.h>

#include <qnetwork.h>
#include "LAVSplitter.h"
#include "moreuuids.h"

#include "registry.h"
#include "IGraphRebuildDelegate.h"

// The GUID we use to register the splitter media types
DEFINE_GUID(MEDIATYPE_LAVSplitter,
  0xbfa3a787, 0xf8e7, 0x4d78, 0xaf, 0x78, 0x58, 0x14, 0x8c, 0x74, 0xd0, 0xa9); 

// --- COM factory table and registration code --------------

const AMOVIESETUP_MEDIATYPE 
  sudMediaTypes[] = {
    { &MEDIATYPE_Stream, &MEDIASUBTYPE_NULL },
};

const AMOVIESETUP_PIN sudOutputPins[] = 
{
  {
    L"Output",            // pin name
      FALSE,              // is rendered?    
      TRUE,               // is output?
      FALSE,              // zero instances allowed?
      TRUE,               // many instances allowed?
      &CLSID_NULL,        // connects to filter (for bridge pins)
      NULL,               // connects to pin (for bridge pins)
      0,                  // count of registered media types
      NULL                // list of registered media types
  },
  {
    L"Input",             // pin name
      FALSE,              // is rendered?    
      FALSE,              // is output?
      FALSE,              // zero instances allowed?
      FALSE,              // many instances allowed?
      &CLSID_NULL,        // connects to filter (for bridge pins)
      NULL,               // connects to pin (for bridge pins)
      1,                  // count of registered media types
      &sudMediaTypes[0]   // list of registered media types
  }
};

const AMOVIESETUP_FILTER sudFilterReg =
{
  &__uuidof(CLAVSplitter),        // filter clsid
  L"LAV Splitter",                // filter name
  MERIT_PREFERRED + 4,            // merit
  2,                              // count of registered pins
  sudOutputPins,                  // list of pins to register
  CLSID_LegacyAmFilterCategory
};

const AMOVIESETUP_FILTER sudFilterRegSource =
{
  &__uuidof(CLAVSplitterSource),  // filter clsid
  L"LAV Splitter Source",         // filter name
  MERIT_PREFERRED + 4,            // merit
  1,                              // count of registered pins
  sudOutputPins,                  // list of pins to register
  CLSID_LegacyAmFilterCategory
};

// --- COM factory table and registration code --------------

// DirectShow base class COM factory requires this table, 
// declaring all the COM objects in this DLL
CFactoryTemplate g_Templates[] = {
  // one entry for each CoCreate-able object
  {
    sudFilterReg.strName,
      sudFilterReg.clsID,
      CreateInstance<CLAVSplitter>,
      NULL,
      &sudFilterReg
  },
  {
    sudFilterRegSource.strName,
      sudFilterRegSource.clsID,
      CreateInstance<CLAVSplitterSource>,
      NULL,
      &sudFilterRegSource
  },
  // This entry is for the property page.
  { 
      L"LAV Splitter Properties",
      &CLSID_LAVSplitterSettingsProp,
      CreateInstance<CLAVSplitterSettingsProp>,
      NULL, NULL
  },
  {
      L"LAV Splitter Input Formats",
      &CLSID_LAVSplitterFormatsProp,
      CreateInstance<CLAVSplitterFormatsProp>,
      NULL, NULL
  }
};
int g_cTemplates = sizeof(g_Templates) / sizeof(g_Templates[0]);

// self-registration entrypoint
STDAPI DllRegisterServer()
{
  std::list<LPCWSTR> chkbytes;

  // LXF: header is 'LEITCH\0\0vvvvssss' where \0 is null, vvvv is header
  // version and ssss is header size (little endian).
  chkbytes.push_back(L"0,16,,4C45495443480000000000003C000000"); // old v0 60-byte header
  chkbytes.push_back(L"0,16,,4C454954434800000100000048000000"); // new v1 72-byte header

  RegisterSourceFilter(__uuidof(CLAVSplitterSource),
    MEDIASUBTYPE_LAVLxf,
    chkbytes,
    L".lxf", NULL);

  chkbytes.clear();

  // GXF
  RegisterSourceFilter(__uuidof(CLAVSplitterSource),
    MEDIASUBTYPE_LAVGxf,
    L"0,6,,0000000001bc,14,2,,e1e2",
    L".gxf", NULL);

  // MKV/WEBM
  RegisterSourceFilter(CLSID_AsyncReader,
    MEDIASUBTYPE_Matroska,
    L"0,4,,1A45DFA3",
    L".mkv", L".mka", L".mks", L".webm", NULL);

  // AVI
  chkbytes.push_back(L"0,4,,52494646,8,4,,41564920"); // 'RIFF' ... 'AVI '
  chkbytes.push_back(L"0,4,,52494646,8,4,,41564958"); // 'RIFF' ... 'AVIX'
  chkbytes.push_back(L"0,4,,52494646,8,4,,414D5620"); // 'RIFF' ... 'AMV '
  RegisterSourceFilter(CLSID_AsyncReader,
    MEDIASUBTYPE_Avi,
    chkbytes,
    L".avi", L".divx", L".vp6", L".amv", NULL);

  chkbytes.clear();

  // MP4
  chkbytes.push_back(L"4,4,,66747970"); // ftyp
  chkbytes.push_back(L"4,4,,6d6f6f76"); // moov
  chkbytes.push_back(L"4,4,,6d646174"); // mdat
  chkbytes.push_back(L"4,4,,736b6970"); // skip
  chkbytes.push_back(L"4,4,,75647461"); // udta
  chkbytes.push_back(L"4,12,ffffffff00000000ffffffff,77696465000000006d646174"); // wide ? mdat
  chkbytes.push_back(L"4,12,ffffffff00000000ffffffff,776964650000000066726565"); // wide ? free
  chkbytes.push_back(L"4,12,ffffffff00000000ffffffff,6672656500000000636D6F76"); // free ? cmov
  chkbytes.push_back(L"4,12,ffffffff00000000ffffffff,66726565000000006D766864"); // free ? mvhd
  chkbytes.push_back(L"4,14,ffffffff000000000000ffffffff,706E6F7400000000000050494354"); // pnot ? PICT
  chkbytes.push_back(L"3,3,,000001"); // mpeg4 video

  RegisterSourceFilter(CLSID_AsyncReader,
    MEDIASUBTYPE_MP4,
    chkbytes,
    L".mp4", L".mov", L".3gp", NULL);

  chkbytes.clear();

  // MPEG1
  RegisterSourceFilter(CLSID_AsyncReader,
    MEDIASUBTYPE_MPEG1System,
    L"0,16,FFFFFFFFF100010001800001FFFFFFFF,000001BA2100010001800001000001BB", NULL);

  // MPEG-PVA
  chkbytes.push_back(L"0,8,fffffc00ffe00000,4156000055000000");
  // MPEG-TS
  chkbytes.push_back(L"0,1,,47,188,1,,47,376,1,,47");
  chkbytes.push_back(L"4,1,,47,196,1,,47,388,1,,47");
  chkbytes.push_back(L"0,4,,54467263,1660,1,,47");

  RegisterSourceFilter(CLSID_AsyncReader,
    MEDIASUBTYPE_MPEG2_TRANSPORT,
    chkbytes,
    L".ts", L".mts", L".m2ts", NULL);

  // MPEG-PS
  RegisterSourceFilter(CLSID_AsyncReader,
    MEDIASUBTYPE_MPEG2_PROGRAM,
    L"0,5,FFFFFFFFC0,000001BA40",
    L".mpeg", L".mpg", L".vob", L".evo", NULL);

  // FLV
  RegisterSourceFilter(CLSID_AsyncReader,
    MEDIASUBTYPE_FLV,
    L"0,4,,464C5601",
    L".flv", NULL);

  // Ogg
  RegisterSourceFilter(CLSID_AsyncReader,
    MEDIASUBTYPE_Ogg,
    L"0,4,,4F676753",
    L".ogg", L".ogm", NULL);

  // BluRay
  chkbytes.clear();
  chkbytes.push_back(L"0,4,,494E4458"); // INDX (index.bdmv)
  chkbytes.push_back(L"0,4,,4D4F424A"); // MOBJ (MovieObject.bdmv)
  chkbytes.push_back(L"0,4,,4D504C53"); // MPLS
  RegisterSourceFilter(__uuidof(CLAVSplitterSource),
    MEDIASUBTYPE_LAVBluRay,
    chkbytes,
    L".bdmv", L".mpls", NULL);

  // base classes will handle registration using the factory template table
  return AMovieDllRegisterServer2(true);
}

STDAPI DllUnregisterServer()
{
  // Deprecated and unused types
  UnRegisterSourceFilter(MEDIATYPE_LAVSplitter);
  UnRegisterSourceFilter(MEDIASUBTYPE_LAVAvi);
  UnRegisterSourceFilter(MEDIASUBTYPE_LAVMatroska);
  UnRegisterSourceFilter(MEDIASUBTYPE_LAVMP4);
  UnRegisterSourceFilter(MEDIASUBTYPE_LAVMPEG);
  UnRegisterSourceFilter(MEDIASUBTYPE_LAVFLV);
  UnRegisterSourceFilter(MEDIASUBTYPE_LAVOgg);

  // Current types
  UnRegisterSourceFilter(MEDIASUBTYPE_LAVLxf);
  UnRegisterSourceFilter(MEDIASUBTYPE_LAVGxf);
  UnRegisterSourceFilter(MEDIASUBTYPE_LAVBluRay);

  // Do not unregister default MS types, like MEDIASUBTYPE_Avi

  // base classes will handle de-registration using the factory template table
  return AMovieDllRegisterServer2(false);
}

// if we declare the correct C runtime entrypoint and then forward it to the DShow base
// classes we will be sure that both the C/C++ runtimes and the base classes are initialized
// correctly
extern "C" BOOL WINAPI DllEntryPoint(HINSTANCE, ULONG, LPVOID);
BOOL WINAPI DllMain(HANDLE hDllHandle, DWORD dwReason, LPVOID lpReserved)
{
  return DllEntryPoint(reinterpret_cast<HINSTANCE>(hDllHandle), dwReason, lpReserved);
}

STDAPI OpenConfiguration()
{
  HRESULT hr = S_OK;
  CUnknown *pInstance = CreateInstance<CLAVSplitter>(NULL, &hr);
  IBaseFilter *pFilter = NULL;
  pInstance->NonDelegatingQueryInterface(IID_IBaseFilter, (void **)&pFilter);
  if (pFilter) {
    pFilter->AddRef();
    CBaseDSPropPage::ShowPropPageDialog(pFilter);
  }
  delete pInstance;

  return 0;
}
