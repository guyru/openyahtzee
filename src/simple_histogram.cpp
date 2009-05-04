/***************************************************************************
 *   Copyright (C) 2009 by Guy Rutenberg   *
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

#include "simple_histogram.h"
#include <boost/foreach.hpp>
using namespace std;

SimpleHistogram::SimpleHistogram(wxWindow* parent, wxWindowID id,
				const wxPoint& pos, const wxSize& size,
				long style, const wxString& name)
				: wxPanel(parent, id, pos, size, style, name)
{
	Connect(this->GetId(), wxEVT_PAINT, wxPaintEventHandler(SimpleHistogram::OnPaint));
	Connect(this->GetId(), wxEVT_SIZE, wxSizeEventHandler(SimpleHistogram::OnResize));
	
}

void SimpleHistogram::OnPaint(wxPaintEvent& event)
{
	wxPaintDC pdc(this);
	wxGraphicsContext* dc = wxGraphicsContext::Create(pdc);
	
	dc->SetBrush(*wxBLUE_BRUSH);
	int width, height;
	GetClientSize(&width, &height);

	const double column_width = static_cast<double>(width) / data.size();

	int i = 0;
	double item_x, item_y, item_ratio, item_height;
	BOOST_FOREACH(double d, data) {
		item_x = i * column_width;
		item_ratio = data_total ? d/data_total : 0;
		item_height = item_ratio * height;
		item_y = height-item_height;
		dc->DrawRectangle(item_x, item_y, column_width, item_height);
		i++;
	}
}

void SimpleHistogram::OnResize(wxSizeEvent& event)
{
	Refresh();
	event.Skip();
}

void SimpleHistogram::SetData(vector<double> d)
{
	data.assign(d.begin(), d.end());
	data_total = 0;
	BOOST_FOREACH(double tmp, data) {
		data_total += tmp;
	}
}


/*
wxSize SimpleHistogram::DoGetBestSize() const
{
	
	return wxSize(m_bitmap.GetWidth(),m_bitmap.GetHeight());
}
*/
