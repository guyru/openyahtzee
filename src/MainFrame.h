/***************************************************************************
 *   Copyright (C) 2006 by Guy Rutenberg   *
 *   guy@Guy_Computer   *
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
 *	This File contains the declaration	*
 *	of the class MainFrame			*
 ***********************************************/
#include "SettingsDB.h"
#include "HighScoreTableDB.h"
#ifndef MAINFRAME_INC
#define MAINFRAME_INC

// Declare our main frame class
class MainFrame : public wxFrame
{
public:
	// Constructor
	MainFrame(const wxString& title,  const wxSize& size, long style);

	// Event handlers
	void OnQuit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnNewGame (wxCommandEvent& event);
	void OnShowHighscore (wxCommandEvent& event);
	void OnSettings (wxCommandEvent& event);

	void OnRollButton (wxCommandEvent& event);
	void OnUpperButtons (wxCommandEvent& event);
	void On3ofakindButton (wxCommandEvent& event);
	void On4ofakindButton (wxCommandEvent& event);	
	void OnFullHouseButton (wxCommandEvent& event);
	void OnSmallSequenceButton (wxCommandEvent& event);
	void OnLargeSequenceButton (wxCommandEvent& event);
	void OnYahtzeeButton (wxCommandEvent& event);
	void OnChanceButton (wxCommandEvent& event);

	

private:
	void ClearDiceHash();
	void ResetRolls();
	void YahtzeeBonus();
	void EndofGame();
	void HighScoreHandler(int score);

	//pointers to hold bitmap data for the dices
	wxBitmap *bitmap_dices[6];

	short int dice[5];	//holds the dices score
	short int dicehash[6];	//the dice hash
	short int m_rolls;	//holds how many rolls left
	short int m_numofplaysleft; //holds how many times the user got to score untill the end of the game
	bool m_yahtzee;

	SettingsDB *m_settingsdb; ///handles the settings database
	HighScoreTableDB *m_highscoredb; ///handles the highscore database managment

};
#endif
