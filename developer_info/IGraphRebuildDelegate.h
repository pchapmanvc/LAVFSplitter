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
 *
 */

#pragma once

// {30512B26-9CBC-455D-98CC-6CB9CA3788A5}
DEFINE_GUID(IID_IGraphRebuildDelegate, 
0x30512b26, 0x9cbc, 0x455d, 0x98, 0xcc, 0x6c, 0xb9, 0xca, 0x37, 0x88, 0xa5);

[uuid("30512B26-9CBC-455D-98CC-6CB9CA3788A5")]
interface IGraphRebuildDelegate : public IUnknown
{
  // Called by the splitter to let the player know that a certain pin needs to be rebuild.
  //
  // This function is called on any stream or title changes that would cause the content of the pin to change.
  // The Splitter will stop the graph prior to this call, and it will return the graph to its previous state afterwards.
  //
  // When this function is called, the pin will already have the new media types set, so the usual functions to query the media types can be used.
  // EnmuMediaTypes is OK (or any wrappers of this in the base classes), ConnectionMediaType is not OK, for obvious reasons.
  //
  // Following return values are supported:
  // S_OK    - The player took complete control over the rebuild, the splitter will do no further actions regarding the pin
  // S_FALSE - The player may or may not have changed the pin, and the splitter is instructed to send a media type with the next packet.
  // E_FAIL  - The player failed, and the splitter should try to rebuild the pin
  STDMETHOD(RebuildPin)(IFilterGraph *pGraph, IPin *pPin) = 0;
};
