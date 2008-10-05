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

 /***********************************************
 *	This File contains the definitions	*
 *	of MainFrame's functions                *
 ***********************************************/

//#define DEBUG

#include <wx/wx.h>

#include "MainFrame.h"
#include "wxDynamicBitmap.h"
#include "ObjectsID.h"
#include "highscores_dialog.h"
#include "About.h"
#include "configuration.h"
#include "settings_dialog.h"
#include "../config.h"
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <wx/version.h>
#include <wx/filename.h>

 //include the images for the dice 	 
 namespace dice {
 #include "one.xpm"
 #include "two.xpm"
 #include "three.xpm"
 #include "four.xpm"
 #include "five.xpm"
 #include "six.xpm"
 } // namespace dice

//include the icon file
#include "Icon.h"

//default values - design
#define SPACE_SIZE 1
#define DICE_SPACE 3
#ifdef WIN32
	#define KEEP_SPACE 13
	#define VERTICAL_ROLL_SIZEX 64
	#define VERTICAL_ROLL_SIZEY 53
#else
	#define KEEP_SPACE 5
	#define VERTICAL_ROLL_SIZEX 64
	#define VERTICAL_ROLL_SIZEY 64
#endif
#define VER_DICE_SPACER 10

const wxEventType wxEVT_ENABLE_ROLL = wxNewEventType();

MainFrame::MainFrame(const wxString& title, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_FRAME_STYLE)
        : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, size, style)
{
	//give the frame an icon
	SetIcon(wxIcon(ICON));

	/* Check if there is configuration file under old-name and
	 * rename it
	 */
	wxString home_path = wxFileName::GetHomeDir();
	if (wxFileExists(home_path+wxT("/.OpenYahtzee"))) {
		// we don't won't to overwrite newer files.
		wxRenameFile(home_path+wxT("/.OpenYahtzee"),home_path+wxT("/.openyahtzee"),false);
	}

	
	std::string config_file;
	config_file = (home_path.utf8_str());
	#ifndef PORTABLE
		config_file += "/.openyahtzee.new";
	#else
		config_file += "/openyahtzee.dat";
	#endif

	m_config = new configuration::Configuration(config_file);

	m_evt_handler = new MainFrameEvtHandler(this);
	
	bitmap_dice[0] = new wxBitmap(dice::one_xpm);
	bitmap_dice[1] = new wxBitmap(dice::two_xpm);
	bitmap_dice[2] = new wxBitmap(dice::three_xpm);
	bitmap_dice[3] = new wxBitmap(dice::four_xpm);
	bitmap_dice[4] = new wxBitmap(dice::five_xpm);
	bitmap_dice[5] = new wxBitmap(dice::six_xpm);
	
	//randomize the random-number generator based on the time
	srand( (unsigned)time( NULL ) );
	
	AddMenus();

	AddControlsAndLayout();
	
	//make the text boxes uneditable to the user	
	wxTextCtrl *textctrl;
	for(int i = ID_ACESTEXT;i<=ID_GRANDTOTAL;i++) {
		textctrl = (wxTextCtrl*) FindWindow(i);
		textctrl->SetEditable(false);
	}
	
	//disable the undo button
	(GetMenuBar()->FindItem(wxID_UNDO))->Enable(false);

	ConnectEventTable();
	
	ResetRolls();
	m_yahtzee = false;
	m_yahtzeebonus = false;
	m_numofplaysleft = 13;

}

/**
 * Adds the menus and menu items to the MainFrame.
 */
void MainFrame::AddMenus()
{
	/*Create the menus*/
	wxMenu *gameMenu = new wxMenu;	//create Game menu
	wxMenu *helpMenu = new wxMenu;	//create Help menu
	
	
	//insert menu items into menu Help
	helpMenu->Append(ID_CHECK_FOR_UPDATES, wxT("&Check for Updates"));
	helpMenu->AppendSeparator();
	helpMenu->Append(ID_SENDCOMMENT, wxT("&Send a Comment to Developers"));
	helpMenu->AppendSeparator();
	helpMenu->Append(wxID_ABOUT, wxT("&About...\tF1"));

	//insert menu items into menu Game
	gameMenu->Append(wxID_NEW,wxT("&New Game\tF2"));
	gameMenu->Append(wxID_UNDO,wxT("&Undo\tCtrl+Z"));
	gameMenu->Append(ID_SHOWHIGHSCORE,wxT("Show Highscores\tCtrl+H"));
	gameMenu->Append(ID_SETTINGS,wxT("Settings"));
	gameMenu->Append(wxID_EXIT);
	
	// Declare the menu-bar and append the freshly created menus to the menu bar...
	wxMenuBar *menuBar = new wxMenuBar();
	menuBar->Append(gameMenu, wxT("&Game"));
	menuBar->Append(helpMenu, wxT("&Help"));
	
	// ... and attach this menu bar to the frame
	SetMenuBar(menuBar);
}

/**
 * Add the controls to the frame and use sizers to lay them out.
 */
void MainFrame::AddControlsAndLayout()
{
	wxPanel* panel = new wxPanel(this, ID_PANEL,
   		wxDefaultPosition, wxDefaultSize);

	wxBoxSizer *topSizer;
	wxFlexGridSizer *diceSizer;

	if (m_config->get("horizontal-layout")=="True") {
		topSizer = new wxBoxSizer( wxVERTICAL );
		sectionsSizer = new wxBoxSizer( wxHORIZONTAL );
		diceSizer = new wxFlexGridSizer(2, 0, 0, 0);
	} else {
		topSizer = new wxBoxSizer( wxHORIZONTAL );
		sectionsSizer = new wxBoxSizer( wxVERTICAL );
		diceSizer = new wxFlexGridSizer(1, 0, 0);;
	}

	uppersection = new wxStaticBoxSizer( new wxStaticBox( 
		panel, wxID_ANY, wxT("Upper Section") ), wxVERTICAL);
	
	lowersection = new wxStaticBoxSizer(new wxStaticBox(
		panel, wxID_ANY, wxT("Lower Section") ), wxVERTICAL);
	
	wxFlexGridSizer* uppergrid = new wxFlexGridSizer(2, 0, 10);
	wxFlexGridSizer* lowergrid = new wxFlexGridSizer(2, 0, 10);

	//BEGIN layout for the upper section of the score board
	uppergrid->Add(new wxButton(panel,ID_ACES,wxT("Aces")),
		0, wxALL, SPACE_SIZE);
	uppergrid->Add(new wxTextCtrl(panel, ID_ACESTEXT),1,wxALL,SPACE_SIZE);
	uppergrid->Add(new wxButton(panel,ID_TWOS,wxT("Twos")),
		0, wxALL, SPACE_SIZE);
	uppergrid->Add(new wxTextCtrl(panel, ID_TWOSTEXT),1,wxALL,SPACE_SIZE);
	uppergrid->Add(new wxButton(panel,ID_THREES,wxT("Threes")),
		0, wxALL, SPACE_SIZE);
	uppergrid->Add(new wxTextCtrl(panel, ID_THREESTEXT),1,wxALL,SPACE_SIZE);
	uppergrid->Add(new wxButton(panel,ID_FOURS,wxT("Fours")),
		0, wxALL, SPACE_SIZE);
	uppergrid->Add(new wxTextCtrl(panel, ID_FOURSTEXT),1,wxALL,SPACE_SIZE);
	uppergrid->Add(new wxButton(panel,ID_FIVES,wxT("Fives")),
		0, wxALL, SPACE_SIZE);
	uppergrid->Add(new wxTextCtrl(panel, ID_FIVESTEXT),1,wxALL,SPACE_SIZE);
	uppergrid->Add(new wxButton(panel,ID_SIXES,wxT("Sixes")),
		0, wxALL, SPACE_SIZE);
	uppergrid->Add(new wxTextCtrl(panel, ID_SIXESTEXT),1,wxALL,SPACE_SIZE);
	uppergrid->Add(new wxStaticText(panel, wxID_ANY, wxT("Total score:")),
		0, wxALL, SPACE_SIZE);
	uppergrid->Add(new wxTextCtrl(panel, ID_UPPERSECTIONTOTAL),
		1, wxALL, SPACE_SIZE);
	uppergrid->Add(new wxStaticText(panel, wxID_ANY, wxT("Bonus:")),
		0, wxALL, SPACE_SIZE);
	uppergrid->Add(new wxTextCtrl(panel, ID_BONUS),1,wxALL,SPACE_SIZE);
	uppergrid->Add(new wxStaticText(panel, wxID_ANY,
		wxT("Total of upper section:")), 0, wxALL, SPACE_SIZE);
	uppergrid->Add(new wxTextCtrl(panel, ID_UPPERTOTAL),
		1, wxALL, SPACE_SIZE);
	//END layout for the upper section of the score board

	//BEGIN layout for the lower section of the score board
	lowergrid->Add(new wxButton(panel,ID_THREEOFAKIND,wxT("3 of a kind")),
		0, wxALL, SPACE_SIZE);
	lowergrid->Add(new wxTextCtrl(panel, ID_THREEOFAKINDTEXT),
		1, wxALL, SPACE_SIZE);
	lowergrid->Add(new wxButton(panel,ID_FOUROFAKIND,wxT("4 of a kind")),
		0, wxALL, SPACE_SIZE);
	lowergrid->Add(new wxTextCtrl(panel, ID_FOUROFAKINDTEXT),
		1, wxALL, SPACE_SIZE);
	lowergrid->Add(new wxButton(panel,ID_FULLHOUSE,wxT("Full House")),
		0, wxALL, SPACE_SIZE);
	lowergrid->Add(new wxTextCtrl(panel, ID_FULLHOUSETEXT),
		1, wxALL, SPACE_SIZE);
	lowergrid->Add(new wxButton(panel, ID_SMALLSEQUENCE,
		wxT("Sequence of 4")), 0, wxALL,SPACE_SIZE);
	lowergrid->Add(new wxTextCtrl(panel, ID_SMALLSEQUENCETEXT),
		1, wxALL, SPACE_SIZE);
	lowergrid->Add(new wxButton(panel, ID_LARGESEQUENCE,
		wxT("Sequence of 5")), 0, wxALL, SPACE_SIZE);
	lowergrid->Add(new wxTextCtrl(panel, ID_LARGESEQUENCETEXT),
		1, wxALL, SPACE_SIZE);
	lowergrid->Add(new wxButton(panel, ID_YAHTZEE, wxT("Yahtzee")),
		0, wxALL, SPACE_SIZE);
	lowergrid->Add(new wxTextCtrl(panel,ID_YAHTZEETEXT),1,wxALL,SPACE_SIZE);
	lowergrid->Add(new wxButton(panel, ID_CHANCE, wxT("Chance")),
		0, wxALL, SPACE_SIZE);
	lowergrid->Add(new wxTextCtrl(panel, ID_CHANCETEXT),1,wxALL,SPACE_SIZE);
	lowergrid->Add(new wxStaticText(panel, wxID_ANY,
		wxT("Yahtzee Bonus")), 0, wxALL, SPACE_SIZE);
	lowergrid->Add(new wxTextCtrl(panel, ID_YAHTZEEBONUSTEXT),
		1, wxALL, SPACE_SIZE);
	lowergrid->Add(new wxStaticText(panel, wxID_ANY, 
		wxT("Total of lower section:")),0,wxALL,SPACE_SIZE);
	lowergrid->Add(new wxTextCtrl(panel, ID_LOWERTOTAL),1,wxALL,SPACE_SIZE);
	lowergrid->Add(new wxStaticText(panel, wxID_ANY, wxT("Grand Total:")),
		0, wxALL, SPACE_SIZE);
	lowergrid->Add(new wxTextCtrl(panel, ID_GRANDTOTAL),1,wxALL,SPACE_SIZE);
	//END layout for the lower section of the score board

	uppersection->Add(uppergrid);
	lowersection->Add(lowergrid);
	sectionsSizer->Add(uppersection, 0, wxALL, 5);
	sectionsSizer->Add(lowersection, 0, wxALL, 5);

	//BEGIN layout for the dice section of the score board
	if (m_config->get("horizontal-layout")=="True") {
		diceSizer->Add(new wxDynamicBitmap(panel, ID_DICE1,
			bitmap_dice[0]), 0, wxALL, DICE_SPACE);
		diceSizer->Add(new wxDynamicBitmap(panel,
			ID_DICE2, bitmap_dice[1]), 0, wxALL, DICE_SPACE);
		diceSizer->Add(new wxDynamicBitmap(panel,
			ID_DICE3, bitmap_dice[2]), 0, wxALL,DICE_SPACE);
		diceSizer->Add(new wxDynamicBitmap(panel,
			ID_DICE4, bitmap_dice[3]), 0, wxALL,DICE_SPACE);
		diceSizer->Add(new wxDynamicBitmap(panel,
			ID_DICE5, bitmap_dice[4]), 0, wxALL,DICE_SPACE);
		diceSizer->Add(new wxButton(panel, ID_ROLL, wxT("Roll!"),
			wxDefaultPosition, wxSize(64,64)), 0, wxALL,DICE_SPACE);
		diceSizer->Add(new wxCheckBox(panel, ID_DICE1KEEP,
			wxT("Keep")), 0, wxBOTTOM | wxLEFT,KEEP_SPACE);
		diceSizer->Add(new wxCheckBox(panel, ID_DICE2KEEP,
			wxT("Keep")), 0, wxBOTTOM | wxLEFT, KEEP_SPACE);
		diceSizer->Add(new wxCheckBox(panel, ID_DICE3KEEP,
			wxT("Keep")), 0, wxBOTTOM | wxLEFT, KEEP_SPACE);
		diceSizer->Add(new wxCheckBox(panel, ID_DICE4KEEP,
			wxT("Keep")), 0, wxBOTTOM | wxLEFT, KEEP_SPACE);
		diceSizer->Add(new wxCheckBox(panel, ID_DICE5KEEP,
			wxT("Keep")), 0, wxBOTTOM | wxLEFT, KEEP_SPACE);
	} else {
		diceSizer->Add(new wxDynamicBitmap(panel, ID_DICE1,
			bitmap_dice[0]), 0, wxALL, DICE_SPACE);
		diceSizer->Add(new wxCheckBox(panel, ID_DICE1KEEP,
			wxT("Keep")), 0, wxLEFT, KEEP_SPACE);
		diceSizer->AddSpacer(VER_DICE_SPACER);
		diceSizer->Add(new wxDynamicBitmap(panel,ID_DICE2,
			bitmap_dice[1]), 0, wxALL, DICE_SPACE);
		diceSizer->Add(new wxCheckBox(panel, ID_DICE2KEEP,
			wxT("Keep")), 0, wxLEFT, KEEP_SPACE);
		diceSizer->AddSpacer(VER_DICE_SPACER);
		diceSizer->Add(new wxDynamicBitmap(panel, ID_DICE3,
			bitmap_dice[2]), 0, wxALL, DICE_SPACE);
		diceSizer->Add(new wxCheckBox(panel, ID_DICE3KEEP,
			wxT("Keep")), 0, wxLEFT, KEEP_SPACE);
		diceSizer->AddSpacer(VER_DICE_SPACER);
		diceSizer->Add(new wxDynamicBitmap(panel, ID_DICE4,
			bitmap_dice[3]), 0, wxALL, DICE_SPACE);
		diceSizer->Add(new wxCheckBox(panel, ID_DICE4KEEP,
			wxT("Keep")), 0, wxLEFT, KEEP_SPACE);
		diceSizer->AddSpacer(VER_DICE_SPACER);
		diceSizer->Add(new wxDynamicBitmap(panel, ID_DICE5, 
			bitmap_dice[4]), 0, wxALL, DICE_SPACE);
		diceSizer->Add(new wxCheckBox(panel, ID_DICE5KEEP,
			wxT("Keep")), 0, wxLEFT, KEEP_SPACE);
		diceSizer->AddSpacer(VER_DICE_SPACER);
		diceSizer->Add(new wxButton(panel, ID_ROLL, wxT("Roll!"),
			wxDefaultPosition, wxSize(VERTICAL_ROLL_SIZEX,
			VERTICAL_ROLL_SIZEY)), 0, wxALL, DICE_SPACE);
	}
	//END layout for the dice section of the score board *******/
	
	topSizer->Add(sectionsSizer);
	topSizer->Add(diceSizer);	

	panel->SetSizer(topSizer);
	
	topSizer->Fit(this);
	topSizer->SetSizeHints(this);
}

/**
 * Connects the tables to the event handlers
 */
void MainFrame::ConnectEventTable()
{
	//BEGIN connecting the menu items' events
	Connect(wxID_EXIT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnQuit));
	Connect(wxID_ABOUT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnAbout));
	Connect(ID_CHECK_FOR_UPDATES, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnCheckForUpdates));
	Connect(ID_SENDCOMMENT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnSendComment));
	Connect(wxID_NEW, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnNewGame));
	Connect(wxID_UNDO, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnUndo));
	Connect(ID_SHOWHIGHSCORE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnShowHighscore));
	Connect(ID_SETTINGS, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnSettings));
	//END connecting the menu items' events

	Connect(ID_ROLL, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (MainFrame::OnRollButton));
	Connect(ID_ROLL, wxEVT_ENABLE_ROLL, wxCommandEventHandler (MainFrame::OnRollButton));

	//BEGIN connecting the scoreboard buttons to the events
	Connect(ID_ACES,ID_SIXES, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (MainFrame::OnUpperButtons));
	Connect(ID_THREEOFAKIND, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (MainFrame::On3ofakindButton));
	Connect(ID_FOUROFAKIND, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (MainFrame::On4ofakindButton));
	Connect(ID_FULLHOUSE, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (MainFrame::OnFullHouseButton));
	Connect(ID_SMALLSEQUENCE, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (MainFrame::OnSmallSequenceButton));
	Connect(ID_LARGESEQUENCE, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (MainFrame::OnLargeSequenceButton));
	Connect(ID_YAHTZEE, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (MainFrame::OnYahtzeeButton));
	Connect(ID_CHANCE, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (MainFrame::OnChanceButton));
	Connect(ID_DICE1,ID_DICE5, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler (MainFrame::OnDiceClick));
	Connect(ID_DICE1KEEP,ID_DICE5KEEP, wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler (MainFrame::OnKeepClick));
	//END connecting the scoreboard buttons to the event

	/* The following code connects the on wxEVT_ENTER_WINDOW and 
	 * wxEVT_LEAVE_WINDOW events to the score buttons to handle the score
	 * hints. For some unknown technical problem we have to directly connect
	 * the events, otherwise the wxEVT_LEAVE_WINDOW doesn't get generated.
	 * We have to use event handler other than MainFrame's as the regular
	 * event handler leads to segmantation fault.
	 */
	wxButton *temp;
	for (int i = ID_ACES; i<=ID_CHANCE; i++) {
		temp = ((wxButton *)FindWindow(i));
		temp->Connect(wxEVT_LEAVE_WINDOW, wxMouseEventHandler(MainFrameEvtHandler::OnScoreMouseLeave), NULL, m_evt_handler);
		temp->Connect(wxEVT_ENTER_WINDOW, wxMouseEventHandler(MainFrameEvtHandler::OnScoreMouseEnter), NULL, m_evt_handler);
	}
}


/*********EVENT PROCCESSING FUNCTIONS********/

void MainFrame::OnAbout(wxCommandEvent& event)
{
#ifdef PORTABLE
	AboutDialog *about = new AboutDialog(this,wxID_ANY,wxT("About Open Yahtzee PE"));
#else
	AboutDialog *about = new AboutDialog(this,wxID_ANY,wxT("About Open Yahtzee"));
#endif
	about->ShowModal();
}

/**
 * Connects to the Open Yahtzee website on sourceforge.net and checks for 
 * updates to the game.
 * \param event 
 */
void MainFrame::OnCheckForUpdates (wxCommandEvent& event)
{
	
	wxString link = wxT("http://openyahtzee.sourceforge.net/download/check-for-updates/");
	
	link += wxT(VERSION);

	wxLaunchDefaultBrowser(link);
}


/**
 * Starts up the user browser and connects to the Open Yahtzee website's
 * feedback page.
 * \param event 
 */
void MainFrame::OnSendComment (wxCommandEvent& event)
{
	wxString link = wxT("http://openyahtzee.sourceforge.net/feedback/");
	
	wxLaunchDefaultBrowser(link);
}

void MainFrame::OnQuit(wxCommandEvent& event)
{
	// Destroy the frame
	Close();
}

/**
 * This is the event handler of the New Game menu item. It starts a new 
 * game and clears the board from the last game.
 * \param event 
 */
void MainFrame::OnNewGame(wxCommandEvent& event)
{
	//disable the undo button so it won't be enabled when a new game is started.
	(GetMenuBar()->FindItem(wxID_UNDO))->Enable(false);
	
	ResetRolls();
	m_yahtzee = false;
	m_numofplaysleft = 13;
	
	for (int i = ID_ACESTEXT; i<= ID_GRANDTOTAL; i++)
		((wxTextCtrl*) FindWindow(i))->Clear();
	for (int i = ID_ACES; i<= ID_CHANCE; i++)
		((wxButton*) FindWindow(i))->Enable(true);
}

/**
 * This function handles the undo events. It's connected to the Undo menu item.
 *
 * Note that the function only allows undoing of scoring actions and not dice
 * rolls. Also it won't allow the undoing of the last move of the game because
 * of a technical problem relating to the case an high-score was made and then
 * the user undo the last move and rescore another high-score and so on.
 * \param event 
 */
void MainFrame::OnUndo(wxCommandEvent& event)
{
	m_rolls = m_rollsundo;

	//after the user scored the button was enabled, check if it should be disabled
	if (m_rolls <= 0) //we don't have remaining rolls 
		((wxButton*) FindWindow(ID_ROLL)) -> Enable(false);
	
	// change the displayed roll counter
	wxString caption = wxString::Format(wxT("Roll! (%i)"),m_rolls);
	FindWindow(ID_ROLL)->SetLabel(caption);

	//restore the 'keep' checkboxes
	for (int i=0; i<5; i++)
		((wxCheckBox*) FindWindow(i + ID_DICE1KEEP)) -> Enable(true);
	
	//reset the users last choice
	FindWindow(m_lastmove)->Enable(true);

	//clear the score;
	((wxTextCtrl*)FindWindow(ID_ACESTEXT + (m_lastmove - ID_ACES)))->SetValue(wxT(""));
	
	//Undo the Yahtzee flag if needed
	if (m_lastmove == ID_YAHTZEE) {
		m_yahtzee = false;
	}

	//undo also the yahtzee bonus if needed
	if (m_yahtzeebonus) {
		long temp;
		wxString tempstr;
	
		tempstr = ((wxTextCtrl*) FindWindow(ID_YAHTZEEBONUSTEXT)) -> GetValue();
		tempstr.ToLong(&temp,10);
		temp -= 100; //this line reduces the points given for the yahtzee bonus
		tempstr.Printf(wxT("%i"),temp);
		((wxTextCtrl*) FindWindow(ID_YAHTZEEBONUSTEXT)) -> SetValue(tempstr);
	}

	//recalculate the subtotals
	CalculateSubTotal();

	(GetMenuBar()->FindItem(wxID_UNDO))->Enable(false);
	//cancel the counting for the choice that was canceled
	m_numofplaysleft++;
}

/**
 * This function enables the undo button and stores the last move
 * \param id 
 */
inline void MainFrame::EnableUndo(int id)
{
	if (m_numofplaysleft) {
		(GetMenuBar()->FindItem(wxID_UNDO))->Enable(true);
		m_lastmove = id;
	}
}

/**
 * Shows the high-score dialog. Connected to the Game->"Show High Score" menu item.
 * \param event 
 */
void MainFrame::OnShowHighscore(wxCommandEvent& event)
{
	highscores_dialog::HighscoresDialog *dialog = new highscores_dialog::HighscoresDialog(this,m_config);

	dialog->ShowModal();

	delete dialog;
}

/**
 * Shows the settings dialog. This event-handler is connected to Game->Settings
 * menu item.
 * \param event 
 */
void MainFrame::OnSettings( wxCommandEvent& event)
{
	settings_dialog::SettingsDialog* settings_dialog = new settings_dialog::SettingsDialog(this, m_config);

	if (settings_dialog->ShowModal()!=wxID_OK) {
		delete settings_dialog;
		return;
	}

	((wxTextCtrl*) FindWindow(ID_UPPERSECTIONTOTAL)) -> SetValue(wxT(""));
	((wxTextCtrl*) FindWindow(ID_LOWERTOTAL)) -> SetValue(wxT(""));
	CalculateSubTotal();

	Relayout();

	delete settings_dialog;
}

/**
 * Event handler for the Roll button. It checks the settings if it should
 * animate the dice and then rolls them accordingly. It also checks for the
 * status of the "keep" checkboxes.
 * \param event 
 */
void MainFrame::OnRollButton (wxCommandEvent& event)
{
	short int dice[5];	//holds the dices score
	
	if (event.GetEventType() == wxEVT_IDLE) {
		m_skiproll = false;
		Disconnect(wxEVT_IDLE,  wxCommandEventHandler(MainFrame::OnRollButton));
		return;
	}
	
	//skip rolling the dice if the user accidently rolled the dice before they finished spinning.
	if (m_skiproll) return;
	m_skiproll = true;

	//fill the dice array with the old values
	dice[0]=m_score_dice.GetDice(1);
	dice[1]=m_score_dice.GetDice(2);
	dice[2]=m_score_dice.GetDice(3);
	dice[3]=m_score_dice.GetDice(4);
	dice[4]=m_score_dice.GetDice(5);
	//roll the dice...
	if (m_config->get("dice-animation")=="True") {
		int dice_throws[5] = {0,0,0,0,0};
		for (int i=0; i<5; i++) { //set the number of rolls for each dice
			if (!((wxCheckBox*) FindWindow(i + ID_DICE1KEEP))->IsChecked()) {
				dice_throws[i] = (rand()%15)+3; //ensures the number is at least one.
			}
		}
		while (dice_throws[0] || dice_throws[1] || dice_throws[2] || dice_throws[3] || dice_throws[4]) {
			for (int i=0 ; i<5; i++){
				if(dice_throws[i]){
					dice_throws[i]--;
					dice[i] = (int)(6.0*rand()/RAND_MAX)+1;
					((wxDynamicBitmap*) FindWindow(i + 
						ID_DICE1)) -> SetBitmap(
						bitmap_dice[dice[i]-1]);
				}
			}
			::wxMilliSleep(100);
		}
	} else {
		for (int i=0; i<5; i++) {
			if (!((wxCheckBox*) FindWindow(i + ID_DICE1KEEP))->IsChecked()) {
				dice[i] = (int)(6.0*rand()/RAND_MAX)+1;
				((wxDynamicBitmap*) FindWindow(i + ID_DICE1))->
					SetBitmap(bitmap_dice[dice[i]-1]);
			}
		}
	}

	m_score_dice.SetDice(dice);
	m_score_dice.SetYahtzeeJoker(YahtzeeJoker());
	--m_rolls;
	
	// change the displayed roll counter
	wxString caption = wxString::Format(wxT("Roll! (%i)"),m_rolls);
	FindWindow(ID_ROLL)->SetLabel(caption);

	#ifndef DEBUG
	if (m_rolls <= 0) {
		((wxButton*) FindWindow(ID_ROLL)) -> Enable(false);
		FindWindow(ID_ROLL)->SetLabel(wxT("Roll!"));
	}

	#endif
	
	//enable the keep checkboxes
	for (int i=0; i<5; i++)
		((wxCheckBox*) FindWindow(i + ID_DICE1KEEP)) -> Enable(true);
	
	//we rolled the dices so undoing isn't allowed
	(GetMenuBar()->FindItem(wxID_UNDO))->Enable(false);
	m_yahtzeebonus = false; //if we scored yahtzee bonus before we don't care anymore.
	
	Connect(wxEVT_IDLE, wxCommandEventHandler(MainFrame::OnRollButton));
}

/**
 * This is the event-handler for all of the scoring buttons of the upper section.
 *
 * It gets the id of the button that called the event handler by comparing it 
 * to the id of the aces button it figures what button was pressed, And updates
 * the suiting score textbox (again by comparing the id to the one of the aces
 * checbox).
 * \param event 
 */
void MainFrame::OnUpperButtons (wxCommandEvent& event)
{
	wxString out;
	short int temp;
	if(m_rolls < 3){
		YahtzeeBonus();
		switch(event.GetId()) {
		case ID_ACES:
			temp = m_score_dice.Aces();
			break;
		case ID_TWOS:
			temp = m_score_dice.Twos();
			break;
		case ID_THREES:
			temp = m_score_dice.Threes();
			break;
		case ID_FOURS:
			temp = m_score_dice.Fours();
			break;
		case ID_FIVES:
			temp = m_score_dice.Fives();
			break;
		case ID_SIXES:
			temp = m_score_dice.Sixes();
			break;
		}
	
		out.Printf(wxT("%i"),temp);
		((wxTextCtrl*) FindWindow(event.GetId() - ID_ACES + ID_ACESTEXT))->SetValue(out);
		((wxTextCtrl*) FindWindow(event.GetId() - ID_ACES + ID_ACESTEXT))->SetBackgroundColour(*wxWHITE);
		
		PostScore(event.GetId());
	}
	else 
		wxMessageBox(wxT("First you need to roll, and after you roll you may score"), wxT("Open Yahtzee"), wxOK | wxICON_INFORMATION, this);
}

/**
 * Event handler for the "3 of a kind" score button.
 * \param event 
 */
void MainFrame::On3ofakindButton(wxCommandEvent& event)
{
	if(m_rolls>=3) {
		wxMessageBox(wxT("First you need to roll, and after you roll you may score"), wxT("Open Yahtzee"), wxOK | wxICON_INFORMATION, this);
		return;
	}
	YahtzeeBonus();
	wxString out;
	
	out.Printf(wxT("%i"),m_score_dice.ThreeOfAKind());
	((wxTextCtrl*) FindWindow(ID_THREEOFAKINDTEXT))->SetValue(out);
	((wxTextCtrl*) FindWindow(ID_THREEOFAKINDTEXT))->SetBackgroundColour(*wxWHITE);
	
	PostScore(event.GetId());
}

/**
 * Event handler for the "4 of a kind" score button.
 * \param event 
 */
void MainFrame::On4ofakindButton(wxCommandEvent& event)
{
	if(m_rolls>=3) {
		wxMessageBox(wxT("First you need to roll, and after you roll you may score"), wxT("Open Yahtzee"), wxOK | wxICON_INFORMATION, this);
		return;
	}
	YahtzeeBonus();
	wxString out;

	out.Printf(wxT("%i"),m_score_dice.FourOfAKind());
	((wxTextCtrl*) FindWindow(ID_FOUROFAKINDTEXT))->SetValue(out);
	((wxTextCtrl*) FindWindow(ID_FOUROFAKINDTEXT))->SetBackgroundColour(*wxWHITE);
	
	PostScore(event.GetId());
}

/**
 * Event handler for the full-house score button.
 * \param event 
 */
void MainFrame::OnFullHouseButton(wxCommandEvent& event)
{
	wxString out;

	if(m_rolls>=3) {
		wxMessageBox(wxT("First you need to roll, and after you roll you may score"), wxT("Open Yahtzee"), wxOK | wxICON_INFORMATION, this);
		return;
	}
	YahtzeeBonus();

	out.Printf(wxT("%i"), m_score_dice.FullHouse());
	((wxTextCtrl*) FindWindow(ID_FULLHOUSETEXT))->SetValue(out);
	((wxTextCtrl*) FindWindow(ID_FULLHOUSETEXT))->SetBackgroundColour(*wxWHITE);
	
	PostScore(event.GetId());
}

/**
 * Event handler for the small-sequence score button.
 * \param event 
 */
void MainFrame::OnSmallSequenceButton(wxCommandEvent& event)
{
	wxString out;

	if(m_rolls>=3) {
		wxMessageBox(wxT("First you need to roll, and after you roll you may score"), wxT("Open Yahtzee"), wxOK | wxICON_INFORMATION, this);
		return;
	}

	YahtzeeBonus();
	
	out.Printf(wxT("%i"), m_score_dice.SmallSequence());
	((wxTextCtrl*) FindWindow(ID_SMALLSEQUENCETEXT))->SetValue(out);
	((wxTextCtrl*) FindWindow(ID_SMALLSEQUENCETEXT))->SetBackgroundColour(*wxWHITE);
	
	PostScore(event.GetId());
}

/**
 * Event handler for the large-sequence score button.
 * \param event 
 */
void MainFrame::OnLargeSequenceButton(wxCommandEvent& event)
{
	wxString out;
	
	if(m_rolls>=3) {
		wxMessageBox(wxT("First you need to roll, and after you roll you may score"), wxT("Open Yahtzee"), wxOK | wxICON_INFORMATION, this);
		return;
	}

	YahtzeeBonus();

	out.Printf(wxT("%i"), m_score_dice.LargeSequence());
	((wxTextCtrl*) FindWindow(ID_LARGESEQUENCETEXT))->SetValue(out);
	((wxTextCtrl*) FindWindow(ID_LARGESEQUENCETEXT))->SetBackgroundColour(*wxWHITE);
	
	PostScore(event.GetId());
}

/**
 * Event handler for the yahtzee score button.
 * \param event 
 */
void MainFrame::OnYahtzeeButton(wxCommandEvent& event)
{
	wxString out;
	
	if(m_rolls>=3) {
		wxMessageBox(wxT("First you need to roll, and after you roll you may score"), wxT("Open Yahtzee"), wxOK | wxICON_INFORMATION, this);
		return;
	}
	
	if (m_score_dice.IsYahtzee()) m_yahtzee = true;
	out.Printf(wxT("%i"), m_score_dice.Yahtzee());
	((wxTextCtrl*) FindWindow(ID_YAHTZEETEXT))->SetValue(out);
	((wxTextCtrl*) FindWindow(ID_YAHTZEETEXT))->SetBackgroundColour(*wxWHITE);

	PostScore(event.GetId());
}

/**
 * Evnet handler for the chance score button.
 * \param event 
 */
void MainFrame::OnChanceButton (wxCommandEvent& event)
{
	wxString out;
	int temp = 0;
	if(m_rolls < 3){
		YahtzeeBonus();
	
		out.Printf(wxT("%i"),m_score_dice.Chance());
		((wxTextCtrl*) FindWindow(ID_CHANCETEXT))->SetValue(out);
		((wxTextCtrl*) FindWindow(ID_CHANCETEXT))->SetBackgroundColour(*wxWHITE);
		
		PostScore(event.GetId());
	}
	else 
		wxMessageBox(wxT("First you need to roll, and after you roll you may score"), wxT("Open Yahtzee"), wxOK | wxICON_INFORMATION, this);

}

/**
 * Event handler for mouse clicks on the dice.
 *
 * When clicking on the dice this event-handler ticks the appropriate "keep" checkbox.
 * \param event 
 */
void MainFrame::OnDiceClick (wxCommandEvent& event)
{
	//tick the checkbox
	if (((wxCheckBox*) FindWindow(event.GetId()-ID_DICE1 + ID_DICE1KEEP))->IsEnabled()){
		bool newvalue = (((wxCheckBox*) FindWindow(event.GetId()-ID_DICE1 + ID_DICE1KEEP))->GetValue())?false:true;
		((wxCheckBox*) FindWindow(event.GetId()-ID_DICE1 + ID_DICE1KEEP)) -> SetValue(newvalue);
	}

	//dispatch a click event on the checkbox
	wxCommandEvent clickevent((event.GetId()-ID_DICE1 + ID_DICE1KEEP),wxEVT_COMMAND_CHECKBOX_CLICKED);
	clickevent.SetEventObject( this );
	clickevent.SetId(event.GetId()-ID_DICE1 + ID_DICE1KEEP);
	this->OnKeepClick(clickevent); ///\todo find a better way to call the event handler.
}

void MainFrame::OnKeepClick (wxCommandEvent& event)
{
	wxCheckBox *temp = (wxCheckBox*) FindWindow(event.GetId());
	((wxDynamicBitmap*) FindWindow(event.GetId()-ID_DICE1KEEP + ID_DICE1))->SetGrayScale(temp->GetValue());
}


//********************************************
//******	General Functions	******
//********************************************

/**
 * This function handles everything related to reseting the dice rolls after scoring.
 */
void MainFrame::ResetRolls()
{
	m_rollsundo = m_rolls;
	m_rolls = 3;
	((wxButton*) FindWindow(ID_ROLL)) -> Enable(true);

	// reset the roll count in the caption
	FindWindow(ID_ROLL)->SetLabel(wxT("Roll! (3)"));
	for (int i=0; i<5; i++){ 
		((wxCheckBox*) FindWindow(i + ID_DICE1KEEP)) -> SetValue(false);
		((wxCheckBox*) FindWindow(i + ID_DICE1KEEP)) -> Enable(false);
		((wxDynamicBitmap*) FindWindow(i+ID_DICE1)) -> SetGrayScale(false);
	}
}

/**
 * This function checks for a Yahtzee Bonus situation and if one exists it scores accordingly.
 */
void MainFrame::YahtzeeBonus()
{
	long temp;
	wxString tempstr;
	
	//if the player didn't have any yathzees yet he can't have the bonus;
	if (!m_yahtzee)
		return;
	if (m_score_dice.IsYahtzee()) {
		tempstr = ((wxTextCtrl*) FindWindow(ID_YAHTZEEBONUSTEXT)) -> GetValue();
		tempstr.ToLong(&temp,10);
		temp += 100;
		tempstr.Printf(wxT("%i"),temp);
		((wxTextCtrl*) FindWindow(ID_YAHTZEEBONUSTEXT)) -> SetValue(tempstr);
		m_yahtzeebonus = true;
	}	
}

/**
 * This function checks whether we have a Yahtzee Joker situation.
 * \return true if there is Yahtzee Joker, false otherwise.
 */
bool MainFrame::YahtzeeJoker()
{
	if (m_score_dice.IsYahtzee() && !(FindWindow(ID_ACES + 
		m_score_dice.GetDice(1)-1)->IsEnabled()) && 
		!(FindWindow(ID_YAHTZEE)->IsEnabled())) {
		return true;
	}
	return false;
}

/**
 * This function handles the end of game.
 *
 * When a game ends it calculates the total score and submit it to the high
 * score list.
 */
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

/**
 * This function checks if a given score qualifies for the high score list and
 * adds it to the list if it does.
 * @param score The score submitted to the high score list.
 */
void MainFrame::HighScoreHandler(int score)
{
	int place;
	std::string name,date;
	wxCommandEvent newevent;

	
	if(!m_config->isHighscore(score)) {
		return;
	}
	
	wxString msg;
	msg.Printf(wxT("Your score made it to the high score table.\nPlease enter your name below:"),place);

	wxString last_name = wxString::FromUTF8(m_config->get("last-name").c_str());

	wxTextEntryDialog infodialog(this,msg,wxT("Please enter your name"),last_name ,wxOK | wxCENTRE);
	infodialog.ShowModal();

	name = infodialog.GetValue().utf8_str();
	m_config->set("last-name",name)->save();

	//get the date
	wxDateTime now = wxDateTime::Now();
	date = now.FormatDate().utf8_str();
	date += " ";
	date += now.FormatISOTime().SubString(0,4).utf8_str();

	int rank = m_config->submitHighscore(score, name, date);

	//now show the high score table
	highscores_dialog::HighscoresDialog *dialog = new highscores_dialog::HighscoresDialog(this,m_config,rank);

	dialog->ShowModal();

	delete dialog;

}

///this function handles all the post scoring stuff such as disabling the right button.
/**
 * This function is always called after scoring and it handles all the post-score
 * stuff such as reseting the dice rolls, enabling the undo button, calculating
 * the sub-total scores and ending the game if necessary.
 * \param id 
 */
void MainFrame::PostScore(int id)
{
	//now after the scoring reset the rolls
	ResetRolls();

	CalculateSubTotal();

	//and disable the button
	FindWindow(id)->Enable(false);
	m_numofplaysleft--;
	EnableUndo(id);
	EndofGame();
}

/**
 * This function calculates the sub-total scores and should be called after
 * every score. It does so only if this feature is requested in the settings
 * dialog.
 */
void MainFrame::CalculateSubTotal()
{
	if (m_config->get("calculate-subtotal")=="False")
		return;
	long upperscore = 0;
	long lowerscore = 0;
	wxString tempstr;
	long temp;


	for (int i = ID_ACESTEXT; i<=ID_SIXESTEXT; i++){
		tempstr = ((wxTextCtrl*) FindWindow(i)) -> GetValue();
		tempstr.ToLong(&temp,10);
		upperscore +=temp;
	}
	
	tempstr.Printf(wxT("%i"),upperscore);
	((wxTextCtrl*) FindWindow(ID_UPPERSECTIONTOTAL)) -> SetValue(tempstr);
	if (upperscore >= 63) {
		((wxTextCtrl*) FindWindow(ID_BONUS))->SetValue(wxT("35"));
	} else {
		// this is needed in case of an undo
		((wxTextCtrl*) FindWindow(ID_BONUS))->SetValue(wxT(""));
	}

	for (int i = ID_THREEOFAKINDTEXT; i<=ID_YAHTZEEBONUSTEXT; i++) {
		tempstr = ((wxTextCtrl*) FindWindow(i)) -> GetValue();
		tempstr.ToLong(&temp,10);
		lowerscore +=temp;
	}
	
	tempstr.Printf(wxT("%i"),lowerscore);
	((wxTextCtrl*) FindWindow(ID_LOWERTOTAL)) -> SetValue(tempstr);
}


void MainFrame::Relayout()
{
	wxBoxSizer *topSizer;
	wxFlexGridSizer *diceSizer;
	bool roll_button_enabled;

	lowersection->GetStaticBox()->Destroy();
	uppersection->GetStaticBox()->Destroy();
	sectionsSizer->Remove(lowersection);
	sectionsSizer->Remove(uppersection);

	if (m_config->get("horizontal-layout")=="True") {
		topSizer = new wxBoxSizer( wxVERTICAL );
		sectionsSizer = new wxBoxSizer( wxHORIZONTAL );
		diceSizer = new wxFlexGridSizer(2, 0, 0, 0);
	} else {
		topSizer = new wxBoxSizer( wxHORIZONTAL );
		sectionsSizer = new wxBoxSizer( wxVERTICAL );
		diceSizer = new wxFlexGridSizer(1, 0, 0);;
	}

	uppersection = new wxStaticBoxSizer( new wxStaticBox( FindWindow(ID_PANEL), wxID_ANY, wxT("Upper Section") ), wxVERTICAL);
	lowersection = new wxStaticBoxSizer( new wxStaticBox( FindWindow(ID_PANEL), wxID_ANY, wxT("Lower Section") ), wxVERTICAL);
	
	wxFlexGridSizer* uppergrid = new wxFlexGridSizer(2, 0, 10);
	wxFlexGridSizer* lowergrid = new wxFlexGridSizer(2, 0, 10);

	//BEGIN layout for the upper section of the score board
	uppergrid->Add(FindWindow(ID_ACES),0,wxALL,SPACE_SIZE);
	uppergrid->Add(FindWindow(ID_ACESTEXT),1,wxALL,SPACE_SIZE);
	uppergrid->Add(FindWindow(ID_TWOS),0,wxALL,SPACE_SIZE);
	uppergrid->Add(FindWindow(ID_TWOSTEXT),1,wxALL,SPACE_SIZE);
	uppergrid->Add(FindWindow(ID_THREES),0,wxALL,SPACE_SIZE);
	uppergrid->Add(FindWindow(ID_THREESTEXT),1,wxALL,SPACE_SIZE);
	uppergrid->Add(FindWindow(ID_FOURS),0,wxALL,SPACE_SIZE);
	uppergrid->Add(FindWindow(ID_FOURSTEXT),1,wxALL,SPACE_SIZE);
	uppergrid->Add(FindWindow(ID_FIVES),0,wxALL,SPACE_SIZE);
	uppergrid->Add(FindWindow(ID_FIVESTEXT),1,wxALL,SPACE_SIZE);
	uppergrid->Add(FindWindow(ID_SIXES),0,wxALL,SPACE_SIZE);
	uppergrid->Add(FindWindow(ID_SIXESTEXT),1,wxALL,SPACE_SIZE);
	uppergrid->Add(FindWindowByLabel(wxT("Total score:")),0,wxALL,SPACE_SIZE);
	uppergrid->Add(FindWindow(ID_UPPERSECTIONTOTAL),1,wxALL,SPACE_SIZE);
	uppergrid->Add(FindWindowByLabel(wxT("Bonus:")),0,wxALL,SPACE_SIZE);
	uppergrid->Add(FindWindow(ID_BONUS),1,wxALL,SPACE_SIZE);
	uppergrid->Add(FindWindowByLabel(wxT("Total of upper section:")),0,wxALL,SPACE_SIZE);
	uppergrid->Add(FindWindow(ID_UPPERTOTAL),1,wxALL,SPACE_SIZE);
	//END layout for the upper section of the score board

	//BEGIN layout for the lower section of the score board
	lowergrid->Add(FindWindow(ID_THREEOFAKIND),0,wxALL,SPACE_SIZE);
	lowergrid->Add(FindWindow(ID_THREEOFAKINDTEXT),1,wxALL,SPACE_SIZE);
	lowergrid->Add(FindWindow(ID_FOUROFAKIND),0,wxALL,SPACE_SIZE);
	lowergrid->Add(FindWindow(ID_FOUROFAKINDTEXT),1,wxALL,SPACE_SIZE);
	lowergrid->Add(FindWindow(ID_FULLHOUSE),0,wxALL,SPACE_SIZE);
	lowergrid->Add(FindWindow(ID_FULLHOUSETEXT),1,wxALL,SPACE_SIZE);
	lowergrid->Add(FindWindow(ID_SMALLSEQUENCE),0,wxALL,SPACE_SIZE);
	lowergrid->Add(FindWindow(ID_SMALLSEQUENCETEXT),1,wxALL,SPACE_SIZE);
	lowergrid->Add(FindWindow(ID_LARGESEQUENCE),0,wxALL,SPACE_SIZE);
	lowergrid->Add(FindWindow(ID_LARGESEQUENCETEXT),1,wxALL,SPACE_SIZE);
	lowergrid->Add(FindWindow(ID_YAHTZEE),0,wxALL,SPACE_SIZE);
	lowergrid->Add(FindWindow(ID_YAHTZEETEXT),1,wxALL,SPACE_SIZE);
	lowergrid->Add(FindWindow(ID_CHANCE),0,wxALL,SPACE_SIZE);
	lowergrid->Add(FindWindow(ID_CHANCETEXT),1,wxALL,SPACE_SIZE);
	lowergrid->Add(FindWindowByLabel(wxT("Yahtzee Bonus")),0,wxALL,SPACE_SIZE);
	lowergrid->Add(FindWindow(ID_YAHTZEEBONUSTEXT),1,wxALL,SPACE_SIZE);
	lowergrid->Add(FindWindowByLabel(wxT("Total of lower section:")),0,wxALL,SPACE_SIZE);
	lowergrid->Add(FindWindow(ID_LOWERTOTAL),1,wxALL,SPACE_SIZE);
	lowergrid->Add(FindWindowByLabel(wxT("Grand Total:")),0,wxALL,SPACE_SIZE);
	lowergrid->Add(FindWindow(ID_GRANDTOTAL),1,wxALL,SPACE_SIZE);
	//END layout for the lower section of the score board

	uppersection->Add(uppergrid);
	lowersection->Add(lowergrid);
	sectionsSizer->Add(uppersection,0,wxALL,5);
	sectionsSizer->Add(lowersection,0,wxALL,5);

	//Change the roll button size if we need to
	roll_button_enabled = FindWindow(ID_ROLL)->IsEnabled();
	if (m_config->get("horizontal-layout")=="True") {
		FindWindow(ID_ROLL)->Destroy();
		new wxButton(FindWindow(ID_PANEL), ID_ROLL, wxT("Roll!"),wxDefaultPosition,wxSize(64,64));
	} else {
		FindWindow(ID_ROLL)->Destroy();
		new wxButton(FindWindow(ID_PANEL), ID_ROLL, wxT("Roll!"),wxDefaultPosition,wxSize(VERTICAL_ROLL_SIZEX,VERTICAL_ROLL_SIZEY));
	}
	FindWindow(ID_ROLL)->Enable(roll_button_enabled);

	// if there are rolls left we should display the count of them
	if (roll_button_enabled) {
		wxString caption = wxString::Format(wxT("Roll! (%i)"),m_rolls);
		FindWindow(ID_ROLL)->SetLabel(caption);
	}

	
	//BEGIN layout for the dice section of the score board
	if (m_config->get("horizontal-layout")=="True") {
		diceSizer->Add(FindWindow(ID_DICE1),0,wxALL,DICE_SPACE);
		diceSizer->Add(FindWindow(ID_DICE2),0,wxALL,DICE_SPACE);
		diceSizer->Add(FindWindow(ID_DICE3),0,wxALL,DICE_SPACE);
		diceSizer->Add(FindWindow(ID_DICE4),0,wxALL,DICE_SPACE);
		diceSizer->Add(FindWindow(ID_DICE5),0,wxALL,DICE_SPACE);
		diceSizer->Add(FindWindow(ID_ROLL),0,wxALL,DICE_SPACE);
		diceSizer->Add(FindWindow(ID_DICE1KEEP),0,wxBOTTOM | wxLEFT,KEEP_SPACE);
		diceSizer->Add(FindWindow(ID_DICE2KEEP),0,wxBOTTOM | wxLEFT,KEEP_SPACE);
		diceSizer->Add(FindWindow(ID_DICE3KEEP),0,wxBOTTOM | wxLEFT,KEEP_SPACE);
		diceSizer->Add(FindWindow(ID_DICE4KEEP),0,wxBOTTOM | wxLEFT,KEEP_SPACE);
		diceSizer->Add(FindWindow(ID_DICE5KEEP),0,wxBOTTOM | wxLEFT,KEEP_SPACE);
	} else {
		diceSizer->Add(FindWindow(ID_DICE1),0,wxALL,DICE_SPACE);
		diceSizer->Add(FindWindow(ID_DICE1KEEP),0,wxLEFT,KEEP_SPACE);
		diceSizer->AddSpacer(VER_DICE_SPACER);
		diceSizer->Add(FindWindow(ID_DICE2),0,wxALL,DICE_SPACE);
		diceSizer->Add(FindWindow(ID_DICE2KEEP),0,wxLEFT,KEEP_SPACE);
		diceSizer->AddSpacer(VER_DICE_SPACER);
		diceSizer->Add(FindWindow(ID_DICE3),0,wxALL,DICE_SPACE);
		diceSizer->Add(FindWindow(ID_DICE3KEEP),0,wxLEFT,KEEP_SPACE);
		diceSizer->AddSpacer(VER_DICE_SPACER);
		diceSizer->Add(FindWindow(ID_DICE4),0,wxALL,DICE_SPACE);
		diceSizer->Add(FindWindow(ID_DICE4KEEP),0,wxLEFT,KEEP_SPACE);
		diceSizer->AddSpacer(VER_DICE_SPACER);
		diceSizer->Add(FindWindow(ID_DICE5),0,wxALL,DICE_SPACE);
		diceSizer->Add(FindWindow(ID_DICE5KEEP),0,wxLEFT,KEEP_SPACE);
		diceSizer->AddSpacer(VER_DICE_SPACER);
		diceSizer->Add(FindWindow(ID_ROLL),0,wxALL,DICE_SPACE);
	}
	//END layout for the dice section of the score board *******/

	
	topSizer->Add(sectionsSizer);
	topSizer->Add(diceSizer);	
	
	topSizer->SetSizeHints(this);

	topSizer->Layout();
	FindWindow(ID_PANEL)->SetSizerAndFit(topSizer);
	
	topSizer->Fit(this);
	
}

/**
 * Check whether the dice should be rolled or if the user already rolled them.
 * \return true if the dice are valid (rolled).
 */
bool MainFrame::IsValidDice()
{
	if ( m_rolls < 3 )
		return true;
	return false;
}

/**
 *
 */
void MainFrameEvtHandler::OnScoreMouseEnter (wxMouseEvent& event)
{
	int id;
	wxTextCtrl *text_control;
	wxString out;
	if (m_main_frame->m_config->get("score-hints")=="False") {
		event.Skip();
		return;
	}
	if (! m_main_frame->IsValidDice()){
		event.Skip();
		return;
	}
	
	id = ((wxWindow *)event.GetEventObject())->GetId();
	text_control = ((wxTextCtrl*)m_main_frame->FindWindow(id-ID_ACES+ID_ACESTEXT));
	switch (id) {
	case ID_ACES:
		out.Printf(wxT("%i"), m_main_frame->m_score_dice.Aces());
		break;
	case ID_TWOS:
		out.Printf(wxT("%i"), m_main_frame->m_score_dice.Twos());
		break;
	case ID_THREES:
		out.Printf(wxT("%i"), m_main_frame->m_score_dice.Threes());
		break;
	case ID_FOURS:
		out.Printf(wxT("%i"), m_main_frame->m_score_dice.Fours());
		break;
	case ID_FIVES:
		out.Printf(wxT("%i"), m_main_frame->m_score_dice.Fives());
		break;
	case ID_SIXES:
		out.Printf(wxT("%i"), m_main_frame->m_score_dice.Sixes());
		break;
	case ID_THREEOFAKIND:
		out.Printf(wxT("%i"), m_main_frame->m_score_dice.ThreeOfAKind());
		break;
	case ID_FOUROFAKIND:
		out.Printf(wxT("%i"), m_main_frame->m_score_dice.FourOfAKind());
		break;
	case ID_FULLHOUSE:
		out.Printf(wxT("%i"), m_main_frame->m_score_dice.FullHouse());
		break;
	case ID_SMALLSEQUENCE:
		out.Printf(wxT("%i"), m_main_frame->m_score_dice.SmallSequence());
		break;
	case ID_LARGESEQUENCE:
		out.Printf(wxT("%i"), m_main_frame->m_score_dice.LargeSequence());
		break;
	case ID_YAHTZEE:
		out.Printf(wxT("%i"), m_main_frame->m_score_dice.Yahtzee());
		break;
	case ID_CHANCE:
		out.Printf(wxT("%i"), m_main_frame->m_score_dice.Chance());
		break;
	}
	text_control->SetValue(out); 		
	text_control->SetBackgroundColour(wxColour(239,239,239));
	
	event.Skip(); //allow default proccesing
}

/**
 *
 */
void MainFrameEvtHandler::OnScoreMouseLeave (wxMouseEvent& event)
{
	//we dont have the id of the control that generated the event
	wxTextCtrl *text_control;
	
	if (! m_main_frame->IsValidDice()){
		event.Skip();
		return;
	}

	for (int i = ID_ACESTEXT; i<=ID_CHANCETEXT; i++) {
		text_control = (wxTextCtrl *)m_main_frame->FindWindow(i);
		if (text_control->GetBackgroundColour()==wxColour(239,239,239)){
			text_control->Clear();
			text_control->SetBackgroundColour(*wxWHITE);
		}
	}

	event.Skip(); //allow default proccesing
}

MainFrame::~MainFrame() {
	// free pointers
	delete m_config;
	delete m_evt_handler;

	for (int i = 0; i<6; i++) {
		delete bitmap_dice[i];
	}
}
