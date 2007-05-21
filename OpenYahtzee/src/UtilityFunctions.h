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
#include <sstream>
#include <wx/wx.h>
/**\file UtilityFunctions.h
 *\brief General purpuse functions.
 *
 * This file contains the declaration and implementation for some general
 * purpuse utility functions.
*/
/**
 * This function launches the default browser and directs it to a given url.
 *
 * This function extends the wxWidgets default function for this job by checking manually for
 * different browsers if the wxWidgets' function doesn't find one. This is usually necessary 
 * under linux when epiphany isn't installed for some unknown reason.
 * @param link The url that the browser will go to when it stats.
 */
void LaunchBrowser (wxString link){
	if (!wxLaunchDefaultBrowser(link)){
		 //if builtin function doesn't work search for couple of browsers manually. see
		 //http://linux-consulting.buanzo.com.ar/2006/05/wxwidgets-code-to-launch-browser.html

		// variable declarations
		wxArrayString browsers;
		wxPathList path_list;
		bool BrowserWasFound = false;
		unsigned int i = 0;
		wxString path;

		// Add directories to wxPathList's search path from PATH environment variable
		path_list.AddEnvList(wxT("PATH"));
		
		// Add browsers filenames. First item = most priority
		browsers.Add(wxT("firefox"));
		browsers.Add(wxT("firefox-bin"));
		browsers.Add(wxT("mozilla"));
		browsers.Add(wxT("mozilla-bin"));
		browsers.Add(wxT("opera"));
		browsers.Add(wxT("konqueror"));
		browsers.Add(wxT("epiphany"));
		
		for (i = 0; i < browsers.GetCount(); i++) {
			path = path_list.FindAbsoluteValidPath(browsers[i]);
			if (path.IsEmpty()) {
				continue;
			} else {
				BrowserWasFound = true;
				break;
			}
		}
		
		browsers.Clear();
		
		if (BrowserWasFound) {
			path += wxT(" ");
			path += link;
			::wxExecute(path);
		} else {
			wxMessageBox(wxT("No browser has been found."),wxT("OpenYahtzee"));
		}
	}
}
