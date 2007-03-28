// $Header$
/***************************************************************************
 *   Copyright (C) 2006 by Guy Rutenberg   *
 *   guyrutenberg@gmail.com   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
/**\file wxDynamicBitmap.h
 *\brief Header file for DBwrapper class.
 *
 * This file contains the header for the wxDynamicBitmap class and all the nessecery include files.
*/

#include <wx/wx.h>
//#include <wx/control.h>

#ifndef WXDYNAMICBITMAP
#define WXDYNAMICBITMAP

/// wxDynamicBitmap class - a wrapper for databases
/**
the wxDynamicBitmap class is a wxWidgets class for dynamicly displaying bitmaps. This is different
than wxStaticBitmap in the fact that it immediatly draws the bitmap and doesn't wait for the paint event.
*/

class wxDynamicBitmap : public wxControl {
public:
	wxDynamicBitmap (wxWindow* parent, wxWindowID id, wxBitmap&  bitmap,
		 const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
		 long style = wxNO_BORDER, const wxString& name = wxPanelNameStr);
	virtual void SetBitmap(wxBitmap&  bitmap);
	wxBitmap GetBitmap();
	void OnPaint(wxPaintEvent& event);
	void OnClick(wxMouseEvent& event);
	wxSize DoGetBestSize() const;
private:
	void PaintBitmap(wxDC& dc);
	wxBitmap m_bitmap;
};

#endif //WXDYNAMICBITMAP
