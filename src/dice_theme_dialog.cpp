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

#include "dice_theme_dialog.h"

#include "Icon.h"

DiceThemeDialog::DiceThemeDialog(wxWindow* parent, int id):
    wxDialog(parent, wxID_ANY, wxT("Dice Theme Selection"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
	SetIcon(wxIcon(ICON));
	
	AddControlsAndLayout();
}

void DiceThemeDialog::AddControlsAndLayout()
{
	wxBoxSizer* top_sizer = new wxBoxSizer(wxVERTICAL);
	wxSizerFlags sizer_flags(1);
	sizer_flags.Expand().Border();
	top_sizer->Add(new wxListCtrl(this, ID_THEMELIST), sizer_flags);
	top_sizer->Add(CreateButtonSizer(wxOK | wxCANCEL), sizer_flags.Proportion(0));

	SetAutoLayout(true);
	SetSizer(top_sizer);
	top_sizer->Fit(this);
	top_sizer->SetSizeHints(this);
	Layout();
}
