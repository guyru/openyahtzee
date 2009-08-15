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

#include "simple_pie_plot.h"
#include <boost/foreach.hpp>
#include <memory>
#include <iostream>
#include <cmath>
using namespace std;
using namespace simple_pie_plot;

#define PI 3.14159265

/**
 * Draws a pie slice with origin in (\a x,\a y), radius (\a r) from
 * from \a start_angle to \a end_angle, where angles are in radians measured
 * from the x-axis. The figure is drawn using \a dc.
 */
void DrawPieSlice(double x, double y, double r, double start_angle,
		  double end_angle, wxGraphicsContext *dc)
{
	wxGraphicsPath path = dc->CreatePath();
	double x1 = x + r * cos(start_angle);
	double y1 = y + r * sin(start_angle);
	double x2 = x + r * cos(end_angle);
	double y2 = y + r * sin(end_angle);

	path.AddArc(x, y, r, start_angle, end_angle, true);
	path.CloseSubpath();

	path.MoveToPoint(x,y);
	path.AddLineToPoint(x1,y1);
	path.AddLineToPoint(x2,y2);
	path.CloseSubpath();

	dc->DrawPath(path);
}

SimplePiePlot::SimplePiePlot(wxWindow* parent, wxWindowID id,
				const wxPoint& pos, const wxSize& size,
				long style, const wxString& name)
				: wxPanel(parent, id, pos, size, style, name)
{
	Connect(this->GetId(), wxEVT_PAINT, wxPaintEventHandler(SimplePiePlot::OnPaint));
	Connect(this->GetId(), wxEVT_SIZE, wxSizeEventHandler(SimplePiePlot::OnResize));
	
}

void SimplePiePlot::OnPaint(wxPaintEvent& event)
{
	wxPaintDC pdc(this);
	auto_ptr<wxGraphicsContext> dc(wxGraphicsContext::Create(pdc));
	
	int width, height;
	GetClientSize(&width, &height);

	wxBrush color_brush;
	int i = 0;
	double item_x, item_y, item_ratio, item_height;

	// radius should be set so it fits exactly
	double radius = (width>height? height : width)/2.0;
	double start_angle = 0;
	double end_angle;

	BOOST_FOREACH(double d, m_data) {
		// create brush
		color_brush.SetColour(GetSegmentColor(i, false));
		dc->SetBrush(color_brush);

		item_ratio = m_data_total ? d/m_data_total : 0;
		end_angle = start_angle + 2*PI* (item_ratio);

		DrawPieSlice(width/2.0, height/2.0, radius, start_angle,
			     end_angle, dc.get());

		start_angle = end_angle;
		i++;
	}
}

void SimplePiePlot::OnResize(wxSizeEvent& event)
{
	Refresh();
	event.Skip();
}

void SimplePiePlot::SetData(vector<double> d)
{
	//m_data.assign(d.begin(), d.end());
	m_data_total = 0;
	BOOST_FOREACH(double tmp, d) {
		m_data_total += tmp;
		if (tmp!=0)
			m_data.push_back(tmp);
	}
}

wxColour SimplePiePlot::GetSegmentColor(int i, bool highlight)
{
	double hue_step = 1.0/(m_data.size());
	double hue = i * hue_step;
	double value = highlight ? 1.0 : 0.8;
	wxColour color;
	wxImage::RGBValue rgb;
	wxImage::HSVValue hsv(hue, 1, value);
	rgb = wxImage::HSVtoRGB(hsv);
	color.Set(rgb.red, rgb.green, rgb.blue);

	return color;
}
