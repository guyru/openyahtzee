// $Header$
/***************************************************************************
 *   Copyright (C) 2006-2007 by Guy Rutenberg   *
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
#include "ObjectsID.h"

#include <iostream>

wxDynamicBitmap::wxDynamicBitmap(wxWindow* parent, wxWindowID id, wxBitmap *bitmap,
				const wxPoint& pos, const wxSize& size,
				long style, const wxString& name)
{
	wxControl::Create(parent,id,pos,size,style,wxDefaultValidator,name);
	Connect(id, wxEVT_PAINT, wxPaintEventHandler(wxDynamicBitmap::OnPaint));
	Connect(id, wxEVT_LEFT_UP,wxMouseEventHandler(wxDynamicBitmap::OnClick));
	m_grayscale=false;
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

void wxDynamicBitmap::SetBitmap( wxBitmap *bitmap)
{
	m_bitmap = *bitmap;
	SetGrayScale(m_grayscale);
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

	dc.DrawBitmap((m_grayscale?m_graybitmap:m_bitmap) , 0 , 0, true);
	
}

wxSize wxDynamicBitmap::DoGetBestSize() const
{
	
	return wxSize(m_bitmap.GetWidth(),m_bitmap.GetHeight());
}

void wxDynamicBitmap::OnClick(wxMouseEvent& event)
{
	//A hack that makes propogates wxEVT_COMMAND_BUTTON_CLICKED events to the parent of this control
	wxCommandEvent newevent( wxEVT_COMMAND_BUTTON_CLICKED, GetId() );
	newevent.SetEventObject( this );
	GetEventHandler()->ProcessEvent( newevent );
}

/**
 * This function repaints the widget with a grayscale version of the currently
 * displayed picture, or repaints it with the original version depending on the
 * value of grayscale.
 * \param grayscale [bool] if true repaints the image with grayscale version, 
 * otherwise reverts to the original picture
 */
void wxDynamicBitmap::SetGrayScale(bool grayscale)
{
	m_grayscale = grayscale;
	
	//regenerate the grayscale bitmap;
	if (grayscale) {
		wxImage tempimage;
		tempimage = m_bitmap.ConvertToImage();
		//ConvertToGrayScale(tempimage);
		m_graybitmap = wxBitmap(tempimage.ConvertToGreyscale());
	}
	wxWindow::Refresh();
	wxWindow::Update();
}

void wxDynamicBitmap::ConvertToGrayScale(wxImage& image) const
{
	double red2Gray    = 0.297;
	double green2Gray = 0.589;
	double blue2Gray = 0.114;
	int w = image.GetWidth(), h = image.GetHeight();
	unsigned char *data = image.GetData();
	int x,y;
	for (y = 0; y < h; y++)
		for (x = 0; x < w; x++)
		{
			long pos = (y * w + x) * 3;
			char g = (char) (data[pos]*red2Gray +
				data[pos+1]*green2Gray +
				data[pos+2]*blue2Gray);
			data[pos] = data[pos+1] = data[pos+2] = g;
		}
}

