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
#ifndef OPENYAHTZEE_HIGHSCORES_DIALOG_INC
#define OPENYAHTZEE_HIGHSCORES_DIALOG_INC

#include "configuration.h"
#include <wx/wx.h>
#include <wx/dialog.h>
#include <wx/listctrl.h>
#include <wx/spinctrl.h>

namespace highscores_dialog {

class HighscoresDialog : public wxDialog
{
public:
	HighscoresDialog(wxWindow* parent, configuration::Configuration* config, int highlight_rank = 0);

	void onClose(wxCommandEvent& event);	
	void onClear(wxCommandEvent& event);	
	void onConfigure(wxCommandEvent& event);	
	void onResize(wxSizeEvent& event);
private:
	void createControls();
	void loadData( );
	void doLayout();
	void connectEventTable();

	int highlight_rank;
	configuration::Configuration* m_config;

	wxListCtrl *highscoreslist;
};


class HighscoresSettingsDialog : public wxDialog
{
public:
	HighscoresSettingsDialog(wxWindow* parent, configuration::Configuration* config);

	void onOk(wxCommandEvent& event);
private:
	void doLayout();
	wxSpinCtrl *spin_ctrl;

	configuration::Configuration* m_config;
};


enum {
	ID_CONFIGURE,
};

}


#endif
