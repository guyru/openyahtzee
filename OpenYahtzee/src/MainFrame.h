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
/**\file MainFrame.h
 *\brief Header file for MainFrame class.
 *
 * This File contains the declaration of the class MainFrame
*/

#include "SettingsDB.h"
#include "HighScoreTableDB.h"
#ifndef MAINFRAME_INC
#define MAINFRAME_INC

DECLARE_EVENT_TYPE(wxEVT_ENABLE_ROLL, -1)

/// MainFrame class - the main window
/**
The Main Frame class is a derieved class from wxFrame which is responsible to 
the main window of the application. This class also hold all the functions 
that process the dice and handles the gameplay. This function are called as
event handlers.
*/
class MainFrame : public wxFrame
{
public:
	// Constructor
	MainFrame(const wxString& title,  const wxSize& size, long style);

	// Event handlers
	void OnQuit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnNewGame (wxCommandEvent& event);
	void OnUndo (wxCommandEvent& event);
	void OnShowHighscore (wxCommandEvent& event);
	void OnSettings (wxCommandEvent& event);
	void OnCheckForUpdates (wxCommandEvent& event);
	void OnSendComment (wxCommandEvent& event);

	void OnRollButton (wxCommandEvent& event);
	void OnUpperButtons (wxCommandEvent& event);
	void On3ofakindButton (wxCommandEvent& event);
	void On4ofakindButton (wxCommandEvent& event);	
	void OnFullHouseButton (wxCommandEvent& event);
	void OnSmallSequenceButton (wxCommandEvent& event);
	void OnLargeSequenceButton (wxCommandEvent& event);
	void OnYahtzeeButton (wxCommandEvent& event);
	void OnChanceButton (wxCommandEvent& event);
	void OnDiceClick (wxCommandEvent& event);
	void OnKeepClick (wxCommandEvent& event);	

private:
	void ClearDiceHash();
	void ResetRolls();
	void YahtzeeBonus();
	bool YahtzeeJoker();
	void EndofGame();
	void HighScoreHandler(int score);
	inline void EnableUndo(int id);
	void PostScore(int id);
	void CalculateSubTotal();
	int InitializeDatabase();

	//pointers to hold bitmap data for the dices
	wxBitmap *bitmap_dices[6];

	short int dice[5];	//holds the dices score
	short int dicehash[6];	//the dice hash
	short int m_rolls;	//holds how many rolls left
	short int m_numofplaysleft; //holds how many times the user got to score untill the end of the game
	bool m_yahtzee;
	bool m_yahtzeebonus; //tells the undo if there is also an yahtzee bonus to undo

	short int m_lastmove; //stores the ID of the last button pressed.
	short int m_rollsundo; //holds the number of remaining rolls for use with the undo option

	SettingsDB *m_settingsdb; //handles the settings database
	HighScoreTableDB *m_highscoredb; //handles the highscore database managment

	bool m_animate; //sets whether to animate the dice.
	bool m_calculatesubtotal; //sets whether to calculate the subtotal after every score or not

};
#endif
