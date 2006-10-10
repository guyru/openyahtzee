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
/**\file DBwrapper.h
 *\brief Header file for DBwrapper class.
 *
 * This file contains the header for the DBwrapper class and all the nessecery include files.
*/

/// DBwrapper class - a wrapper for databases
/**
the DBwrapper class is a c++ wrapper for databases. It is designed
to allow to access different kinds of data bases seamlessly
*/

#include <sqlite3.h>
#include <string>
#include <list>

#ifndef DBWRAPPER_INC
#define DBWRAPPER_INC

class DBwrapper {

public:
	DBwrapper(std::string filename);
	DBwrapper();
	virtual ~DBwrapper();

	int Open(std::string filename);
	void Close();

	std::list<std::string> Query(std::string query, const int maxbusy = 20);

protected:
	
private:
	sqlite3* m_db; ///< holds the sqllite database
	char *m_errmsg; ///< holds the sqllite error messages
	int m_errorcode; ///< holds the returned sqllite errorcode
};

#endif
