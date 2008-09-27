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

#include <iostream>
#include <cstdlib> // used in the back-compatibility code
#include "configuration.h"
#include "../config.h"

using namespace std;
using namespace configuration;

Configuration::Configuration(string file)
{
	load(file);
}

void Configuration::load(string file)
{
	m_file = file;

	cerr<<"loading file: "<<file<<endl;
	ifstream conf_file (file.c_str());
	if (!conf_file.is_open()) {
		// file couldn't be opened, this is due to missing file or
		// permission error.
		cerr<<"Error openning configuration file for reading: "<<m_file<<endl;
	}
	
	// the file was opened successfully. we need to check if it is an
	// Open Yahtzee configuration file.
	string header;
	getline(conf_file,header);
	cerr<<"line: "<<header.substr(0,11)<<endl;
	if (header.substr(0,11) != "openyahtzee") {
		// this is probably an old sqllite file import it.
		conf_file.close();
		importOldFile();
		return;
	}
	
	while(!conf_file.eof()) {
		getline(conf_file,header);
		cerr<<"line: "<<header<<endl;
		if (header == "[settings]") {
			parseSettings(&conf_file);
		} else if (header == "[highscores]") {
			parseHighscores(&conf_file);
		}
	}

	conf_file.close();
}

void Configuration::parseSettings(ifstream *file)
{
	loadDefaultSettings();
	cerr<<"parsing settings"<<endl;
	string temp_line;
	size_t pos; // used to loacate the '=' sign

	while(!file->eof()) {
		getline(*file,temp_line);
		pos = temp_line.find('=');
		if (pos == string::npos) {
			// this isn't a configuration line.
			// set the get pointer of the file to the beginning
			// of the line
			file->seekg(-1-temp_line.length(),ios_base::cur);
			/* I didn't use ifstream::gcount in the above line
			 * because I used the getline from string.h and not
			 * ifstream::getline
			 */
			return;
		}
		cerr<<"key: "<<temp_line.substr(0,pos)<<endl;
		cerr<<"value: "<<temp_line.substr(pos+1)<<endl;
		m_settings[temp_line.substr(0,pos)] = temp_line.substr(pos+1);
	}
}

void Configuration::parseHighscores(ifstream *file)
{
	char temp_chr;
	size_t pos; // used to loacate the '=' sign
	int score;
	string date,hour,name;
	HighscoreItem temp_item;
	while(file->good()) {
		temp_chr = file->get();
		file->unget();
		if (temp_chr == '[') {
			// we already called unget
			return;
		}
		(*file)>>score;
		(*file)>>date;
		(*file)>>hour;
		(*file).get(); // discard space before name
		getline(*file,name);
		temp_item.score = score;
		temp_item.name = name;
		temp_item.date = date+" "+hour;
		m_highscores.push_back(temp_item);
		cerr<<score<<" "<<name<<" "<<temp_item.date<<endl;

		// The following two lines read one character forword and
		// return it. This is done in order to raise the eofbit if
		// we reached the eof (it is raised only after reading
		// operation failed).
		temp_chr = file->get();
		file->unget();
	}
}

void Configuration::save()
{
	ofstream conf_file (m_file.c_str());
	if (!conf_file.is_open()) {
		// file couldn't be opened, this is due to missing file or
		// permission error.
		cerr<<"Error openning configuration file for writing: "<<m_file<<endl;
	}
	conf_file<<"openyahtzee="<<VERSION<<endl;

	conf_file<<"[settings]\n";
	saveSettings(&conf_file);
	conf_file<<"[highscores]\n";
	saveHighscores(&conf_file);

	conf_file.close();
}

void Configuration::saveSettings(ofstream *file)
{
	map<string,string>::iterator it;
	for (it = m_settings.begin(); it!=m_settings.end(); it++) {
		(*file)<<(*it).first<<"="<<(*it).second<<"\n";
	}
}

void Configuration::saveHighscores(ofstream *file)
{
	HighscoresList::iterator it;
	for (it = m_highscores.begin(); it!=m_highscores.end(); it++) {
		(*file)<<(*it).score<<" ";
		(*file)<<(*it).date<<" ";
		(*file)<<(*it).name<<"\n";
	}
}

string Configuration::get(string key)
{
	map<string,string>::iterator obj;

	obj = m_settings.find(key);
	if (obj == m_settings.end()) {
		// key is missing return empty string
		return "";
	}
	return obj->second;
}

Configuration* Configuration::set(string key, string value)
{
	m_settings[key] = value;
	return this;
}

void Configuration::loadDefaultSettings()
{
	m_settings["dice-animation"] = "True";
	m_settings["calculate-subtotal"] = "True";
	m_settings["horizontal-layout"] = "True";
	m_settings["score-hints"] = "True";
	m_settings["highscore-list-size"] = DEFAULT_HIGHSCORE_SIZE;
}


void Configuration::importOldFile()
{
	loadDefaultSettings();
	if (old_db.Open(m_file)!=SQLITE_OK) {
		return;
	}
	importSettings();
	// importHighscores is called after importSettings() so the
	// highscore size settings will be known
	importHighscores();
}

string Configuration::getKeyFromDb(string key)
{
	string tmp_query;
	std::list<string> tmp_value;
	
	tmp_query = "SELECT value FROM settings WHERE key = \"" + key + "\"";

	tmp_value = old_db.Query(tmp_query);
	if (tmp_value.empty()) { //if there was no result return an empty string
		tmp_query = "";	//use tmp_query for holding a tmp_string
		return tmp_query;
	}
	return	*(tmp_value.begin());
}

void Configuration::importSettings()
{
	string temp_value;

	temp_value = getKeyFromDb("animate");
	if (temp_value=="No") {
		set("dice-animation","False");
	}

	temp_value = getKeyFromDb("calculatesubtotal");
	if (temp_value=="No") {
		set("calculate-subtotal","False");
	}

	temp_value = getKeyFromDb("horizontalayout");
	if (temp_value=="No") {
		set("horizontal-layout","False");
	}

	temp_value = getKeyFromDb("score_hints");
	if (temp_value=="No") {
		set("score-hints","False");
	}

	temp_value = getKeyFromDb("highscoresize");
	if (temp_value=="") {
		set("highscore-list-size",DEFAULT_HIGHSCORE_SIZE);
	}
}

void Configuration::importHighscores()
{
	string tmp_query = "SELECT name,date,score FROM highscore LIMIT "
		+ m_settings["highscore-list-size"];
	
	list<string> res = old_db.Query(tmp_query);
	HighscoreItem temp_item;

	list<string>::iterator it = res.begin();
	size_t pos;
	while(it!=res.end()) {
		temp_item.name = (*it++);

		// the database saved the the hour before the date,
		// we need to reverse this
		pos = it->find(' ');
		temp_item.date = it->substr(pos+1); // date
		temp_item.date += " " + it->substr(0,pos-3); // time, without seconds
		it++;

		temp_item.score = atoi((*it++).c_str());

		m_highscores.push_back(temp_item);
	}
}

bool Configuration::isHighscore(int score) {
	const int highscore_list_size = atoi(m_settings["highscore-list-size"].c_str());
	if (m_highscores.size()<highscore_list_size) {
		// we have extra room in the highscore list
		return true;
	}

	HighscoresList::iterator it= m_highscores.end();
	if ((--it)->score<score) {
		return true;
	}

	return false;
}

int Configuration::submitHighscore(int score, string name, string date) {
	const int highscore_list_size = atoi(m_settings["highscore-list-size"].c_str());
	int place = 1;
	HighscoreItem temp_item;

	//create sentinel in end of list, we'll remove when we finish
	temp_item.score = -1; // this is lower than any valid score
	m_highscores.push_back(temp_item);

	temp_item.score = score;
	temp_item.date = date;
	temp_item.name = name;

	HighscoresList::iterator it = m_highscores.begin();
	while (it!=m_highscores.end()) {
		if (it->score < temp_item.score) {
			m_highscores.insert(it, temp_item);
			break;
		}
		place++;
		it++;
	}
	m_highscores.pop_back(); // remove the sentinel;

	if (m_highscores.size()>highscore_list_size) {
		m_highscores.pop_back();
	}

	if (place>highscore_list_size) {
		return 0;
	}
	return place;
}

const HighscoresList* Configuration::getHighscores() const
{
	const HighscoresList * ptr = &m_highscores;
	return ptr;
}
