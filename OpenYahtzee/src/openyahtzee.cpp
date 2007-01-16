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
	DECLARE_APP(MyApp)
	// Give wxWidgets the means to create a MyApp object
	IMPLEMENT_APP(MyApp)
	// Initialize the application

bool MyApp::OnInit()
{

	// Create the main application window
	MainFrame *frame = new MainFrame(wxT("OpenYahtzee"), wxDefaultSize, wxDEFAULT_FRAME_STYLE & (~wxRESIZE_BORDER));
	
	//Show it
	frame->Show(true);	
	return true;
}
