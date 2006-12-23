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

 /***********************************************
 *	This File contains the definitions	*
 *	of MainFrame's functions                *
 ***********************************************/

//#define DEBUG

#include <wx/wx.h>

#include "MainFrame.h"
#include "ObjectsID.h"
#include "HighScoreDialog.h"
#include "SettingsDialog.h"
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <wx/version.h>

//include the images for the dice
#include "one.xpm"
#include "two.xpm"
#include "three.xpm"
#include "four.xpm"
#include "five.xpm"
#include "six.xpm"

//include the icon file
#include "icon.xpm"

//default values
#define SPACE_SIZE 1
#define DEF_HIGHSCORESIZE 16

MainFrame::MainFrame(const wxString& title, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_FRAME_STYLE)
        : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, size, style)
{
	//give the frame an icon
	SetIcon(wxIcon(icon_xpm));
	std::ostringstream sstr;

	m_settingsdb = new SettingsDB(); //Get the settings database connection
	m_highscoredb = new HighScoreTableDB();


	if (m_settingsdb->GetKey("highscoresize") == "") { //check if we need to create a newdatabase
		m_highscoredb->SetSize(DEF_HIGHSCORESIZE);
		sstr<<DEF_HIGHSCORESIZE<<std::flush;
		m_settingsdb->SetKey("highscoresize", sstr.str());
	} else {
		int highscoresize = atoi((m_settingsdb->GetKey("highscoresize")).c_str());
		//m_highscoredb->SetSize((highscoresize>0)?highscoresize:DEF_HIGHSCORESIZE);
		m_highscoredb->SetSize(highscoresize);
	}

	bitmap_dices[0] = new wxBitmap(one_xpm);
	bitmap_dices[1] = new wxBitmap(two_xpm);
	bitmap_dices[2] = new wxBitmap(three_xpm);
	bitmap_dices[3] = new wxBitmap(four_xpm);
	bitmap_dices[4] = new wxBitmap(five_xpm);
	bitmap_dices[5] = new wxBitmap(six_xpm);
	
	//randomize the random-number generator based on the time
	srand( (unsigned)time( NULL ) );


	/*****Create and initialize the menu bar******/

	/*Create the menus*/
	wxMenu *gameMenu = new wxMenu;	//create File menu
	wxMenu *helpMenu = new wxMenu;	//create Help menu
	
	
	//insert menu items into menu Help
	helpMenu->Append(wxID_ABOUT, wxT("&About...\tF1"),
			wxT("Show about dialog"));

	//insert menu items into menu File
	gameMenu->Append(ID_NEWGAME,wxT("&New Game\tF2"),wxT("Start a new game"));
	//create the undo button and make it disabled
	gameMenu->Append(ID_UNDO,wxT("&Undo"),wxT("Undo the last move"));
	gameMenu->Append(ID_SHOWHIGHSCORE,wxT("High &Scores"),wxT("Show high-scores table"));
	gameMenu->Append(ID_SETTINGS,wxT("Settings"),wxT("Show settings dialog"));
	gameMenu->Append(wxID_EXIT, wxT("E&xit\tAlt-X"),
			wxT("Quit this program"));
	
	// Declare the menu-bar and append the freshly created menus to the menu bar...
	wxMenuBar *menuBar = new wxMenuBar();
	menuBar->Append(gameMenu, wxT("&Game"));
	menuBar->Append(helpMenu, wxT("&Help"));
	
	// ... and attach this menu bar to the frame
	SetMenuBar(menuBar);
	
	/***End menu-bar ***/
	wxPanel* panel = new wxPanel(this, ID_PANEL,
   		wxDefaultPosition, wxDefaultSize);

	wxBoxSizer *topSizer = new wxBoxSizer( wxHORIZONTAL );
	wxBoxSizer *sectionsSizer = new wxBoxSizer( wxVERTICAL );
	wxBoxSizer *diceSizer = new wxBoxSizer( wxVERTICAL );

	wxSizer *uppersection = new wxStaticBoxSizer( new wxStaticBox( panel, wxID_ANY, wxT("Upper Section") ), wxVERTICAL);
	wxSizer *lowersection = new wxStaticBoxSizer( new wxStaticBox( panel, wxID_ANY, wxT("Lower Section") ), wxVERTICAL);
	
	wxFlexGridSizer* uppergrid = new wxFlexGridSizer(9, 2, 0, 0);
	wxFlexGridSizer* lowergrid = new wxFlexGridSizer(9, 2, 0, 0);

	
	uppergrid->Add(new wxButton(panel,ID_ACES,wxT("Aces")),0,wxALL,SPACE_SIZE);
	uppergrid->Add(new wxTextCtrl(panel, ID_ACESTEXT),1,wxALL,SPACE_SIZE);
	uppergrid->Add(new wxButton(panel,ID_TWOS,wxT("Twos")),0,wxALL,SPACE_SIZE);
	uppergrid->Add(new wxTextCtrl(panel, ID_TWOSTEXT),1,wxALL,SPACE_SIZE);
	uppergrid->Add(new wxButton(panel,ID_THREES,wxT("Threes")),0,wxALL,SPACE_SIZE);
	uppergrid->Add(new wxTextCtrl(panel, ID_THREESTEXT),1,wxALL,SPACE_SIZE);
	uppergrid->Add(new wxButton(panel,ID_FOURS,wxT("Fours")),0,wxALL,SPACE_SIZE);
	uppergrid->Add(new wxTextCtrl(panel, ID_FOURSTEXT),1,wxALL,SPACE_SIZE);
	uppergrid->Add(new wxButton(panel,ID_FIVES,wxT("Fives")),0,wxALL,SPACE_SIZE);
	uppergrid->Add(new wxTextCtrl(panel, ID_FIVESTEXT),1,wxALL,SPACE_SIZE);
	uppergrid->Add(new wxButton(panel,ID_SIXES,wxT("Sixes")),0,wxALL,SPACE_SIZE);
	uppergrid->Add(new wxTextCtrl(panel, ID_SIXESTEXT),1,wxALL,SPACE_SIZE);
	uppergrid->Add(new wxStaticText(panel, wxID_ANY, wxT("Total score:")),0,wxALL,SPACE_SIZE);
	uppergrid->Add(new wxTextCtrl(panel, ID_UPPERSECTIONTOTAL),1,wxALL,SPACE_SIZE);
	uppergrid->Add(new wxStaticText(panel, wxID_ANY, wxT("Bonus:")),0,wxALL,SPACE_SIZE);
	uppergrid->Add(new wxTextCtrl(panel, ID_BONUS),1,wxALL,SPACE_SIZE);
	uppergrid->Add(new wxStaticText(panel, wxID_ANY, wxT("Total of upper section:")),0,wxALL,SPACE_SIZE);
	uppergrid->Add(new wxTextCtrl(panel, ID_UPPERTOTAL),1,wxALL,SPACE_SIZE);


	lowergrid->Add(new wxButton(panel,ID_THREEOFAKIND,wxT("3 of a kind")),0,wxALL,SPACE_SIZE);
	lowergrid->Add(new wxTextCtrl(panel, ID_THREEOFAKINDTEXT),1,wxALL,SPACE_SIZE);
	lowergrid->Add(new wxButton(panel,ID_FOUROFAKIND,wxT("4 of a kind")),0,wxALL,SPACE_SIZE);
	lowergrid->Add(new wxTextCtrl(panel, ID_FOUROFAKINDTEXT),1,wxALL,SPACE_SIZE);
	lowergrid->Add(new wxButton(panel,ID_FULLHOUSE,wxT("Full House")),0,wxALL,SPACE_SIZE);
	lowergrid->Add(new wxTextCtrl(panel, ID_FULLHOUSETEXT),1,wxALL,SPACE_SIZE);
	lowergrid->Add(new wxButton(panel,ID_SMALLSEQUENCE,wxT("Sequence of 4")),0,wxALL,SPACE_SIZE);
	lowergrid->Add(new wxTextCtrl(panel, ID_SMALLSEQUENCETEXT),1,wxALL,SPACE_SIZE);
	lowergrid->Add(new wxButton(panel,ID_LARGESEQUENCE,wxT("Sequence of 5")),0,wxALL,SPACE_SIZE);
	lowergrid->Add(new wxTextCtrl(panel, ID_LARGESEQUENCETEXT),1,wxALL,SPACE_SIZE);
	lowergrid->Add(new wxButton(panel,ID_YAHTZEE,wxT("Yahtzee")),0,wxALL,SPACE_SIZE);
	lowergrid->Add(new wxTextCtrl(panel, ID_YAHTZEETEXT),1,wxALL,SPACE_SIZE);
	lowergrid->Add(new wxButton(panel,ID_CHANCE,wxT("Chance")),0,wxALL,SPACE_SIZE);
	lowergrid->Add(new wxTextCtrl(panel, ID_CHANCETEXT),1,wxALL,SPACE_SIZE);
	lowergrid->Add(new wxStaticText(panel, wxID_ANY, wxT("Yahtzee Bonus")),0,wxALL,SPACE_SIZE);
	lowergrid->Add(new wxTextCtrl(panel, ID_YAHTZEEBONUSTEXT),1,wxALL,3);
	lowergrid->Add(new wxStaticText(panel, wxID_ANY, wxT("Total of lower section:")),0,wxALL,SPACE_SIZE);
	lowergrid->Add(new wxTextCtrl(panel, ID_LOWERTOTAL),1,wxALL,SPACE_SIZE);
	lowergrid->Add(new wxStaticText(panel, wxID_ANY, wxT("Grand Total:")),0,wxALL,SPACE_SIZE);
	lowergrid->Add(new wxTextCtrl(panel, ID_GRANDTOTAL),1,wxALL,SPACE_SIZE);

	uppersection->Add(uppergrid);
	lowersection->Add(lowergrid);
	sectionsSizer->Add(uppersection,0,wxALL,5);
	sectionsSizer->Add(lowersection,0,wxALL,5);

	diceSizer->Add(new wxStaticBitmap(panel,ID_DICE1,*bitmap_dices[0]),0,wxALL,3);
	diceSizer->Add(new wxCheckBox(panel, ID_DICE1KEEP, wxT("Keep")),0,wxALL,3);
	diceSizer->Add(new wxStaticBitmap(panel,ID_DICE2,*bitmap_dices[1]),0,wxALL,3);
	diceSizer->Add(new wxCheckBox(panel, ID_DICE2KEEP, wxT("Keep")),0,wxALL,3);
	diceSizer->Add(new wxStaticBitmap(panel,ID_DICE3,*bitmap_dices[2]),0,wxALL,3);
	diceSizer->Add(new wxCheckBox(panel, ID_DICE3KEEP, wxT("Keep")),0,wxALL,3);
	diceSizer->Add(new wxStaticBitmap(panel,ID_DICE4,*bitmap_dices[3]),0,wxALL,3);
	diceSizer->Add(new wxCheckBox(panel, ID_DICE4KEEP, wxT("Keep")),0,wxALL,3);
	diceSizer->Add(new wxStaticBitmap(panel,ID_DICE5,*bitmap_dices[4]),0,wxALL,3);
	diceSizer->Add(new wxCheckBox(panel, ID_DICE5KEEP, wxT("Keep")),0,wxALL,3);
	diceSizer->Add(new wxButton(panel, ID_ROLL, wxT("Roll")),0,wxALL,3);

	
	topSizer->Add(sectionsSizer);
	topSizer->Add(diceSizer);	

	panel->SetSizer(topSizer);
	
	topSizer->Fit(this);
	topSizer->SetSizeHints(this);
	
	//make the text boxes uneditable to the user	
	wxTextCtrl *textctrl;
	for(int i = ID_ACESTEXT;i<=ID_GRANDTOTAL;i++) {
		textctrl = (wxTextCtrl*) FindWindow(i);
		textctrl->SetEditable(false);
	}
	
	//disable the undo button
	(GetMenuBar()->FindItem(ID_UNDO))->Enable(false);


	

	/***************************************/
	/********* Declare Event Table *********/
	/***************************************/
	
	/***Connect Menu items***/
	Connect(wxID_EXIT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnQuit));
	Connect(wxID_ABOUT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnAbout));
	Connect(ID_NEWGAME, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnNewGame));
	Connect(ID_UNDO, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnUndo));
	Connect(ID_SHOWHIGHSCORE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnShowHighscore));
	Connect(ID_SETTINGS, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnSettings));
	Connect(ID_ROLL, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (MainFrame::OnRollButton));

	for (int i=ID_ACES; i<=ID_SIXES; i++)
		Connect(i, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (MainFrame::OnUpperButtons));
	Connect(ID_THREEOFAKIND, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (MainFrame::On3ofakindButton));
	Connect(ID_FOUROFAKIND, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (MainFrame::On4ofakindButton));
	Connect(ID_FULLHOUSE, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (MainFrame::OnFullHouseButton));
	Connect(ID_SMALLSEQUENCE, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (MainFrame::OnSmallSequenceButton));
	Connect(ID_LARGESEQUENCE, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (MainFrame::OnLargeSequenceButton));
	Connect(ID_YAHTZEE, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (MainFrame::OnYahtzeeButton));
	Connect(ID_CHANCE, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (MainFrame::OnChanceButton));
	
	/*** End of Event Table ***/

	ResetRolls();
	ClearDiceHash();
	m_yahtzee = false;
	m_numofplaysleft = 13;

 }

/*********EVENT PROCCESSING FUNCTIONS********/


void MainFrame::OnAbout(wxCommandEvent& event)
{
	wxString msg;
	wxString sqliteversion = wxString(sqlite3_version,wxConvUTF8);
	msg.Printf(wxT("OpenYahtzee 1.5.1\nCopyright (C) 2006 by Guy Rutenberg\n\nThis program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.\n\nOpenYahtzee was built against:\nwxWidgets %i.%i\n"),wxMAJOR_VERSION,wxMINOR_VERSION);
	msg += wxT("SQLite ") + sqliteversion;
		
	wxMessageBox(msg, wxT("About Yahtzee"), wxOK | wxICON_INFORMATION, this);
}

void MainFrame::OnQuit(wxCommandEvent& event)
{
	// Destroy the frame
	Close();
}

void MainFrame::OnNewGame(wxCommandEvent& event)
{

	ResetRolls();
	ClearDiceHash();
	m_yahtzee = false;
	m_numofplaysleft = 13;
	
	for (int i = ID_ACESTEXT; i<= ID_GRANDTOTAL; i++)
		((wxTextCtrl*) FindWindow(i))->Clear();
	for (int i = ID_ACES; i<= ID_CHANCE; i++)
		((wxButton*) FindWindow(i))->Enable(true);
}

///This function handles the undo events
void MainFrame::OnUndo(wxCommandEvent& event)
{
	m_rolls = m_rollsundo;

	//after the user scored the button was enabled, check if it should be disabled
	if (m_rolls <= 0) //we don't have remaining rolls 
		((wxButton*) FindWindow(ID_ROLL)) -> Enable(false);

	//restore the 'keep' checkboxes
	for (int i=0; i<5; i++)
		((wxCheckBox*) FindWindow(i + ID_DICE1KEEP)) -> Enable(true);
	
	//reset the users last choice
	FindWindow(m_lastmove)->Enable(true);

	//clear the score;
	((wxTextCtrl*)FindWindow(ID_ACESTEXT + (m_lastmove - ID_ACES)))->SetValue(wxT(""));

	(GetMenuBar()->FindItem(ID_UNDO))->Enable(false);
	//cancel the counting for the choice that was canceled
	m_numofplaysleft++;
}
///This function enables the undo button and stores the last move
inline void MainFrame::EnableUndo(int id)
{
		(GetMenuBar()->FindItem(ID_UNDO))->Enable(true);
		m_lastmove = id;
}

void MainFrame::OnShowHighscore(wxCommandEvent& event)
{
	HighScoreDialog *dialog = new HighScoreDialog(this,wxID_ANY,m_highscoredb);
	dialog->ShowModal();
}

void MainFrame::OnSettings( wxCommandEvent& event)
{
	SettingsDialog *dialog = new SettingsDialog(this,wxID_ANY);
	SettingsDialogData data;
	std::ostringstream sstr;
	
	data.highscoresize = m_highscoredb->GetSize();
	
	dialog->SetData(data);
	if(dialog->ShowModal()==wxID_OK) { //user saved Changes
		data = dialog->GetData();

		if(data.reset)
			m_highscoredb->SetSize(0);
		
		sstr<<data.highscoresize<<std::flush;
		m_settingsdb->SetKey("highscoresize",sstr.str());
				
		m_highscoredb->SetSize(data.highscoresize);
	}
}

void MainFrame::OnRollButton (wxCommandEvent& event)
{
	//roll the dices...
	for (int i=0; i<5; i++) {
		if (!((wxCheckBox*) FindWindow(i + ID_DICE1KEEP))->IsChecked()) {
			dice[i] = rand()%6;
			((wxStaticBitmap*) FindWindow(i + ID_DICE1)) -> SetBitmap(*bitmap_dices[dice[i]]);
		}
	}
	
	//Clear old dice-hash and create a new one
	ClearDiceHash();
	for (int i=0; i<5; i++)
		dicehash[dice[i]] += 1;	

	//if out of rolls disable the roll butoon
	m_rolls -= 1;
	#ifndef DEBUG
	if (m_rolls <= 0) 
		((wxButton*) FindWindow(ID_ROLL)) -> Enable(false);
	#endif
	
	//enable the keep checkboxes
	for (int i=0; i<5; i++)
		((wxCheckBox*) FindWindow(i + ID_DICE1KEEP)) -> Enable(true);
	
	//we rolled the dices so undoing isn't allowed
	(GetMenuBar()->FindItem(ID_UNDO))->Enable(false);

}

void MainFrame::OnUpperButtons (wxCommandEvent& event)
{
	wxString out;
	int temp;
	if(m_rolls < 3){
		YahtzeeBonus();
		temp = dicehash[(event.GetId() - ID_ACES)] * (event.GetId() - ID_ACES + 1);
	
		out.Printf(wxT("%i"),temp);
		((wxTextCtrl*) FindWindow(event.GetId() - ID_ACES + ID_ACESTEXT))->SetValue(out);
		
		//now after the scoring reset the rolls
		ResetRolls();
		//and disable the button
		FindWindow(event.GetId())->Enable(false);
		m_numofplaysleft--;
		EnableUndo(event.GetId());
		EndofGame();
	}
	else 
		wxMessageBox(wxT("First you need to roll, and after you roll you may score"), wxT("OpenYahtzee"), wxOK | wxICON_INFORMATION, this);
}

void MainFrame::On3ofakindButton(wxCommandEvent& event)
{
	if(m_rolls>=3) {
		wxMessageBox(wxT("First you need to roll, and after you roll you may score"), wxT("OpenYahtzee"), wxOK | wxICON_INFORMATION, this);
		return;
	}
	YahtzeeBonus();
	bool three = false;
	wxString out;
	int temp = 0;	

	//check for the conditions of scoring
	for (int i=0; i<6; i++)
		if (dicehash[i] >= 3)
			three = true;
	if (three){
		for(int i = 0; i<5; i++) 
			temp += dice[i]+1;
	
		out.Printf(wxT("%i"),temp);
		((wxTextCtrl*) FindWindow(ID_THREEOFAKINDTEXT))->SetValue(out);
	} else
		((wxTextCtrl*) FindWindow(ID_THREEOFAKINDTEXT))->SetValue(wxT("0"));
	
	//now after the scoring reset the rolls
	ResetRolls();
	//and disable the button
	FindWindow(event.GetId())->Enable(false);
	m_numofplaysleft--;
	EnableUndo(event.GetId());
	EndofGame();
}

void MainFrame::On4ofakindButton(wxCommandEvent& event)
{
	if(m_rolls>=3) {
		wxMessageBox(wxT("First you need to roll, and after you roll you may score"), wxT("OpenYahtzee"), wxOK | wxICON_INFORMATION, this);
		return;
	}
	YahtzeeBonus();
	bool four = false;
	wxString out;
	int temp = 0;	

	//check for the conditions of scoring
	for (int i=0; i<6; i++)
		if (dicehash[i] >= 4)
			four = true;
	if (four){
		for(int i = 0; i<5; i++) 
			temp += dice[i]+1;
	
		out.Printf(wxT("%i"),temp);
		((wxTextCtrl*) FindWindow(ID_FOUROFAKINDTEXT))->SetValue(out);
	} else
		((wxTextCtrl*) FindWindow(ID_FOUROFAKINDTEXT))->SetValue(wxT("0"));
	
	//now after the scoring reset the rolls
	ResetRolls();
	//and disable the button
	FindWindow(event.GetId())->Enable(false);
	m_numofplaysleft--;
	EnableUndo(event.GetId());
	EndofGame();
}

void MainFrame::OnFullHouseButton(wxCommandEvent& event)
{
	if(m_rolls>=3) {
		wxMessageBox(wxT("First you need to roll, and after you roll you may score"), wxT("OpenYahtzee"), wxOK | wxICON_INFORMATION, this);
		return;
	}
	YahtzeeBonus();
	bool two = false;
	bool three = false;

	//check for the conditions of scoring
	for (int i=0; i<6; i++)
		if (dicehash[i] == 2)
			two = true;
	for (int i=0; i<6; i++)
		if (dicehash[i] == 3)
			three = true;
	if (two && three)
		((wxTextCtrl*) FindWindow(ID_FULLHOUSETEXT))->SetValue(wxT("25"));
	else
		((wxTextCtrl*) FindWindow(ID_FULLHOUSETEXT))->SetValue(wxT("0"));
	
	//now after the scoring reset the rolls
	ResetRolls();
	//and disable the button
	FindWindow(event.GetId())->Enable(false);
	m_numofplaysleft--;
	EnableUndo(event.GetId());
	EndofGame();
}

void MainFrame::OnSmallSequenceButton(wxCommandEvent& event)
{
	if(m_rolls>=3) {
		wxMessageBox(wxT("First you need to roll, and after you roll you may score"), wxT("OpenYahtzee"), wxOK | wxICON_INFORMATION, this);
		return;
	}

	YahtzeeBonus();
	bool sequence = false;
	//check for the conditions of scoring
	if ( (dicehash[0]>=1 && dicehash[1]>=1 && dicehash[2]>=1 && dicehash[3]>=1) ||
		(dicehash[1]>=1 && dicehash[2]>=1 && dicehash[3]>=1 && dicehash[4]>=1) ||
		(dicehash[2]>=1 && dicehash[3]>=1 && dicehash[4]>=1 && dicehash[5]>=1))
			sequence = true;
	if (sequence)
		((wxTextCtrl*) FindWindow(ID_SMALLSEQUENCETEXT))->SetValue(wxT("30"));
	else
		((wxTextCtrl*) FindWindow(ID_SMALLSEQUENCETEXT))->SetValue(wxT("0"));
	
	//now after the scoring reset the rolls
	ResetRolls();
	//and disable the button
	FindWindow(event.GetId())->Enable(false);
	m_numofplaysleft--;
	EnableUndo(event.GetId());
	EndofGame();
}

void MainFrame::OnLargeSequenceButton(wxCommandEvent& event)
{
	if(m_rolls>=3) {
		wxMessageBox(wxT("First you need to roll, and after you roll you may score"), wxT("OpenYahtzee"), wxOK | wxICON_INFORMATION, this);
		return;
	}

	YahtzeeBonus();
	bool sequence = false;
	//check for the conditions of scoring
	if ( (dicehash[0]==1 && dicehash[1]==1 && dicehash[2]==1 && dicehash[3]==1 && dicehash[4]==1) ||
		(dicehash[1]==1 && dicehash[2]==1 && dicehash[3]==1 && dicehash[4]==1 && dicehash[5]==1))
			sequence = true;
	if (sequence)
		((wxTextCtrl*) FindWindow(ID_LARGESEQUENCETEXT))->SetValue(wxT("40"));
	else
		((wxTextCtrl*) FindWindow(ID_LARGESEQUENCETEXT))->SetValue(wxT("0"));
	
	//now after the scoring reset the rolls
	ResetRolls();
	//and disable the button
	FindWindow(event.GetId())->Enable(false);
	m_numofplaysleft--;
	EnableUndo(event.GetId());
	EndofGame();
}

void MainFrame::OnYahtzeeButton(wxCommandEvent& event)
{
	if(m_rolls>=3) {
		wxMessageBox(wxT("First you need to roll, and after you roll you may score"), wxT("OpenYahtzee"), wxOK | wxICON_INFORMATION, this);
		return;
	}
	//give the score
	if ((dice[0]==dice[1]) && (dice[1]==dice[2]) && (dice[1]==dice[3]) && (dice[1]==dice[4])){
		((wxTextCtrl*) FindWindow(ID_YAHTZEETEXT))->SetValue(wxT("50"));
		m_yahtzee=true;
	} else
		((wxTextCtrl*) FindWindow(ID_YAHTZEETEXT))->SetValue(wxT("0"));

	FindWindow(event.GetId())->Enable(false);
	m_numofplaysleft--;
	ResetRolls();
	EnableUndo(event.GetId());
	EndofGame();
}

void MainFrame::OnChanceButton (wxCommandEvent& event)
{
	wxString out;
	int temp = 0;
	if(m_rolls < 3){
		YahtzeeBonus();
		for(int i = 0; i<5; i++) 
			temp += dice[i]+1;
	
		out.Printf(wxT("%i"),temp);
		((wxTextCtrl*) FindWindow(ID_CHANCETEXT))->SetValue(out);
		
		//now after the scoring reset the rolls
		ResetRolls();
		//and disable the button
		FindWindow(event.GetId())->Enable(false);
		m_numofplaysleft--;
		EnableUndo(event.GetId());
		EndofGame();
	}
	else 
		wxMessageBox(wxT("First you need to roll, and after you roll you may score"), wxT("OpenYahtzee"), wxOK | wxICON_INFORMATION, this);

}

//********************************************
//******	General Functions	******
//********************************************

void MainFrame::ClearDiceHash()
{
	for (int i=0; i<6; i++)
		dicehash[i] = 0;
}

void MainFrame::ResetRolls()
{
	m_rollsundo = m_rolls;
	m_rolls = 3;
	((wxButton*) FindWindow(ID_ROLL)) -> Enable(true);
	for (int i=0; i<5; i++){ 
		((wxCheckBox*) FindWindow(i + ID_DICE1KEEP)) -> SetValue(false);
		((wxCheckBox*) FindWindow(i + ID_DICE1KEEP)) -> Enable(false);
	}
}

void MainFrame::YahtzeeBonus()
{
	long temp;
	wxString tempstr;
	
	//if the player didn't have any yathzees yet he can't have the bonus;
	if (!m_yahtzee)
		return;
	if ((dice[0]==dice[1]) && (dice[1]==dice[2]) && (dice[1]==dice[3]) && (dice[1]==dice[4])) {
		tempstr = ((wxTextCtrl*) FindWindow(ID_YAHTZEEBONUSTEXT)) -> GetValue();
		tempstr.ToLong(&temp,10);
		temp += 100;
		tempstr.Printf(wxT("%i"),temp);
		((wxTextCtrl*) FindWindow(ID_YAHTZEEBONUSTEXT)) -> SetValue(tempstr);

	}	
}

void MainFrame::EndofGame()
{
	if(m_numofplaysleft>0)
		return;
	
	wxString tempstr;
	long temp;
	long upperscore = 0;
	long lowerscore = 0;

	for (int i = ID_ACESTEXT; i<=ID_SIXESTEXT; i++){
		tempstr = ((wxTextCtrl*) FindWindow(i)) -> GetValue();
		tempstr.ToLong(&temp,10);
		upperscore +=temp;
	}
	
	tempstr.Printf(wxT("%i"),upperscore);
	((wxTextCtrl*) FindWindow(ID_UPPERSECTIONTOTAL)) -> SetValue(tempstr);
	
	//check for bonus
	if(upperscore>=63) {
		((wxTextCtrl*) FindWindow(ID_BONUS)) -> SetValue(wxT("35"));
		upperscore +=35;
	} else
		((wxTextCtrl*) FindWindow(ID_BONUS)) -> SetValue(wxT("0"));
	
	tempstr.Printf(wxT("%i"),upperscore);
	((wxTextCtrl*) FindWindow(ID_UPPERTOTAL)) -> SetValue(tempstr);
	
	//calculate total on lower section
	for (int i = ID_THREEOFAKINDTEXT; i<=ID_YAHTZEEBONUSTEXT; i++) {
		tempstr = ((wxTextCtrl*) FindWindow(i)) -> GetValue();
		tempstr.ToLong(&temp,10);
		lowerscore +=temp;
	}
	
	tempstr.Printf(wxT("%i"),lowerscore);
	((wxTextCtrl*) FindWindow(ID_LOWERTOTAL)) -> SetValue(tempstr);
	tempstr.Printf(wxT("%i"),upperscore + lowerscore);
	((wxTextCtrl*) FindWindow(ID_GRANDTOTAL)) -> SetValue(tempstr);

	//disable the roll button;
	((wxButton*) FindWindow(ID_ROLL)) -> Enable(false);

	tempstr.Printf(wxT("Your final score is %i points!"),lowerscore+upperscore);
	wxMessageBox(tempstr, wxT("Game Ended"), wxOK | wxICON_INFORMATION, this);

	//submit to high score
	HighScoreHandler(lowerscore+upperscore);
	
}

void MainFrame::HighScoreHandler(int score)
{
	int place;
	std::string name,date;
	wxCommandEvent newevent;


	place = m_highscoredb->IsHighScore(score);
	
	if(!place)  //if the score didn't make it to the highscore table do nothing
		return;
	HighScoreInfo *infodialog = new HighScoreInfo(this,place);
	infodialog->ShowModal();

	name = infodialog->GetName().mb_str();

	//get the date
	wxDateTime now = wxDateTime::Now();
	date = now.FormatISOTime().mb_str();
	date +=" ";
	date += now.FormatDate().mb_str();

	m_highscoredb->SendHighScore(name,date,score);

	//now show the high score table
	newevent.SetId(ID_SHOWHIGHSCORE);
	newevent.SetEventType(wxEVT_COMMAND_MENU_SELECTED);
	ProcessEvent(newevent);

}
