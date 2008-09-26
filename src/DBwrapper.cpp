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
#include "DBwrapper.h"

#ifdef WIN32
#include <windows.h>
#endif

using namespace std;

#include <iostream>




/**
 * This constructor takes the filename of the database and opens it.
 * \param[in] filename a pointer a UNICODE string which make the filename of the sqllite database file
 * \return none
 */

DBwrapper::DBwrapper(std::string filename)
{
	m_errmsg=0; //set the pointer to null
	db_loaded = false;
	
	m_errorcode = sqlite3_open(filename.c_str(), &m_db);
	
	if (m_errorcode!=SQLITE_OK)
		cerr<<"ERROR: couldn't open database (DBhandler constructor). filename: "<< filename<<endl;
	
	db_loaded = true;

}
/** 
* \brief Default constructor
*
* When using this consturctor the created object should call Open(std::string filename) in order to open the
* datatabase for reading/writing.
*/
DBwrapper::DBwrapper(){
	db_loaded = false;
}

/**
 * The destructor for DBwrapper. The destructor is marked as virtual so derieved classes will be able to use it easily/
 * \return none
 */
DBwrapper::~DBwrapper()
{
	if (db_loaded) {
		sqlite3_close(m_db);
	}
}

/**
 * This function queries the database
 *
 * \param[in] query a unicode string which holds the SQL query 
 * \param[in] maxbusy the number of 0.1 seconds to wait for the database in case it is busy. after this time expires the query will stop excute. defualts to 20.
 * \return a std::list of std::string holding the database query results.
 */
list<string> DBwrapper::Query (std::string query, const int maxbusy)
{
	list<string> values;
	const char* tail;
	sqlite3_stmt* stmt;

#ifdef DEBUG
	cout<<query<<endl;
#endif

	m_errorcode = sqlite3_prepare( m_db, query.c_str(), query.length(), &stmt, &tail );


	if ( m_errorcode != SQLITE_OK )
	{
		cerr << "sqlite3_compile error:" <<endl;
		cerr << sqlite3_errmsg( m_db ) <<endl;
		cerr << "on insert: " << query <<endl;
		values = list<string>::list();
	} else {
		int busyCnt = 0;
		int number = sqlite3_column_count( stmt );
		//execute virtual machine by iterating over rows
		while ( true )
		{
			m_errorcode = sqlite3_step( stmt );
			if ( m_errorcode == SQLITE_BUSY )
			{
				if ( busyCnt++ > maxbusy ) {
					cerr << "Busy-counter has reached maximum. Aborting this sql statement!\n";
					break;
				}
#ifdef WIN32
				Sleep(100);
#else
				::usleep( 100000 ); // Sleep 100 msec
#endif
				cerr << "sqlite3_step: BUSY counter: " << busyCnt << endl;
			}
			
			if ( m_errorcode == SQLITE_MISUSE )
				cerr << "sqlite3_step: MISUSE" << endl;
			if ( m_errorcode == SQLITE_DONE || m_errorcode == SQLITE_ERROR )
				break;

			//iterate over columns
			for ( int i = 0; i < number; i++ )
			{
				values.push_back((string)((char*)sqlite3_column_text( stmt, i)));
			}
			
		}
		//deallocate vm ressources
		sqlite3_finalize( stmt );

		if ( m_errorcode != SQLITE_DONE )
		{
			cerr <<  "sqlite_step error.\n";
			cerr << sqlite3_errmsg( m_db ) << endl;
			cerr << "on query: " << query << endl;
			values = list<string>::list();
        }
    }

	return values;
}

/**
 * Opens a new connection to database file
 * \param filename filename a pointer a UNICODE string which make the filename of the SQLite database file
 * \return The error code returned by sqlite3_open. On success should be SQLITE_OK
 */
int DBwrapper::Open( std::string filename )
{
	db_loaded = false;
	m_errorcode = sqlite3_open(filename.c_str(), &m_db);
	
	if (m_errorcode!=SQLITE_OK)
		cerr<<"ERROR: couldn't open database (DBhandler constructor). filename: "<< filename<<endl;
	
	db_loaded = true;
	return m_errorcode;
}

/**
 * Closes the connection to the database
 */
void DBwrapper::Close()
{
	sqlite3_free(m_errmsg);
	sqlite3_close(m_db);
}
