// $Header: $
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

#include "about.h"
#include <wx/hyperlink.h>
#include "icon64.xpm"
#include "../config.h"

using namespace about;
AboutDialog::AboutDialog(wxWindow* parent):
    wxDialog(parent, wxID_ANY, wxT("About ") wxT(PACKAGE_NAME), wxDefaultPosition, wxSize(-1,300), wxDEFAULT_DIALOG_STYLE)
{
	addControlsAndLayout();
	SetEscapeId(wxID_CLOSE);
}

void AboutDialog::addControlsAndLayout()
{
	wxBoxSizer* title_sizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticBitmap* logo = new wxStaticBitmap(this, wxID_ANY,wxBitmap(icon64_xpm));
	title_sizer->Add(logo, 0, wxALL|wxALIGN_CENTER_VERTICAL, 10);

	wxStaticText* app_label = new wxStaticText(this,wxID_ANY,wxT("Open Yahtzee ") wxT(VERSION));
	app_label->SetFont(wxFont(14, wxDEFAULT, wxNORMAL, wxBOLD, false));
	title_sizer->Add(app_label, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxADJUST_MINSIZE, 10);

	wxNotebook* notebook_main = new wxNotebook(this, wxID_ANY);
	notebookAboutTab(notebook_main);
	notebookAuthorTab(notebook_main);
	notebookThanksTab(notebook_main);
	notebookLicenseTab(notebook_main);

	wxBoxSizer* top_sizer = new wxBoxSizer(wxVERTICAL);
    	top_sizer->Add(title_sizer);
    	top_sizer->Add(notebook_main, 1, wxEXPAND, 0);
    	top_sizer->Add(new wxButton(this,wxID_CLOSE), 0, wxALL|wxALIGN_RIGHT, 10);

	SetSizer(top_sizer);
	Layout();
}

void AboutDialog::notebookAboutTab(wxNotebook *notebook)
{
	wxPanel* panel = new wxPanel(notebook, wxID_ANY);
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	wxStaticText* label_desc = new wxStaticText(panel, wxID_ANY, wxT("A full-featured wxWidgets version of\nthe classic dice game Yahtzee."));
	sizer->Add(label_desc, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 10);

	wxStaticText* label_copyright = new wxStaticText(panel, wxID_ANY, wxT("\xA9 2006-2008 Guy Rutenberg"));
	sizer->Add(label_copyright, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 10);

	sizer->Add(new wxHyperlinkCtrl(panel,wxID_ANY,OY_URL,OY_URL),0,wxALL|wxALIGN_CENTER_HORIZONTAL,10);

	panel->SetSizer(sizer);

	notebook->AddPage(panel, wxT("About"));
}

void AboutDialog::notebookAuthorTab(wxNotebook *notebook)
{
	wxTextCtrl* text = new wxTextCtrl(notebook, wxID_ANY,wxT(""),wxDefaultPosition,wxDefaultSize,wxTE_MULTILINE|wxTE_READONLY);
	*text << wxT("Guy Rutenberg <guyrutenberg@gmail.com>\n");

	notebook->AddPage(text, wxT("Author"));
}

void AboutDialog::notebookThanksTab(wxNotebook *notebook)
{
	wxTextCtrl* text = new wxTextCtrl(notebook, wxID_ANY,wxT(""),wxDefaultPosition,wxDefaultSize,wxTE_MULTILINE|wxTE_READONLY);
	*text << wxT("Seamous McGill <johndoe@ggmail.com>\n");
	*text << wxT("    Logo and dice design\n\n");

	*text << wxT("Neil Gierman <ngierman@roadrunn.com>\n");
	*text << wxT("    Help with RPM packages\n\n");

	notebook->AddPage(text, wxT("Thanks To"));
}

void AboutDialog::notebookLicenseTab(wxNotebook *notebook)
{
	wxTextCtrl* text = new wxTextCtrl(notebook, wxID_ANY,wxT(""),wxDefaultPosition,wxDefaultSize,wxTE_MULTILINE|wxTE_READONLY);
	*text << wxT("This program is free software; you can redistribute ")
		wxT("it and/or modify it under the terms of the GNU General ")
		wxT("Public License as published by the Free Software ")
		wxT("Foundation; either version 2 of the License, or (at your ")
		wxT("option) any later version.\n\n")

		wxT("This program is distributed in the hope that it will be ")
		wxT("useful, but WITHOUT ANY WARRANTY; without even the ")
		wxT("implied warranty of MERCHANTABILITY or FITNESS FOR A ")
		wxT("PARTICULAR PURPOSE.  See the GNU General Public License ")
		wxT("for more details. \n\n")

		wxT("You should have received a copy of the GNU General ")
		wxT("Public License along with this program; if not, write to ")
		wxT("the\n")
		wxT("Free Software Foundation, Inc.,\n")
		wxT("59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.");

	notebook->AddPage(text, wxT("License Agreement"));
}
