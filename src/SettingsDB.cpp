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
#include "SettingsDB.h"
#include <iostream>

using namespace std;

///Default constructor
SettingsDB::SettingsDB()
{
	/* Open the database which holds the settings. */
#ifdef PORTABLE
	m_path = "";
#else
	m_path = ((wxFileName::GetHomeDir()).mb_str());
	m_path += "/";
#endif
	m_path += DBFILENAME;

#ifdef DEBUG
	if (this->Open(m_path)==SQLITE_OK)
		cerr<<"file opened: "<<m_path<<endl;
#else
	this->Open(m_path);
#endif

	/* Initialize the database table */
	CreateTable();

}

/**
\brief Destructor
*
* Closes the connection to the database
*/
SettingsDB::~SettingsDB()
{
	Close();
}

/**
 * Creates the settings database table if the table doesn't exist already. 
 *
*/
void SettingsDB::CreateTable()
{
	//Create the database table.
	Query("CREATE TABLE IF NOT EXISTS settings (key TEXT, value TEXT)"); // create the table
	
	//Create the unique index on the 'key' column.
	Query("CREATE UNIQUE INDEX IF NOT EXISTS key ON settings ( key )");	
}

/**
 * Sets a settings key in the database. If the key doesn't exist it will create it
 *
 *\param key a string which holds the key's name.
 *\param value a string which holds the value which should be assigned to the key
*/

int SettingsDB::SetKey(std::string key, std::string value)
{
	string tmp_query;
	
	//build the replace query	
	tmp_query = "REPLACE INTO settings ( key , value ) VALUES ( \"" + key + "\" , \"" + value + "\" )";
	
	Query(tmp_query);

	return 0;
}

/**
 * Gets a settings key from the database.
 *
 *\param key a string which holds the key's name.
 *\return the value of the requested key or empty string if key doesn't exist
*/

std::string SettingsDB::GetKey(std::string key)
{
	string tmp_query;
	std::list<string> tmp_value;
	
	tmp_query = "SELECT value FROM settings WHERE key = \"" + key + "\"";

	tmp_value = Query(tmp_query);
	if (tmp_value.empty()) { //if there was no result return an empty string
		tmp_query = "";	//use tmp_query for holding a tmp_string
		return tmp_query;
	}
	return	*(tmp_value.begin());
}
