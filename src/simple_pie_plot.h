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

#include <wx/wx.h>
#include <vector>

#ifndef SIMPLE_HISTOGRAM_INC
#define SIMPLE_HISTOGRAM_INC

namespace simple_pie_plot {

class SimplePiePlot : public wxPanel {
public:
	SimplePiePlot (wxWindow* parent, wxWindowID id,
		 const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
		 long style = wxNO_BORDER, const wxString& name = wxPanelNameStr);
	void SetData(std::vector<double> d, std::vector<wxString> labels);
	void OnPaint(wxPaintEvent& event);
	void OnResize(wxSizeEvent& event);
	void OnMouseLeaveWindow(wxMouseEvent& event);
	void OnMouseMove(wxMouseEvent& event);
	wxSize GetMinSize() const;
private:
	/**
	 * Gets the color suiting the \a i th segment.
	 * \param i The segment number.
	 */
	wxColour GetSegmentColor(int i);

	/**
	 * Calculate the angles in the plot and populate the m_angles array.
	 */
	void CalculateAngles();

	/**
	 * Calculate the maximum widht and height for a label in the legend.
	 */
	void CalculateLegendDimensions(wxGraphicsContext *dc);

	/**
	 * Calculate the location and radius of the pie plot.
	 */
	void CalculatePiePlotLocation();

	void DrawLegend(wxGraphicsContext *dc);

	void Highlight(int i);
	void ClearHighlight();

	std::vector<double> m_data;
	std::vector<wxString> m_labels;
	std::vector<double> m_angles;
	double m_data_total;
	int m_highlight;

	/// maximum width for a label in the legend.
	double m_max_legend_width;
	/// maximum height for a label in the legend.
	double m_max_legend_height;
	/// total width of the legend
	double m_legend_width;
	/// the line height of each legend entry including padding
	double m_legend_line_height;

	static const double m_legend_left_padding = 10;

	// The center point of the pie plot
	double m_pie_x;
	double m_pie_y;
	double m_radius;

};

} // namespace

#endif
