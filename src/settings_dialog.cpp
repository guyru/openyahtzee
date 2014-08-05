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

#include <wx/intl.h>
#include "settings_dialog.h"
#include "icon32.xpm"

using namespace settings_dialog;

SettingsDialog::SettingsDialog(wxWindow* parent, configuration::Configuration* config):
    wxDialog(parent, wxID_ANY, _("Settings Dialog"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE)
{

	SetIcon(wxIcon(icon32_xpm));

	m_config = config;

	connectEventTable();
	createControls();
	loadSettings();
	doLayout();
}

void SettingsDialog::createControls()
{
	// note that child windows are automatically deleted by wxWidgets, so
	// need to delete them manually
	animate_checkbox = new wxCheckBox(this, wxID_ANY, _("Dice animation"));
	subtotal_checkbox = new wxCheckBox(this, wxID_ANY, _("Calculate sub-total score for the upper and lower sections"));
	score_hints_checkbox = new wxCheckBox(this, wxID_ANY, _("Display score hints."));
	horizontal_checkbox = new wxCheckBox(this, wxID_ANY, _("Enable horizontal layout for user interface."));
}


void SettingsDialog::connectEventTable()
{
	Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(SettingsDialog::onOK));
}

void SettingsDialog::doLayout()
{
	wxBoxSizer* top_sizer = new wxBoxSizer(wxVERTICAL);
	wxStaticBoxSizer* settings_sizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("General Settings") ), wxVERTICAL);

	settings_sizer->Add(animate_checkbox,0,wxALL,5);
	settings_sizer->Add(subtotal_checkbox,0,wxALL,5);
	settings_sizer->Add(score_hints_checkbox,0,wxALL,5);
	settings_sizer->Add(horizontal_checkbox,0,wxALL,5);

	top_sizer->Add(settings_sizer,0,wxALL,5);

	top_sizer->Add(CreateButtonSizer(wxOK|wxCANCEL), 1, wxBOTTOM, 10);

	SetAutoLayout(true);
	SetSizer(top_sizer);

	top_sizer->Fit(this);
	top_sizer->SetSizeHints(this);

	Layout();
}

void SettingsDialog::loadSettings()
{
	animate_checkbox->SetValue(m_config->get("dice-animation")=="True");
	subtotal_checkbox->SetValue(m_config->get("calculate-subtotal")=="True");
	score_hints_checkbox->SetValue(m_config->get("score-hints")=="True");
	horizontal_checkbox->SetValue(m_config->get("horizontal-layout")=="True");
	
}
void SettingsDialog::onOK(wxCommandEvent& event)
{
	m_config->set("dice-animation",animate_checkbox->GetValue()?"True":"False");
	m_config->set("calculate-subtotal",subtotal_checkbox->GetValue()?"True":"False");
	m_config->set("score-hints",score_hints_checkbox->GetValue()?"True":"False");
	m_config->set("horizontal-layout",horizontal_checkbox->GetValue()?"True":"False");

	m_config->save();

	event.Skip();
}
