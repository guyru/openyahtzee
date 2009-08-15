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

#include <wx/button.h>
#include <wx/sizer.h>

#include "statistics_dialog.h"

#include "icon32.xpm"

using namespace std;
using namespace statistics_dialog;

StatisticsDialog::StatisticsDialog(wxWindow* parent,statistics::Statistics* stats) :
	wxDialog(parent, wxID_ANY, wxT("Statistics"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
	m_stats = stats;

	SetIcon(wxIcon(icon32_xpm));

	CreateControls();
	LoadData();
	DoLayout();
	ConnectEventTable();
}

void StatisticsDialog::CreateControls()
{
	games_started = new wxStaticText(this, wxID_ANY, wxT(""));
	games_finished = new wxStaticText(this, wxID_ANY, wxT(""));
	histogram = new simple_pie_plot::SimplePiePlot(this, wxID_ANY);
	statistics_reset_date = new wxStaticText(this, wxID_ANY, wxT(""));
}

void StatisticsDialog::LoadData()
{
	wxString tmp;
	tmp = wxString::Format(wxT("Games started: %i"), m_stats->games_started());
	games_started->SetLabel(tmp);

	tmp = wxString::Format(wxT("Games finished: %i"), m_stats->games_finished());
	games_finished->SetLabel(tmp);

	wxDateTime reset_time(m_stats->last_reset());
	tmp = wxT("Last reset: ") + reset_time.Format();
	statistics_reset_date->SetLabel(tmp);

	vector<int> score_dist = m_stats->score_distribution();
	vector<double> dist (score_dist.begin(), score_dist.end());
	histogram->SetData(dist);
}

void StatisticsDialog::DoLayout()
{
	wxBoxSizer* top_sizer = new wxBoxSizer(wxVERTICAL);

	wxStaticBoxSizer* game_counts = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Game Counts") ), wxHORIZONTAL);
	game_counts->Add(games_started);
	game_counts->AddStretchSpacer(10);
	game_counts->Add(games_finished);
	top_sizer->Add(game_counts,0,wxEXPAND,10);

	wxStaticBoxSizer* score_distribution = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, wxT("Score Distribution") ), wxHORIZONTAL);
	score_distribution->Add(histogram, 1, wxEXPAND, 10);
	top_sizer->Add(score_distribution,10,wxEXPAND, 10);

	top_sizer->Add(statistics_reset_date,0,wxEXPAND,10);
	
	wxBoxSizer *button_sizer = new wxBoxSizer( wxHORIZONTAL );
	wxSizerFlags flags = wxSizerFlags().Border(wxALL & ~wxLEFT, 10);
	button_sizer->AddStretchSpacer();
	button_sizer->Add(new wxButton(this,wxID_CLEAR, wxT("Reset")),flags);
	button_sizer->Add( new wxButton(this,wxID_CLOSE), flags);
	top_sizer->Add(button_sizer,0,wxEXPAND);
	
	SetSizer(top_sizer);
	top_sizer->SetSizeHints(this);
}


void StatisticsDialog::ConnectEventTable()
{
	Connect(wxID_CLOSE, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(StatisticsDialog::OnClose));
	Connect(wxID_CLEAR, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(StatisticsDialog::OnReset));
}

void StatisticsDialog::OnClose(wxCommandEvent& event)
{
	Close();
}


void StatisticsDialog::OnReset(wxCommandEvent& event)
{
	int answer = wxMessageBox(
		wxT("Are you sure you want to clear the statistics data?\nThis action cannot be reversed."),
		wxT("Are You Sure?"),
		wxYES | wxNO,
		this);
	
	if (answer == wxYES) {
		m_stats->reset();
		LoadData();
	}
}
