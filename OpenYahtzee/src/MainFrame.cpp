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

// #define DEBUG

#include <wx/wx.h>

#include "MainFrame.h"
#include "wxDynamicBitmap.h"
#include "ObjectsID.h"
#include "HighScoreDialog.h"
#include "SettingsDialog.h"
#include "About.h"
#include "UtilityFunctions.h"
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

//default values - settings
#define DEF_HIGHSCORESIZE 20
#define OY_VERSION "1.8.0"

DEFINE_EVENT_TYPE(wxEVT_ENABLE_ROLL)


MainFrame::MainFrame(const wxString& title, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_FRAME_STYLE)
        : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, size, style)
{
	//give the frame an icon
	SetIcon(wxIcon(ICON));


	std::ostringstream sstr;

	m_settingsdb = new SettingsDB(); //Get the settings database connection
	m_highscoredb = new HighScoreTableDB();
	
	InitializeDatabase();//this must come _after_ m_settingsdb and m_highscoredb are created

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
	helpMenu->Append(ID_CHECK_FOR_UPDATES, wxT("&Check for Updates"),
			wxT("Check for new version of the game via the web"));
	helpMenu->AppendSeparator();
	helpMenu->Append(ID_SENDCOMMENT, wxT("&Send a Comment to Developers"),
			wxT("Send a comment to the developers of the game"));
	helpMenu->AppendSeparator();
	helpMenu->Append(wxID_ABOUT, wxT("&About...\tF1"),
			wxT("Show about dialog"));

	//insert menu items into menu File
	gameMenu->Append(ID_NEWGAME,wxT("&New Game\tF2"),wxT("Start a new game"));
	//create the undo button and make it disabled
	gameMenu->Append(ID_UNDO,wxT("&Undo\tCTRL+Z"),wxT("Undo the last move"));
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

	wxBoxSizer *topSizer;
	wxFlexGridSizer *diceSizer;

	if (m_horizontallayout) {
		topSizer = new wxBoxSizer( wxVERTICAL );
		sectionsSizer = new wxBoxSizer( wxHORIZONTAL );
		diceSizer = new wxFlexGridSizer(2, 0, 0, 0);
	} else {
		topSizer = new wxBoxSizer( wxHORIZONTAL );
		sectionsSizer = new wxBoxSizer( wxVERTICAL );
		diceSizer = new wxFlexGridSizer(1, 0, 0);;
	}

	uppersection = new wxStaticBoxSizer( new wxStaticBox( panel, wxID_ANY, wxT("Upper Section") ), wxVERTICAL);
	
lowersection = new wxStaticBoxSizer( new wxStaticBox( panel, wxID_ANY, wxT("Lower Section") ), wxVERTICAL);
	
	wxFlexGridSizer* uppergrid = new wxFlexGridSizer(2, 0, 10);
	wxFlexGridSizer* lowergrid = new wxFlexGridSizer(2, 0, 10);

	//BEGIN layout for the upper section of the score board
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
	//END layout for the upper section of the score board

	//BEGIN layout for the lower section of the score board
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
	lowergrid->Add(new wxTextCtrl(panel, ID_YAHTZEEBONUSTEXT),1,wxALL,SPACE_SIZE);
	lowergrid->Add(new wxStaticText(panel, wxID_ANY, wxT("Total of lower section:")),0,wxALL,SPACE_SIZE);
	lowergrid->Add(new wxTextCtrl(panel, ID_LOWERTOTAL),1,wxALL,SPACE_SIZE);
	lowergrid->Add(new wxStaticText(panel, wxID_ANY, wxT("Grand Total:")),0,wxALL,SPACE_SIZE);
	lowergrid->Add(new wxTextCtrl(panel, ID_GRANDTOTAL),1,wxALL,SPACE_SIZE);
	//END layout for the lower section of the score board

	uppersection->Add(uppergrid);
	lowersection->Add(lowergrid);
	sectionsSizer->Add(uppersection,0,wxALL,5);
	sectionsSizer->Add(lowersection,0,wxALL,5);

	//BEGIN layout for the dice section of the score board
	if (m_horizontallayout) {
		diceSizer->Add(new wxDynamicBitmap(panel,ID_DICE1,*bitmap_dices[0]),0,wxALL,DICE_SPACE);
		diceSizer->Add(new wxDynamicBitmap(panel,ID_DICE2,*bitmap_dices[1]),0,wxALL,DICE_SPACE);
		diceSizer->Add(new wxDynamicBitmap(panel,ID_DICE3,*bitmap_dices[2]),0,wxALL,DICE_SPACE);
		diceSizer->Add(new wxDynamicBitmap(panel,ID_DICE4,*bitmap_dices[3]),0,wxALL,DICE_SPACE);
		diceSizer->Add(new wxDynamicBitmap(panel,ID_DICE5,*bitmap_dices[4]),0,wxALL,DICE_SPACE);
		diceSizer->Add(new wxButton(panel, ID_ROLL, wxT("Roll!"),wxDefaultPosition,wxSize(64,64)),0,wxALL,DICE_SPACE);
		diceSizer->Add(new wxCheckBox(panel, ID_DICE1KEEP, wxT("Keep")),0,wxBOTTOM | wxLEFT,KEEP_SPACE);
		diceSizer->Add(new wxCheckBox(panel, ID_DICE2KEEP, wxT("Keep")),0,wxBOTTOM | wxLEFT,KEEP_SPACE);
		diceSizer->Add(new wxCheckBox(panel, ID_DICE3KEEP, wxT("Keep")),0,wxBOTTOM | wxLEFT,KEEP_SPACE);
		diceSizer->Add(new wxCheckBox(panel, ID_DICE4KEEP, wxT("Keep")),0,wxBOTTOM | wxLEFT,KEEP_SPACE);
		diceSizer->Add(new wxCheckBox(panel, ID_DICE5KEEP, wxT("Keep")),0,wxBOTTOM | wxLEFT,KEEP_SPACE);
	} else {
		diceSizer->Add(new wxDynamicBitmap(panel,ID_DICE1,*bitmap_dices[0]),0,wxALL,DICE_SPACE);
		diceSizer->Add(new wxCheckBox(panel, ID_DICE1KEEP, wxT("Keep")),0,wxLEFT,KEEP_SPACE);
		diceSizer->AddSpacer(VER_DICE_SPACER);
		diceSizer->Add(new wxDynamicBitmap(panel,ID_DICE2,*bitmap_dices[1]),0,wxALL,DICE_SPACE);
		diceSizer->Add(new wxCheckBox(panel, ID_DICE2KEEP, wxT("Keep")),0,wxLEFT,KEEP_SPACE);
		diceSizer->AddSpacer(VER_DICE_SPACER);
		diceSizer->Add(new wxDynamicBitmap(panel,ID_DICE3,*bitmap_dices[2]),0,wxALL,DICE_SPACE);
		diceSizer->Add(new wxCheckBox(panel, ID_DICE3KEEP, wxT("Keep")),0,wxLEFT,KEEP_SPACE);
		diceSizer->AddSpacer(VER_DICE_SPACER);
		diceSizer->Add(new wxDynamicBitmap(panel,ID_DICE4,*bitmap_dices[3]),0,wxALL,DICE_SPACE);
		diceSizer->Add(new wxCheckBox(panel, ID_DICE4KEEP, wxT("Keep")),0,wxLEFT,KEEP_SPACE);
		diceSizer->AddSpacer(VER_DICE_SPACER);
		diceSizer->Add(new wxDynamicBitmap(panel,ID_DICE5,*bitmap_dices[4]),0,wxALL,DICE_SPACE);
		diceSizer->Add(new wxCheckBox(panel, ID_DICE5KEEP, wxT("Keep")),0,wxLEFT,KEEP_SPACE);
		diceSizer->AddSpacer(VER_DICE_SPACER);
		diceSizer->Add(new wxButton(panel, ID_ROLL, wxT("Roll!"),wxDefaultPosition,wxSize(VERTICAL_ROLL_SIZEX,VERTICAL_ROLL_SIZEY)),0,wxALL,DICE_SPACE);
	}
	//END layout for the dice section of the score board *******/

	
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
	
	//BEGIN connecting the menu items' events
	Connect(wxID_EXIT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnQuit));
	Connect(wxID_ABOUT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnAbout));
	Connect(ID_CHECK_FOR_UPDATES, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnCheckForUpdates));
	Connect(ID_SENDCOMMENT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnSendComment));
	Connect(ID_NEWGAME, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnNewGame));
	Connect(ID_UNDO, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnUndo));
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

	/*** End of Event Table ***/

	ResetRolls();
	ClearDiceHash();
	m_yahtzee = false;
	m_yahtzeebonus = false;
	m_numofplaysleft = 13;

 }

/*********EVENT PROCCESSING FUNCTIONS********/


void MainFrame::OnAbout(wxCommandEvent& event)
{
	AboutDialog *about = new AboutDialog(this,wxID_ANY,wxT("About Open Yahtzee"));
	about->ShowModal();
}

/**
 * Connects to the Open Yahtzee website on sourceforge.net and checks for 
 * updates to the game.
 * \param event 
 */
void MainFrame::OnCheckForUpdates (wxCommandEvent& event)
{
	
	wxString link = wxT("http://openyahtzee.sourceforge.net/update.php?version=");
	
	link += wxT(OY_VERSION);

	LaunchBrowser(link);
}


/**
 * Starts up the user browser and connects to the Open Yahtzee website's
 * feedback page.
 * \param event 
 */
void MainFrame::OnSendComment (wxCommandEvent& event)
{
	wxString link = wxT("http://openyahtzee.sourceforge.net/feedback.php");
	
	LaunchBrowser(link);
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
	(GetMenuBar()->FindItem(ID_UNDO))->Enable(false);
	
	ResetRolls();
	ClearDiceHash();
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

	(GetMenuBar()->FindItem(ID_UNDO))->Enable(false);
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
		(GetMenuBar()->FindItem(ID_UNDO))->Enable(true);
		m_lastmove = id;
	}
}

/**
 * Shows the high-score dialog. Connected to the Game->"Show High Score" menu item.
 * \param event 
 */
void MainFrame::OnShowHighscore(wxCommandEvent& event)
{
	HighScoreDialog *dialog = new HighScoreDialog(this,wxID_ANY,m_highscoredb);
	dialog->ShowModal();
}

/**
 * Shows the settings dialog. This event-handler is connected to Game->Settings
 * menu item.
 * \param event 
 */
void MainFrame::OnSettings( wxCommandEvent& event)
{
	SettingsDialog *dialog = new SettingsDialog(this,wxID_ANY);
	SettingsDialogData data;
	std::ostringstream sstr;
	
	data.highscoresize = m_highscoredb->GetSize();

	data.animate = (m_settingsdb->GetKey("animate")=="Yes")?true:false;
	data.subtotal = (m_settingsdb->GetKey("calculatesubtotal")=="Yes")?true:false;
	data.horizontal = (m_settingsdb->GetKey("horizontallayout")=="Yes")?true:false;
	
	dialog->SetData(data);
	if(dialog->ShowModal()==wxID_OK) { //user saved Changes
		data = dialog->GetData();

		if(data.reset)
			m_highscoredb->SetSize(0);
		
		sstr<<data.highscoresize<<std::flush;
		m_settingsdb->SetKey("highscoresize",sstr.str());
				
		m_highscoredb->SetSize(data.highscoresize);
		
		if (data.animate){
			m_settingsdb->SetKey("animate","Yes");
			m_animate = true;
			
		} else {
			m_settingsdb->SetKey("animate","No");
			m_animate = false;
		}
		if (data.subtotal){
			m_settingsdb->SetKey("calculatesubtotal","Yes");
			m_calculatesubtotal = true;
			
		} else {
			m_settingsdb->SetKey("calculatesubtotal","No");
			m_calculatesubtotal = false;
		}

		if (data.horizontal){
			m_settingsdb->SetKey("horizontallayout","Yes");
			m_horizontallayout = true;
			Relayout();			
		} else {
			m_settingsdb->SetKey("horizontallayout","No");
			m_horizontallayout = false;
			Relayout();
		}

	}
}

/**
 * Event handler for the Roll button. It checks the settings if it should
 * animate the dice and then rolls them accordingly. It also checks for the
 * status of the "keep" checkboxes.
 * \param event 
 */
void MainFrame::OnRollButton (wxCommandEvent& event)
{
	if (event.GetEventType() == wxEVT_IDLE) {
		m_skiproll = false;
		Disconnect(wxEVT_IDLE,  wxCommandEventHandler(MainFrame::OnRollButton));
		return;
	}
	
	//skip rolling the dice if the user accidently rolled the dice before they finished spinning.
	if (m_skiproll) return;
	m_skiproll = true;

	//roll the dice...
	if (m_animate) {
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
					dice[i] = rand()%6;
					((wxDynamicBitmap*) FindWindow(i + ID_DICE1)) -> SetBitmap(*bitmap_dices[dice[i]]);
				}
			}
			::wxMilliSleep(100);
		}
	} else {
		for (int i=0; i<5; i++) {
			if (!((wxCheckBox*) FindWindow(i + ID_DICE1KEEP))->IsChecked()) {
				dice[i] = rand()%6;
				((wxDynamicBitmap*) FindWindow(i + ID_DICE1)) -> SetBitmap(*bitmap_dices[dice[i]]);
			}
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
	int temp;
	if(m_rolls < 3){
		YahtzeeBonus();
		temp = dicehash[(event.GetId() - ID_ACES)] * (event.GetId() - ID_ACES + 1);
	
		out.Printf(wxT("%i"),temp);
		((wxTextCtrl*) FindWindow(event.GetId() - ID_ACES + ID_ACESTEXT))->SetValue(out);
		
		PostScore(event.GetId());
	}
	else 
		wxMessageBox(wxT("First you need to roll, and after you roll you may score"), wxT("OpenYahtzee"), wxOK | wxICON_INFORMATION, this);
}

/**
 * Event handler for the "3 of a kind" score button.
 * \param event 
 */
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
	
	PostScore(event.GetId());
}

/**
 * Event handler for the "4 of a kind" score button.
 * \param event 
 */
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
	
	PostScore(event.GetId());
}

/**
 * Event handler for the full-house score button.
 * \param event 
 */
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
	if ((two && three) || YahtzeeJoker())
		((wxTextCtrl*) FindWindow(ID_FULLHOUSETEXT))->SetValue(wxT("25"));
	else
		((wxTextCtrl*) FindWindow(ID_FULLHOUSETEXT))->SetValue(wxT("0"));
	
	PostScore(event.GetId());
}

/**
 * Event handler for the small-sequence score button.
 * \param event 
 */
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
	if (sequence || YahtzeeJoker())
		((wxTextCtrl*) FindWindow(ID_SMALLSEQUENCETEXT))->SetValue(wxT("30"));
	else
		((wxTextCtrl*) FindWindow(ID_SMALLSEQUENCETEXT))->SetValue(wxT("0"));
	
	PostScore(event.GetId());
}

/**
 * Event handler for the large-sequence score button.
 * \param event 
 */
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
	if (sequence || YahtzeeJoker())
		((wxTextCtrl*) FindWindow(ID_LARGESEQUENCETEXT))->SetValue(wxT("40"));
	else
		((wxTextCtrl*) FindWindow(ID_LARGESEQUENCETEXT))->SetValue(wxT("0"));
	
	PostScore(event.GetId());
}

/**
 * Event handler for the yahtzee score button.
 * \param event 
 */
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
		for(int i = 0; i<5; i++) 
			temp += dice[i]+1;
	
		out.Printf(wxT("%i"),temp);
		((wxTextCtrl*) FindWindow(ID_CHANCETEXT))->SetValue(out);
		
		PostScore(event.GetId());
	}
	else 
		wxMessageBox(wxT("First you need to roll, and after you roll you may score"), wxT("OpenYahtzee"), wxOK | wxICON_INFORMATION, this);

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
 * This function clears the dice hash.
 *
 * The dice hash is just an array that holds how many dices have each value.
 */
void MainFrame::ClearDiceHash()
{
	for (int i=0; i<6; i++)
		dicehash[i] = 0;
}

/**
 * This function handles everything related to reseting the dice rolls after scoring.
 */
void MainFrame::ResetRolls()
{
	m_rollsundo = m_rolls;
	m_rolls = 3;
	((wxButton*) FindWindow(ID_ROLL)) -> Enable(true);
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
	if ((dice[0]==dice[1]) && (dice[1]==dice[2]) && (dice[1]==dice[3]) && (dice[1]==dice[4])) {
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
	if ((dice[0]==dice[1]) && (dice[1]==dice[2]) && (dice[1]==dice[3]) && (dice[1]==dice[4]) && !(FindWindow(ID_ACES+dice[0])->IsEnabled()) && !(FindWindow(ID_YAHTZEE)->IsEnabled())) {
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


	place = m_highscoredb->IsHighScore(score);
	
	if(!place)  //if the score didn't make it to the highscore table do nothing
		return;
	
	wxString msg;
	msg.Printf(wxT("Your score made it to the high score table. Your place is number %i.\nPlease enter your name below:"),place);

	wxTextEntryDialog infodialog(this,msg,wxT("Please enter your name"),wxT(""),wxOK | wxCENTRE);
	infodialog.ShowModal();

	name = infodialog.GetValue().mb_str();

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
	if (!m_calculatesubtotal)
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

	for (int i = ID_THREEOFAKINDTEXT; i<=ID_YAHTZEEBONUSTEXT; i++) {
		tempstr = ((wxTextCtrl*) FindWindow(i)) -> GetValue();
		tempstr.ToLong(&temp,10);
		lowerscore +=temp;
	}
	
	tempstr.Printf(wxT("%i"),lowerscore);
	((wxTextCtrl*) FindWindow(ID_LOWERTOTAL)) -> SetValue(tempstr);
}


/**
 * Initializes the database and stores default settings if needed.
 * @return 0 if some error
 */
int MainFrame::InitializeDatabase()
{
	std::ostringstream sstr;


	if (m_settingsdb->GetKey("highscoresize") == "") { //check if we need to create a new high score table
		m_highscoredb->SetSize(DEF_HIGHSCORESIZE);
		sstr<<DEF_HIGHSCORESIZE<<std::flush;
		m_settingsdb->SetKey("highscoresize", sstr.str());
	} else {
		int highscoresize = atoi((m_settingsdb->GetKey("highscoresize")).c_str());
		//m_highscoredb->SetSize((highscoresize>0)?highscoresize:DEF_HIGHSCORESIZE);
		m_highscoredb->SetSize(highscoresize);
	}
	
	if (m_settingsdb->GetKey("animate") == "Yes") {
		m_animate = true;
	} else if (m_settingsdb->GetKey("animate") == "No") {
		m_animate = false;
	} else {
		m_settingsdb->SetKey("animate", "Yes");
		m_animate = true;
	}

	if (m_settingsdb->GetKey("calculatesubtotal") == "Yes") {
		m_calculatesubtotal = true;
	} else if (m_settingsdb->GetKey("calculatesubtotal") == "No") {
		m_calculatesubtotal = false;
	} else {
		m_settingsdb->SetKey("calculatesubtotal", "Yes");
		m_calculatesubtotal = true;
	}

	if (m_settingsdb->GetKey("horizontallayout") == "Yes") {
		m_horizontallayout = true;
	} else if (m_settingsdb->GetKey("horizontallayout") == "No") {
		m_horizontallayout = false;
	} else {
		m_settingsdb->SetKey("horizontallayout", "No");
		m_horizontallayout = false;
	}
	if (m_settingsdb->GetKey("openyahtzeehomepage") == "") {
		m_settingsdb->SetKey("openyahtzeehomepage", "http://openyahtzee.sourceforge.net/");
	}

	if (m_settingsdb->GetKey("updateurl") == "") {
		/*the version string will be appended in the end of the
		  given url */
		m_settingsdb->SetKey("updateurl", "http://openyahtzee.sourceforge.net/update.php?version=");
	}
	
	return 1;
}

void MainFrame::Relayout()
{
	wxBoxSizer *topSizer;
	wxFlexGridSizer *diceSizer;

	lowersection->GetStaticBox()->Destroy();
	uppersection->GetStaticBox()->Destroy();
	sectionsSizer->Remove(lowersection);
	sectionsSizer->Remove(uppersection);

	if (m_horizontallayout) {
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
	if (m_horizontallayout) {
		FindWindow(ID_ROLL)->Destroy();
		new wxButton(FindWindow(ID_PANEL), ID_ROLL, wxT("Roll!"),wxDefaultPosition,wxSize(64,64));
	} else {
		FindWindow(ID_ROLL)->Destroy();
		new wxButton(FindWindow(ID_PANEL), ID_ROLL, wxT("Roll!"),wxDefaultPosition,wxSize(VERTICAL_ROLL_SIZEX,VERTICAL_ROLL_SIZEY));
	}
	
	//BEGIN layout for the dice section of the score board
	if (m_horizontallayout) {
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

