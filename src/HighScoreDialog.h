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
#ifndef HIGHSCOREDIALOG_INC
#define HIGHSCOREDIALOG_INC

#include "HighScoreTableDB.h"

class HighScoreDialog : public wxDialog
{
public:
	HighScoreDialog(wxWindow* parent,wxWindowID id,HighScoreTableDB* highscoredb);

private:

};

class HighScoreInfo : public wxDialog 
{
public:
	HighScoreInfo(wxWindow* parent,int place);
	wxString GetName();
};

#endif
