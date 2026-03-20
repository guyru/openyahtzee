/***************************************************************************
 *   Copyright (C) 2006-2016 by Guy Rutenberg   *
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


#ifndef OPENYAHTZEE_MAIN_FRAME_INC
#define OPENYAHTZEE_MAIN_FRAME_INC
#include "ScoreDice.h"
#include "configuration.h"
#include <memory>
#include <boost/scoped_ptr.hpp>
#include <wx/wx.h>

namespace main_frame {

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
	MainFrame(const wxString& title,  const wxSize& size = wxDefaultSize, long style = wxDEFAULT_FRAME_STYLE ^ wxRESIZE_BORDER );
	~MainFrame();

	// Event handlers
	void OnQuit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnNewGame (wxCommandEvent& event);
	void OnUndo (wxCommandEvent& event);
	void OnShowHighscore (wxCommandEvent& event);
	void OnSettings (wxCommandEvent& event);
	void OnSendComment (wxCommandEvent& event);
	void OnHelpMenuLink (wxCommandEvent& event);

	/**
	 * Prevent accidental roll of the dice by double clicking the button.
	 * \param event The event that was send to the ::OnRollButton()
	 */
	void DoubleRollLock(wxCommandEvent& event);

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

	void OnScoreMouseEnter (wxMouseEvent& event);
	void OnScoreMouseLeave (wxMouseEvent& event);

	bool IsValidDice();

	ScoreDice m_score_dice;

	boost::scoped_ptr<configuration::Configuration> m_config;

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
	void Relayout();
	void AddMenus();
	void ConnectEventTable();
	void AddControlsAndLayout();

	/**
	 * Adjust the frame size to the specified sizer and set proper
	 * size hints to avoid resizing.
	 */
	void AdjustFrameSize(wxSizer* sizer);

	/**
	 * Check if a click on a score button is valid or should
	 * be ignored.
	 * \returns true if the click is valid and should be scored.
	 */
	bool IsValidClick();

	void OnRollButton ();

	wxStaticBoxSizer *uppersection, *lowersection;
	wxBoxSizer *sectionsSizer;
	wxBoxSizer *diceSizer;

	//pointers to hold bitmap data for the dices
	wxBitmap *bitmap_dice[6];

	short int m_rolls;	//holds how many rolls left
	short int m_numofplaysleft; //holds how many times the user got to score untill the end of the game
	bool m_yahtzee;
	bool m_yahtzeebonus; //tells the undo if there is also an yahtzee bonus to undo

	short int m_lastmove; //stores the ID of the last button pressed.
	short int m_rollsundo; //holds the number of remaining rolls for use with the undo option

	class MainFrameEvtHandler *m_evt_handler;
};

/**
 * This class handles events for windows in MainFrame where it hasn't been
 * possible to use the regular event handler.
 */
class MainFrameEvtHandler : public wxEvtHandler
{
public: 
	MainFrameEvtHandler(MainFrame *main_frame) { m_main_frame = main_frame;}
	void OnScoreMouseEnter (wxMouseEvent& event);
	void OnScoreMouseLeave (wxMouseEvent& event);
private:
	MainFrame *m_main_frame;

};

enum {
	ID_PANEL,
	ID_SHOWHIGHSCORE,
	ID_SETTINGS,
	ID_THEMES,
	ID_HOWTOPLAY,
	ID_FAQ,
	ID_SENDCOMMENT,
	
	ID_ACES,
	ID_TWOS,
	ID_THREES,
	ID_FOURS,
	ID_FIVES,
	ID_SIXES,
	ID_THREEOFAKIND,
	ID_FOUROFAKIND,
	ID_FULLHOUSE,
	ID_SMALLSEQUENCE,
	ID_LARGESEQUENCE,
	ID_YAHTZEE,
	ID_CHANCE,

	ID_ACESTEXT,
	ID_TWOSTEXT,
	ID_THREESTEXT,
	ID_FOURSTEXT,
	ID_FIVESTEXT,
	ID_SIXESTEXT,
	ID_THREEOFAKINDTEXT,
	ID_FOUROFAKINDTEXT,
	ID_FULLHOUSETEXT,
	ID_SMALLSEQUENCETEXT,
	ID_LARGESEQUENCETEXT,
	ID_YAHTZEETEXT,
	ID_CHANCETEXT,
	ID_YAHTZEEBONUSTEXT,

	ID_UPPERSECTIONTOTAL,
	ID_BONUS,
	ID_UPPERTOTAL,
	ID_LOWERTOTAL,
	ID_GRANDTOTAL,
	
	ID_ROLL,
	ID_DICE1,
	ID_DICE2,
	ID_DICE3,
	ID_DICE4,
	ID_DICE5,
	
	ID_DICE1KEEP,
	ID_DICE2KEEP,
	ID_DICE3KEEP,
	ID_DICE4KEEP,
	ID_DICE5KEEP,
};

}
#endif
