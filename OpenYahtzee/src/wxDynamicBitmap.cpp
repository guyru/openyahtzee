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

#include "wxDynamicBitmap.h"
#include <wx/wx.h>

#include <iostream>

wxDynamicBitmap::wxDynamicBitmap(wxWindow* parent, wxWindowID id, wxBitmap&  bitmap,
				const wxPoint& pos, const wxSize& size,
				long style, const wxString& name)
{
	wxControl::Create(parent,id,pos,size,style,wxDefaultValidator,name);
	Connect(id, wxEVT_PAINT, wxPaintEventHandler(wxDynamicBitmap::OnPaint));
	SetBitmap( bitmap);
}

void wxDynamicBitmap::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);
	PaintBitmap(dc);
}

wxBitmap wxDynamicBitmap::GetBitmap()
{
	return m_bitmap;
}

void wxDynamicBitmap::SetBitmap( wxBitmap& bitmap)
{
	m_bitmap = bitmap;
	wxWindow::Refresh();
	wxWindow::Update();
}

void wxDynamicBitmap::PaintBitmap(wxDC& dc)
{
	wxColour backgroundColour = GetBackgroundColour();
	if (!backgroundColour.Ok())
		backgroundColour = wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE);
	dc.SetBrush(wxBrush(backgroundColour));
	dc.SetPen(wxPen(backgroundColour, 1));
	wxRect windowRect(wxPoint(0, 0), GetClientSize());

	dc.DrawRectangle(windowRect);

	dc.DrawBitmap(m_bitmap , 0 , 0, true);
}

wxSize wxDynamicBitmap::DoGetBestSize() const
{
	
	return wxSize(m_bitmap.GetHeight(),m_bitmap.GetWidth());
}
