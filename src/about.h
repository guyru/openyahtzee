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

#ifndef OPENYAHTZEE_ABOUT_INC
#define OPENYAHTZEE_ABOUT_INC

#include <wx/wx.h>
#include <wx/image.h>
#include "wxDynamicBitmap.h"

#include <wx/notebook.h>

namespace about {

class AboutDialog: public wxDialog {
public:
    AboutDialog(wxWindow* parent);

private:
    void set_properties();
    void do_layout();

protected:
    wxDynamicBitmap* bitmap_1;
    wxStaticText* app_label;
    wxStaticText* label_desc;
    wxStaticText* label_copyright;
    wxStaticText* label_1;
    wxPanel* notebook_main_pane_about;
    wxStaticText* label_7;
    wxScrolledWindow* notebook_main_pane_authors;
    wxStaticText* label_6;
    wxScrolledWindow* notebook_main_pane_thanks;
    wxStaticText* label_15;
    wxScrolledWindow* notebook_main_pane_license;
    wxNotebook* notebook_main;
    wxButton* close_button;
};

static const wxString OY_URL = wxT("http://www.openyahtzee.org/");

}



#endif // ABOUT_H
