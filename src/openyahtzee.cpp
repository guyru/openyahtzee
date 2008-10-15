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


// This is the main source file for the project. It includes the creation of the main window
// but all other stuff is done on other files.

/*
PREFIX and DATA_DIR are passed by the make file to the program and hold the the
path prefix and datadir path accordingly.
*/

/*
 * If PORTABLE is defined, Open Yahtzee will be compiled for the Portable Edition. Add "-DPORTABLE" to the CXXFLAGS when compiling.
 */

#include "wx/wx.h"
#include "MainFrame.h"
// #ifdef WIN32 
// 	#include openyahtzee.rc
// #endif


// Declare the application class
class MyApp : public wxApp 
{
public:
	// Called on application startup
	virtual bool OnInit();
};



	// Implements MyApp& GetApp()
	DECLARE_APP(MyApp) /*just to satisfy kdevelop*/;
	// Give wxWidgets the means to create a MyApp object
	IMPLEMENT_APP(MyApp) /*just to satisfy kdevelop*/;
	// Initialize the application

bool MyApp::OnInit()
{
	//load all image handlers
	::wxInitAllImageHandlers();

	// Create the main application window
	main_frame::MainFrame *frame = new main_frame::MainFrame(wxT("Open Yahtzee"), wxDefaultSize, wxDEFAULT_FRAME_STYLE & (~wxRESIZE_BORDER));
	
	//Show it
	frame->Show(true);	
	return true;
}
