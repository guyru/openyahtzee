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
    wxDialog(parent, wxID_ANY, wxT("About " PACKAGE_NAME), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE)
{
	wxBitmap *logo = new wxBitmap; 
	*logo = wxBitmap(icon64_xpm);
	
	notebook_main = new wxNotebook(this, -1, wxDefaultPosition, wxDefaultSize, 0);
	
	notebook_main_pane_license = new wxScrolledWindow(notebook_main, -1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	
	notebook_main_pane_thanks = new wxScrolledWindow(notebook_main, -1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	
	notebook_main_pane_authors = new wxScrolledWindow(notebook_main, -1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	
	notebook_main_pane_about = new wxPanel(notebook_main, -1);
	
	bitmap_1 = new wxDynamicBitmap((wxWindow*)this, (wxWindowID)wxID_ANY,logo);
#ifdef PORTABLE
	app_label = new wxStaticText(this, -1, wxT("Open Yahtzee Portable Edition " VERSION));
#else	
	app_label = new wxStaticText(this, -1, wxT("Open Yahtzee " VERSION));
#endif
	label_desc = new wxStaticText(notebook_main_pane_about, -1, wxT("A full-featured wxWidgets version of\nthe classic dice game Yahtzee."));
	label_copyright = new wxStaticText(notebook_main_pane_about, -1, wxT("(C) 2006-2008 Guy Rutenberg"));
	//label_1 = new wxStaticText(notebook_main_pane_about, -1, wxT("http://openyahtzee.sourceforge.net/"));
	label_7 = new wxStaticText(notebook_main_pane_authors, -1, wxT("Please report bugs to openyahtzee-users@lists.sourceforge.net.\n\nGuy Rutenberg\n\tguyrutenberg@gmail.com\n\tAuthor, maintainer"));
	label_6 = new wxStaticText(notebook_main_pane_thanks, -1, wxT("Seamous McGill\n\tjohndoe@gmail.com\n\tLogo and dice design\n\nNeil Gierman\n\tngierman@roadrunn.com\n\tRPM packages"));
    label_15 = new wxStaticText(notebook_main_pane_license, -1, wxT("This program is free software; you can redistribute it and/or modify\nit under the terms of the GNU General Public License as published by \nthe Free Software Foundation; either version 2 of the License, or\n(at your option) any later version.\n\nThis program is distributed in the hope that it will be useful,\nbut WITHOUT ANY WARRANTY; without even the implied warranty of \nMERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\nGNU General Public License for more details. \n\nYou should have received a copy of the GNU General Public License \nalong with this program; if not, write to the\nFree Software Foundation, Inc.,\n59 Temple Place - Suite 330, Boston, MA  02111-1307, USA."));
    close_button = new wxButton(this, wxID_OK, wxT("Close"));

	set_properties();
	do_layout();
}


void AboutDialog::set_properties()
{
	SetSize(wxSize(400, 300));
	app_label->SetFont(wxFont(14, wxDEFAULT, wxNORMAL, wxBOLD, 0, wxT("")));
	notebook_main_pane_authors->SetScrollRate(10, 10);
	notebook_main_pane_thanks->SetScrollRate(10, 10);
	notebook_main_pane_license->SetScrollRate(10, 10);
}


void AboutDialog::do_layout()
{
    // begin wxGlade: AboutDialog::do_layout
    wxBoxSizer* sizer_1 = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer_6 = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer_5 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_4 = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer_3 = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer_2 = new wxBoxSizer(wxHORIZONTAL);
    sizer_2->Add(bitmap_1, 0, wxALL|wxALIGN_CENTER_VERTICAL, 10);
    sizer_2->Add(app_label, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxADJUST_MINSIZE, 10);
    sizer_1->Add(sizer_2, 0, 0, 0);
    sizer_3->Add(label_desc, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 10);
    sizer_3->Add(label_copyright, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 10);
    //sizer_3->Add(label_1, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 10);
    sizer_3->Add(new wxHyperlinkCtrl(notebook_main_pane_about,wxID_ANY,OY_URL,OY_URL),0,wxALL|wxALIGN_CENTER_HORIZONTAL,10);
    notebook_main_pane_about->SetSizer(sizer_3);
    sizer_4->Add(label_7, 0, wxALL, 10);
    notebook_main_pane_authors->SetSizer(sizer_4);
    sizer_5->Add(label_6, 0, wxALL, 10);
    notebook_main_pane_thanks->SetSizer(sizer_5);
    sizer_6->Add(label_15, 0, wxADJUST_MINSIZE, 0);
    notebook_main_pane_license->SetSizer(sizer_6);
    notebook_main->AddPage(notebook_main_pane_about, wxT("About"));
    notebook_main->AddPage(notebook_main_pane_authors, wxT("Authors"));
    notebook_main->AddPage(notebook_main_pane_thanks, wxT("Thanks To"));
    notebook_main->AddPage(notebook_main_pane_license, wxT("License Agreement"));
    sizer_1->Add(notebook_main, 1, wxEXPAND, 0);
    sizer_1->Add(close_button, 0, wxALL|wxALIGN_RIGHT, 10);
    SetSizer(sizer_1);
    Layout();
    // end wxGlade
}

