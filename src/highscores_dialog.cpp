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
	highscoreslist = new wxListCtrl(this,wxID_ANY,wxDefaultPosition,wxSize(400,400),wxLC_REPORT | wxBORDER_SUNKEN );

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
}

void HighscoresDialog::loadData()
{
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
		buf = wxString(it->name.c_str(),wxConvUTF8);
		highscoreslist->SetItem(i,1,buf);

		// score
		buf.Clear();
		buf<< it->score;
		highscoreslist->SetItem(i,2,buf);

		// date
		buf = wxString(it->date.c_str(),wxConvUTF8);
		highscoreslist->SetItem(i,3,buf);
	}

	if (highlight_rank) {
		highscoreslist->SetItemTextColour(highlight_rank-1,*wxRED);
	}

	highscoreslist->SetColumnWidth(0, wxLIST_AUTOSIZE_USEHEADER );
	highscoreslist->SetColumnWidth(1, wxLIST_AUTOSIZE );
	highscoreslist->SetColumnWidth(2, wxLIST_AUTOSIZE_USEHEADER );
	highscoreslist->SetColumnWidth(3, wxLIST_AUTOSIZE );
}

void HighscoresDialog::doLayout()
{
	wxBoxSizer *top_sizer = new wxBoxSizer( wxVERTICAL );
	
	top_sizer->Add(highscoreslist, 1, wxALL, 10); 

	top_sizer->Add(
		new wxButton(this,wxID_CLOSE),
		0, //no streching
		wxALL, //we want border around everything
		10);
	
	SetAutoLayout(true);
	SetSizer(top_sizer);

	top_sizer->Fit(this);
	top_sizer->SetSizeHints(this);

	Layout();
}


void HighscoresDialog::connectEventTable()
{
	Connect(wxID_CLOSE, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(HighscoresDialog::onClose));
}

void HighscoresDialog::onClose(wxCommandEvent& event)
{
	Close();
}

