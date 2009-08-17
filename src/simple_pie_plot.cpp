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
#include <algorithm>
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

	m_max_legend_width = 0;
	m_max_legend_height = 0;
	m_legend_width = 0;

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

	if (!m_max_legend_width || !m_max_legend_height) {
		CalculateLegendDimensions(dc.get());
		// This might have caused a new minimum size to occure, tell
		// parent about it by refitting. See also comment in
		// GetMinSize()
		if (this->GetParent() && this->GetParent()->GetSizer())
			this->GetParent()->GetSizer()->SetSizeHints(this->GetParent());
	}
	CalculatePiePlotLocation();
	

	dc->SetPen(*wxTRANSPARENT_PEN);
	for (int i = 1; i<m_angles.size(); i++) {
		// create brush
		color_brush.SetColour(GetSegmentColor(i-1));
		dc->SetBrush(color_brush);

		DrawPieSlice(m_pie_x, m_pie_y, m_radius, m_angles[i-1],
			     m_angles[i], dc.get());
	}

	DrawLegend(dc.get());
}

void SimplePiePlot::CalculatePiePlotLocation()
{
	double pie_width;
	int width, height;

	assert(m_legend_width);

	GetClientSize(&width, &height);
	// m_max_legend_height* for the color box and it's right padding
	pie_width = width - m_legend_width - m_legend_left_padding;
	m_pie_x = pie_width/2.0;
	m_pie_y = height/2.0;
	m_radius = min(pie_width,(double)height)/2.0;
}

void SimplePiePlot::OnResize(wxSizeEvent& event)
{
	Refresh();
	event.Skip();
}

void SimplePiePlot::SetData(vector<double> d, vector<wxString> labels)
{
	// number of values should match number of labels
	assert(labels.size() == d.size());

	m_data.clear();
	m_data_total = 0;
	BOOST_FOREACH(double tmp, d) {
		// we can't plot negative values
		assert(tmp >= 0);
		m_data_total += tmp;
		m_data.push_back(tmp);
	}
	m_labels.assign(labels.begin(), labels.end());
	CalculateAngles();

	// this will cause the legend size to be re-calculated
	m_max_legend_width = 0;
	m_max_legend_height = 0;
}

void SimplePiePlot::CalculateAngles()
{
	double new_angle = 0;

	m_angles.clear();
	m_angles.push_back(new_angle);
	BOOST_FOREACH(double tmp, m_data) {
		new_angle = new_angle + 2 * PI * (tmp/m_data_total);
		m_angles.push_back(new_angle);
	}
}

void SimplePiePlot::CalculateLegendDimensions(wxGraphicsContext *dc)
{
	double tmp_height;
	double tmp_width;
	m_max_legend_width = 0;
	m_max_legend_height = 0;

	dc->SetFont(*wxNORMAL_FONT, *wxBLACK);
	
	BOOST_FOREACH(wxString label, m_labels) {
		dc->GetTextExtent(label, &tmp_width, &tmp_height, NULL, NULL);
		m_max_legend_width = max(m_max_legend_width, tmp_width);
		m_max_legend_height = max(m_max_legend_height, tmp_height);
	}

	m_legend_width = m_max_legend_width + 2*m_max_legend_height;
	m_legend_line_height = 1.5 * m_max_legend_height;
}

void SimplePiePlot::DrawLegend(wxGraphicsContext *dc)
{
	wxBrush color_brush;
	int width, height;
	GetClientSize(&width, &height);

	assert(m_legend_width);
	double legend_x = width - m_legend_width;

	dc->SetPen(*wxTRANSPARENT_PEN);
	dc->SetFont(*wxNORMAL_FONT, *wxBLACK);

	for (int i = 0; i<m_labels.size(); i++) {
		color_brush.SetColour(GetSegmentColor(i));
		dc->SetBrush(color_brush);

		dc->DrawRectangle(legend_x, i * m_legend_line_height,
			m_max_legend_height, m_max_legend_height);
		dc->DrawText(m_labels[i], legend_x + 2*m_max_legend_height,
			i * m_legend_line_height);
	}

}

wxColour SimplePiePlot::GetSegmentColor(int i)
{
	double hue_step = 1.0/(m_data.size());
	double hue = i * hue_step;
	double value = m_highlight == i ? 1.0 : 0.9;
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

	const double dist_mouse = (m_pie_x-event.m_x)*(m_pie_x-event.m_x) + 
			    (m_pie_y-event.m_y)*(m_pie_y-event.m_y);
	if (dist_mouse <= (m_radius*m_radius)) { 
		//angles are clockwise from the x-axis
		double angle = acos((event.m_x-m_pie_x)/sqrt(dist_mouse));
		if (event.m_y < m_pie_y)
			angle = 2*PI - angle;
		int i;
		for (i = 1; i<m_angles.size(); i++) {
			if (angle<=m_angles[i])
				break;
		}
		Highlight(i-1);
		return;
	} else if (event.m_x >= width-m_legend_width){
		int i = (int) (event.m_y / m_legend_line_height);
		if (i<m_labels.size()){
			Highlight(i);
			return;
		}
	}

	ClearHighlight();
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

wxSize SimplePiePlot::GetMinSize() const
{
	// In the first call to GetMinSize, m_legend_line_height and
	// m_legend_width may not be populated yet (set to zero), this
	// is a result of not being able to call CalculateLegendDimensions()
	// at this point due to constness. In order to workaround this issue,
	// if CalculateLegendDimensions() gets called in OnPaint() (this
	// happens also after setting new data), OnPaint() also notifies the
	// parent's sizer (is such exist) to update the size hints
	// accordingly.
	const double total_legend_height = m_labels.size() * m_legend_line_height;
	const double total_width = total_legend_height + m_legend_width +
		      		   m_legend_left_padding;
	return wxSize((int)ceil(total_width),(int)ceil(total_legend_height));
}

