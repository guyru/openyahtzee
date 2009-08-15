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
#include <cmath>
using namespace std;
using namespace simple_pie_plot;

const double PI = 4.0 * atan(1.0);

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
	m_highlight = -1;
	Connect(this->GetId(), wxEVT_PAINT, wxPaintEventHandler(SimplePiePlot::OnPaint));
	Connect(this->GetId(), wxEVT_SIZE, wxSizeEventHandler(SimplePiePlot::OnResize));

	Connect(this->GetId(), wxEVT_MOTION, wxMouseEventHandler(SimplePiePlot::OnMouseMove));
	Connect(this->GetId(), wxEVT_LEAVE_WINDOW, wxMouseEventHandler(SimplePiePlot::OnMouseLeaveWindow));
}

void SimplePiePlot::OnPaint(wxPaintEvent& event)
{
	wxPaintDC pdc(this);
	auto_ptr<wxGraphicsContext> dc(wxGraphicsContext::Create(pdc));
	wxBrush color_brush;
	int width, height;

	GetClientSize(&width, &height);

	// radius should be set so it fits exactly
	double radius = (width>height? height : width)/2.0;

	for (int i = 1; i<m_angles.size(); i++) {
		// create brush
		color_brush.SetColour(GetSegmentColor(i-1));
		dc->SetBrush(color_brush);

		DrawPieSlice(width/2.0, height/2.0, radius, m_angles[i-1],
			     m_angles[i], dc.get());
	}
}

void SimplePiePlot::OnResize(wxSizeEvent& event)
{
	Refresh();
	event.Skip();
}

void SimplePiePlot::SetData(vector<double> d)
{
	m_data.clear();
	m_data_total = 0;
	BOOST_FOREACH(double tmp, d) {
		m_data_total += tmp;
		if (tmp!=0)
			m_data.push_back(tmp);
	}

	m_angles.clear();
	double new_angle = 0;
	m_angles.push_back(new_angle);
	BOOST_FOREACH(double tmp, m_data) {
		new_angle = new_angle + 2 * PI * (tmp/m_data_total);
		m_angles.push_back(new_angle);
	}
}

wxColour SimplePiePlot::GetSegmentColor(int i)
{
	double hue_step = 1.0/(m_data.size());
	double hue = i * hue_step;
	double value = m_highlight == i ? 1.0 : 0.8;
	wxColour color;
	wxImage::RGBValue rgb;
	wxImage::HSVValue hsv(hue, 1, value);
	rgb = wxImage::HSVtoRGB(hsv);
	color.Set(rgb.red, rgb.green, rgb.blue);

	return color;
}

void SimplePiePlot::OnMouseMove(wxMouseEvent& event)
{
	//check if the move is even in the plot
	int width, height;
	GetClientSize(&width, &height);
	// radius should be set so it fits exactly
	const double radius = (width>height? height : width)/2.0;

	const double dist_mouse = (width/2.0-event.m_x)*(width/2.0-event.m_x) + 
			    (height/2.0-event.m_y)*(height/2.0-event.m_y);
	if (dist_mouse > (radius*radius)) { 
		ClearHighlight();
		return;
	}

	//angles are clockwise from the x-axis
	double angle = acos((event.m_x-width/2.0)/sqrt(dist_mouse));
	if (event.m_y < height/2.0)
		angle = 2*PI - angle;
	int i;
	for (i = 1; i<m_angles.size(); i++) {
		if (angle<=m_angles[i])
			break;
	}
	Highlight(i-1);
}

void SimplePiePlot::OnMouseLeaveWindow(wxMouseEvent& event)
{
	ClearHighlight();
}

void SimplePiePlot::Highlight(int i)
{
	m_highlight = i;
	Refresh();
}

void SimplePiePlot::ClearHighlight()
{
	if (m_highlight != -1) {
		m_highlight = -1;
		Refresh();
	}
}
