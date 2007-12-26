// $Header$
/***************************************************************************
 *   Copyright (C) 2006-2007 by Guy Rutenberg   *
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
#include <wx/spinctrl.h>
#include "ObjectsID.h"


#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

struct SettingsDialogData {
	int highscoresize;
	bool reset;
	bool animate;
	bool subtotal;
	bool horizontal;
	bool score_hints;
};

class SettingsDialog: public wxDialog {
public:
	SettingsDialog(wxWindow* parent, int id);
	
	SettingsDialogData GetData();
	void SetData(SettingsDialogData data);
	
	void OnResetHighScore(wxCommandEvent& event);
private:
	void DoLayout();
	void ConnectEventTable();
	


protected:
	wxStaticText* label_1;
	wxSpinCtrl* spin_ctrl;
	wxCheckBox* checkbox_reset; //the reset button
	
	wxCheckBox* animate_checkbox;
	wxCheckBox* subtotal_checkbox;
	wxCheckBox* score_hints_checkbox;
	wxCheckBox* horizontal_checkbox;
	
};


#endif // SETTINGSDIALOG_H
