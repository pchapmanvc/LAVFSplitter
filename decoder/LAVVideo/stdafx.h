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

// pre-compiled header

#pragma once

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN

// Support for Version 6.0 styles
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include "version.h"

// include headers
#include <Windows.h>
#include <Commctrl.h>

#include <d3d9.h>
#include <dxva2api.h>

extern "C" {
#define __STDC_CONSTANT_MACROS
#define FF_API_MAX_STREAMS 0
#include "libavcodec/avcodec.h"
#include "libswscale/swscale.h"
  // This causes a conflict in some structures
#ifdef SampleFormat
#undef SampleFormat
#endif
}
#include "streams.h"

#include "DShowUtil.h"
#include "growarray.h"

#define REF_SECOND_MULT 10000000LL
