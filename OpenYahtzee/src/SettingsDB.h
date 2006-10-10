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
/**\file SettingsDB.h
 *\brief Header file for SettingsDB class.
 *
 * This file contains the header for the SettingsDB class and all the nessecery include files.
*/

/// SettingsDB - a settings managemnet class built on top of a DB.
/**
SettingsDB is designed to handle all the settings managemnet in a database file. This will be pretty generic class with some specific
setting for the class in the beginning of it's definitions.
*/

#include "DBwrapper.h"
#include "wx/filename.h"


#ifndef SETTINGSDB_INC
#define SETTINGSDB_INC

///Sets the file name of the database the class will open. The directory will always be the home dir.
#define DBFILENAME ".OpenYahtzee" 
class SettingsDB : public DBwrapper
{
public:
	SettingsDB();
	~SettingsDB();
	std::string GetKey(std::string key);
	int SetKey(std::string key, std::string value);
private:
	std::string m_path;
	void CreateTable();
};


#endif
