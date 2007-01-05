// $Header$
/***************************************************************************
 *   Copyright (C) 2006 by Guy Rutenberg   *
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
#include <wx/dialog.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/listctrl.h>

#include "ObjectsID.h"
#include "HighScoreDialog.h"

#include "Icon.h"

using namespace std;

HighScoreDialog::HighScoreDialog(wxWindow* parent,wxWindowID id,HighScoreTableDB *highscoredb) :
	wxDialog(parent, id, wxT("High-Score Table"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE)
{

	SetIcon(wxIcon(ICON));


	list<string> table;

	table = highscoredb->GetHighScoreTable();	

	list<string>::iterator lip = table.begin();
	wxString buf;


	//Now create the dialog
	//Create The top-level sizer
	wxBoxSizer *topSizer = new wxBoxSizer( wxVERTICAL );
	wxListCtrl *highscorelist = new wxListCtrl(this,wxID_ANY,wxDefaultPosition,wxSize(400,400),wxLC_REPORT);
	
	topSizer->Add(
		highscorelist,
		0,
		wxALL,
		10);
		
	topSizer->Add(
		new wxButton(this,wxID_OK),
		0, //no streching
		wxALL, //we want border around everything
		10);
	
	///////////////////////////////////////
	////Enter content into highscoretable

	//create the columns
	wxListItem itemCol;
	itemCol.SetText(wxT("Place"));
	itemCol.SetImage(-1);
	highscorelist->InsertColumn(0, itemCol);
	itemCol.SetText(wxT("Name"));
	itemCol.SetImage(-1);
	highscorelist->InsertColumn(1, itemCol);
	itemCol.SetText(wxT("Date"));
	itemCol.SetImage(-1);
	highscorelist->InsertColumn(2, itemCol);
	itemCol.SetText(wxT("Score"));
	itemCol.SetImage(-1);
	highscorelist->InsertColumn(3, itemCol);
	
	int i =0;
	while(lip != table.end()) {
		if (i%4){
			buf = wxString(lip->c_str(),wxConvUTF8);
			highscorelist->SetItem(i/4,i%4,buf);
			lip++;

		} else { 
			buf.Clear();
			buf << (i/4+1);
			highscorelist->InsertItem(i/4,buf,-1);
		}
		i++;
	}
	

	//size the collumns
	highscorelist->SetColumnWidth(0, wxLIST_AUTOSIZE_USEHEADER );
	highscorelist->SetColumnWidth(1, wxLIST_AUTOSIZE );
	highscorelist->SetColumnWidth(2, wxLIST_AUTOSIZE );
	highscorelist->SetColumnWidth(3, wxLIST_AUTOSIZE_USEHEADER );

	

	SetSizer( topSizer ); // use the sizer for layout
	topSizer->Fit( this );          // fit the dialog to the contents
	topSizer->SetSizeHints( this ); // set hints to honor min size

}

///////////////////////
//The Info Dialog

HighScoreInfo::HighScoreInfo(wxWindow* parent,int place) :
	wxDialog(parent, wxID_ANY, wxT("High-Score Table"), wxDefaultPosition, wxDefaultSize)
{
	
	SetIcon(wxIcon(ICON));

	wxString msg;

	msg.Printf(wxT("Your score made it to the high-score table. Your place is number %i.\nPlease enter your name below and press OK."),place);
	//Now create the dialog
	//Create The top-level sizer
	wxBoxSizer *topSizer = new wxBoxSizer( wxVERTICAL );
	
	topSizer->Add(
		new wxStaticText(this, wxID_ANY, msg),
		0,
		wxALL,
		10);
	
	topSizer->Add(
		new wxTextCtrl(this, ID_INFODIALOGNAMEBOX,wxT(""),wxDefaultPosition,wxSize(300,-1)),
		0,
		wxALL,
		10);
			
	topSizer->Add(
		new wxButton(this,wxID_OK),
		0, //no streching
		wxALL, //we want border around everything
		10);

	

	SetSizer( topSizer ); // use the sizer for layout
	topSizer->Fit( this );          // fit the dialog to the contents
	topSizer->SetSizeHints( this ); // set hints to honor min size

}

wxString HighScoreInfo::GetName()
{
	return ((wxTextCtrl*) FindWindow(ID_INFODIALOGNAMEBOX)) -> GetValue();
}
