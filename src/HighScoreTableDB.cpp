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

#include "HighScoreTableDB.h"
#include <sstream>
#include <iostream>

using namespace std;

///Default constructor
HighScoreTableDB::HighScoreTableDB()
{
	/* Open the database which holds the settings. */
	m_path = ((wxFileName::GetHomeDir()).mb_str());
	m_path += "/"; //IMPORTANT! only works under linux/unix/*nix
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
HighScoreTableDB::~HighScoreTableDB()
{
	Close();
}

/**
 * Creates the highscore database table if the table doesn't exist already. 
 *
*/
void HighScoreTableDB::CreateTable()
{
	//Create the database table.
	Query("CREATE TABLE IF NOT EXISTS highscore (place INTEGER PRIMARY KEY, name TEXT, date TEXT, score INTEGER)"); // create the table
	
	//Create the unique index on the 'key' column.
	Query("CREATE UNIQUE INDEX IF NOT EXISTS place ON highscore ( place )");	
}

int HighScoreTableDB::GetSize()
{
	return m_size;
}
void HighScoreTableDB::SetSize(int size)
{
	string tmp_query;
	std::ostringstream sstr;
	
	for(int i = 1; i<=size; i++){
		sstr.str("");
		tmp_query="INSERT OR IGNORE INTO highscore (place,name,score,date) VALUES( ";
		sstr<<i<<flush; //make sure the number realiy gets into the stream
		tmp_query += sstr.str(); 
		tmp_query += ",\"anonymous\", 0, \"never\")";
		Query(tmp_query);
	}
	
	sstr.str("");
	sstr << size<<flush;
	tmp_query = "DELETE FROM highscore WHERE place > " + sstr.str();
	Query(tmp_query);
	
	m_size = size;

}

/**
 * Check if a score qualifies for the highscore table
 *\param score the score to be checked
 *\return highscore position or 0 if doesn't qualify
*/
int HighScoreTableDB::IsHighScore(int score)
{
	list<string> scores;
	int temp_score;
	scores = Query("SELECT score FROM highscore");
	
	list<string>::iterator p = scores.begin();
	int i = 1;
	while (p != scores.end()) {
		temp_score = atoi(p->c_str());
		if(score > temp_score)
			break;
		i++;
		p++;
	}
	if (i<=m_size)
		return i;
	return 0; // the score doesn't qualifiy
}

int HighScoreTableDB::SendHighScore(std::string name, std::string date, int score)
{
	int place;
	std::ostringstream sstr;
	
	std::string tmp_date, tmpswap_date="", tmp_name,tmpswap_name="";
	int tmp_score, tmpswap_score=0;

	place = IsHighScore(score);
	if(!place) //the score doesn't qualify
		return 0;

	string tmp_query;
	list<string> result;
	list<string>::iterator p;

		
	//first move all the rows starting at 'place' one row down
	for (int i = place; i <= m_size ; i++){
		sstr.str("");
		//Get the current score and details of the row and then move them one row below;
		tmp_query="SELECT name,date,score FROM highscore WHERE place=";
		sstr<<i<<flush; //make sure the number realiy gets into the stream
		tmp_query += sstr.str(); 
		result = Query(tmp_query);
		p = result.begin();
		tmp_name=*(p);
		tmp_date=*(++p);
		tmp_score= atoi((++p)->c_str());

		sstr.str("");
		tmp_query="REPLACE INTO highscore (place,name,date,score) VALUES(";
		sstr<<i<<flush; //make sure the number realiy gets into the stream
		tmp_query += sstr.str(); 
		tmp_query += ",";
		tmp_query +="\""+tmpswap_name+"\"" +", " + "\""+tmpswap_date+"\"" + ", ";

		sstr.str("");
		sstr<<tmpswap_score<<flush; //make sure the number realiy gets into the stream
		tmp_query += sstr.str()+")"; 
		result = Query(tmp_query);

		//pass the info to the next item
		tmpswap_score = tmp_score;
		tmpswap_name = tmp_name;
		tmpswap_date = tmp_date;
	}
	//now replace the row at 'place'
	sstr.str("");
	tmp_query="REPLACE INTO highscore (place,name,date,score) VALUES(";
	sstr<<place<<flush; //make sure the number realiy gets into the stream
	tmp_query += sstr.str(); 
	sstr.str("");
	tmp_query += ",";
	tmp_query +="\""+name+"\"" +", " + "\""+date+"\"" + ", ";
	sstr<<score<<flush; //make sure the number realiy gets into the stream
	tmp_query += sstr.str()+")"; 
	result = Query(tmp_query);

	return place;
}


std::list<std::string> HighScoreTableDB::GetHighScoreTable()
{
	std::ostringstream sstr;
	sstr.str("");
	string tmp_query = "SELECT name,date,score FROM highscore LIMIT ";
	sstr<<m_size<<flush;
	tmp_query += sstr.str();
	
	return Query(tmp_query);
}

