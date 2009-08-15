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
	void SetData(std::vector<double> d);
	void OnPaint(wxPaintEvent& event);
	void OnResize(wxSizeEvent& event);
	void OnMouseLeaveWindow(wxMouseEvent& event);
	void OnMouseMove(wxMouseEvent& event);
private:
	/**
	 * Gets the color suiting the \a i th segment.
	 * \param i The segment number.
	 */
	wxColour GetSegmentColor(int i);

	void Highlight(int i);
	void ClearHighlight();

	std::vector<double> m_data;
	std::vector<double> m_angles;
	double m_data_total;
	int m_highlight;
};

} // namespace

#endif
