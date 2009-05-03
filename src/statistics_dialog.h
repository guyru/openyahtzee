/***************************************************************************
 *   Copyright (C) 2006-2009 by Guy Rutenberg   *
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
#ifndef OPENYAHTZEE_STATISTICS_DIALOG_INC
#define OPENYAHTZEE_STATISTICS_DIALOG_INC

#include "statistics.h"
#include <wx/wx.h>
#include <wx/dialog.h>
#include <wx/listctrl.h>
#include <wx/spinctrl.h>

namespace statistics_dialog {

class StatisticsDialog : public wxDialog
{
public:
	StatisticsDialog(wxWindow* parent, statistics::Statistics* stats);

	void OnClose(wxCommandEvent& event);
	void OnReset(wxCommandEvent& event);
	void OnConfigure(wxCommandEvent& event);
private:
	void CreateControls();
	void LoadData( );
	void DoLayout();
	void ConnectEventTable();

	wxStaticText *games_started;
	wxStaticText *games_finished;
	wxStaticText *statistics_reset_date;

	statistics::Statistics* m_stats;
};

} // namespace statistics_dialog


#endif
