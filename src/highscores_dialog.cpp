/***************************************************************************
 *   Copyright (C) 2006-2008 by Guy Rutenberg   *
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

#include "highscores_dialog.h"

#include "Icon.h"

using namespace std;
using namespace highscores_dialog;

HighscoresDialog::HighscoresDialog(wxWindow* parent,configuration::Configuration* config, int highlight_rank) :
	wxDialog(parent, wxID_ANY, wxT("Highscores Table"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE)
{
	this->highlight_rank = highlight_rank;
	m_config = config;

	SetIcon(wxIcon(ICON));

	createControls();
	loadData();
	doLayout();
	connectEventTable();
}

void HighscoresDialog::createControls()
{
	highscoreslist = new wxListCtrl(this,wxID_ANY,wxDefaultPosition,wxSize(450,400),wxLC_REPORT | wxBORDER_SUNKEN );

	wxListItem itemCol;

	itemCol.SetText(wxT("Rank"));
	itemCol.SetImage(-1);
	highscoreslist->InsertColumn(0, itemCol);

	itemCol.SetText(wxT("Name"));
	itemCol.SetImage(-1);
	highscoreslist->InsertColumn(1, itemCol);

	itemCol.SetText(wxT("Score"));
	itemCol.SetImage(-1);
	highscoreslist->InsertColumn(2, itemCol);

	itemCol.SetText(wxT("Date"));
	itemCol.SetImage(-1);
	highscoreslist->InsertColumn(3, itemCol);

	highscoreslist->SetColumnWidth(0, 50 );
	highscoreslist->SetColumnWidth(1, 195 );
	highscoreslist->SetColumnWidth(2, 60 );
	highscoreslist->SetColumnWidth(3, 140 );
}

void HighscoresDialog::loadData()
{
	highscoreslist->DeleteAllItems();
	const configuration::HighscoresList *list = m_config->getHighscores();

	configuration::HighscoresList::const_iterator it;
	wxString buf;

	int i = 0;
	for (it = list->begin(); it!=list->end(); it++, i++) {
		// rank
		buf.Clear();
		buf<<(i+1);
		highscoreslist->InsertItem(i,buf,-1);

		// name
		buf = wxString::FromUTF8(it->name.c_str());
		highscoreslist->SetItem(i,1,buf);

		// score
		buf.Clear();
		buf<< it->score;
		highscoreslist->SetItem(i,2,buf);

		// date
		buf = wxString::FromUTF8(it->date.c_str());
		highscoreslist->SetItem(i,3,buf);
	}

	if (highlight_rank) {
		highscoreslist->SetItemTextColour(highlight_rank-1,*wxRED);
	}
}

void HighscoresDialog::doLayout()
{
	wxBoxSizer *top_sizer = new wxBoxSizer( wxVERTICAL );
	
	top_sizer->Add(highscoreslist, 1, wxALL, 10); 

	wxBoxSizer *button_sizer = new wxBoxSizer( wxHORIZONTAL );

	wxSizerFlags flags = wxSizerFlags().Border(wxALL & ~wxLEFT, 10);

	button_sizer->AddStretchSpacer();
	button_sizer->Add(new wxButton(this,wxID_CLEAR),flags);
	button_sizer->Add(new wxButton(this,ID_CONFIGURE,wxT("Configure")),flags);
	button_sizer->Add( new wxButton(this,wxID_CLOSE), flags);

	top_sizer->Add(button_sizer,0,wxEXPAND);
	
	SetSizer(top_sizer);
	top_sizer->SetSizeHints(this);
}


void HighscoresDialog::connectEventTable()
{
	Connect(wxID_CLOSE, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(HighscoresDialog::onClose));

	Connect(wxID_CLEAR, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(HighscoresDialog::onClear));
	Connect(ID_CONFIGURE, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(HighscoresDialog::onConfigure));
}
void HighscoresDialog::onClose(wxCommandEvent& event)
{
	Close();
}

void HighscoresDialog::onConfigure(wxCommandEvent& event)
{
	HighscoresSettingsDialog *dialog = new HighscoresSettingsDialog(this, m_config);
	
	if (dialog->ShowModal() == wxID_OK) {
		loadData();
	}

	delete dialog;
}

void HighscoresDialog::onClear(wxCommandEvent& event)
{
	int answer = wxMessageBox(
		wxT("Are you sure you want to clear the Highscores table?\nThis action cannot be reversed."),
		wxT("Are You Sure?"),
		wxYES | wxNO,
		this);
	
	if (answer == wxYES) {
		m_config->clearHighscores();
		loadData();
	}
}


HighscoresSettingsDialog::HighscoresSettingsDialog(wxWindow* parent, configuration::Configuration* config) :
	wxDialog(parent, wxID_ANY, wxT("Highscores Table Settings"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE)
{
	this->m_config = config;
	doLayout();

	Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(HighscoresSettingsDialog::onOk));
}

void HighscoresSettingsDialog::doLayout()
{
	wxBoxSizer* top_sizer = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* size_sizer = new wxBoxSizer(wxHORIZONTAL);

	size_sizer->Add(new wxStaticText(this, wxID_ANY, wxT("High score table size:")), wxSizerFlags().Border(wxALL,10));
	
	int size = atoi(m_config->get("highscore-list-size").c_str());
	spin_ctrl = new wxSpinCtrl(this, wxID_ANY);
	spin_ctrl->SetRange(10,200);
	spin_ctrl->SetValue(size);

	size_sizer->Add(spin_ctrl,wxSizerFlags().Border(wxALL & ~wxLEFT,10));

	top_sizer->Add(size_sizer);
	top_sizer->Add(CreateButtonSizer(wxOK|wxCANCEL), 1, wxBOTTOM, 10);

	SetSizer(top_sizer);
	top_sizer->SetSizeHints(this);
}

void HighscoresSettingsDialog::onOk(wxCommandEvent& event) {
	int size = spin_ctrl->GetValue();
	m_config->setHighscoresSize(size);
	event.Skip();
}
