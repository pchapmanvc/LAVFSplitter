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

#include <Qnetwork.h>

#include "BaseDemuxer.h"
#include "IKeyFrameInfo.h"

#define FFMPEG_FILE_BUFFER_SIZE   32768 // default reading size for ffmpeg
class CLAVFDemuxer : public CBaseDemuxer, public IAMExtendedSeeking, public IKeyFrameInfo
{
public:
  CLAVFDemuxer(CCritSec *pLock);
  ~CLAVFDemuxer();

  // IUnknown
  DECLARE_IUNKNOWN
  STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void** ppv);

  // IDispatch
  STDMETHODIMP GetTypeInfoCount(UINT* pctinfo) {return E_NOTIMPL;}
  STDMETHODIMP GetTypeInfo(UINT itinfo, LCID lcid, ITypeInfo** pptinfo) {return E_NOTIMPL;}
  STDMETHODIMP GetIDsOfNames(REFIID riid, OLECHAR** rgszNames, UINT cNames, LCID lcid, DISPID* rgdispid) {return E_NOTIMPL;}
  STDMETHODIMP Invoke(DISPID dispidMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS* pdispparams, VARIANT* pvarResult, EXCEPINFO* pexcepinfo, UINT* puArgErr) {return E_NOTIMPL;}

  // CBaseDemuxer
  STDMETHODIMP Open(LPCOLESTR pszFileName);
  REFERENCE_TIME GetDuration() const;
  STDMETHODIMP GetNextPacket(Packet **ppPacket);
  STDMETHODIMP Seek(REFERENCE_TIME rTime);
  const char *GetContainerFormat() const;
  HRESULT StreamInfo(DWORD streamId, LCID *plcid, WCHAR **ppszName) const;

  // Select the best video stream
  const stream* SelectVideoStream();
  // Select the best audio stream
  const stream* SelectAudioStream(std::list<std::string> prefLanguages);
  // Select the best subtitle stream
  const stream* SelectSubtitleStream(std::list<std::string> prefLanguages, int subtitleMode, BOOL bOnlyMatching);


  // IAMExtendedSeeking
  STDMETHODIMP get_ExSeekCapabilities(long* pExCapabilities);
  STDMETHODIMP get_MarkerCount(long* pMarkerCount);
  STDMETHODIMP get_CurrentMarker(long* pCurrentMarker);
  STDMETHODIMP GetMarkerTime(long MarkerNum, double* pMarkerTime);
  STDMETHODIMP GetMarkerName(long MarkerNum, BSTR* pbstrMarkerName);
  STDMETHODIMP put_PlaybackSpeed(double Speed) {return E_NOTIMPL;}
  STDMETHODIMP get_PlaybackSpeed(double* pSpeed) {return E_NOTIMPL;}

  // IKeyFrameInfo
  STDMETHODIMP GetKeyFrameCount(UINT& nKFs);
  STDMETHODIMP GetKeyFrames(const GUID* pFormat, REFERENCE_TIME* pKFs, UINT& nKFs);

private:
  STDMETHODIMP AddStream(int streamId);
  STDMETHODIMP CreateStreams();

  REFERENCE_TIME ConvertTimestampToRT(int64_t pts, int den, int num, int64_t starttime = (int64_t)AV_NOPTS_VALUE) const;
  int64_t ConvertRTToTimestamp(REFERENCE_TIME timestamp, int den, int num, int64_t starttime = (int64_t)AV_NOPTS_VALUE) const;

private:
  AVFormatContext *m_avFormat;

  bool m_bMatroska;
  bool m_bAVI;
  bool m_bMPEGTS;

  unsigned int m_program;

  REFERENCE_TIME m_rtCurrent;
};