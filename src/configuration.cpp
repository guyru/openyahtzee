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

#include <cstdlib> // used in the back-compatibility code
#include <sstream>
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

	ifstream conf_file (file.c_str());
	if (!conf_file.is_open()) {
		// file couldn't be opened, this is due to missing file or
		// permission error.
		loadDefaultSettings();
		save();
		return;
	}
	
	// the file was opened successfully. we need to check if it is an
	// Open Yahtzee configuration file.
	string header;
	getline(conf_file,header);
	if (header.substr(0,11) != "openyahtzee") {
		/* The file might be an old configuration file or
		 * currupted, anyway re-create it
		 */
		loadDefaultSettings();
		save();
		return;
	}
	
	while(!conf_file.eof()) {
		getline(conf_file,header);
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
	string temp_line;
	size_t pos; // used to loacate the '=' sign

	while(!file->eof()) {
		char temp_chr = file->get();
		file->unget();
		if (temp_chr == '[') { //new section started
			// we already called unget
			return;
		}
		getline(*file,temp_line);
		pos = temp_line.find('=');
		if (pos == string::npos) {
			// this isn't a configuration line, but it isn't a new section
			// as we checked this before, just skip
			continue;
		}
		m_settings[temp_line.substr(0,pos)] = temp_line.substr(pos+1);
	}
}

void Configuration::parseHighscores(ifstream *file)
{
	char temp_chr;
	int score;
	string date,hour,name;
	HighscoreItem temp_item;

	temp_chr = file->get();
	file->unget();

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
		// file couldn't be opened, probably due to permission error
		return;
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

bool Configuration::isHighscore(int score) {
	const unsigned int highscore_list_size = atoi(m_settings["highscore-list-size"].c_str());
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
	const unsigned int highscore_list_size = atoi(m_settings["highscore-list-size"].c_str());
	unsigned int place = 1;
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
	save();
	return place;
}

const HighscoresList* Configuration::getHighscores() const
{
	const HighscoresList * ptr = &m_highscores;
	return ptr;
}

void Configuration::clearHighscores()
{
	m_highscores.clear();
	save();
}

void Configuration::setHighscoresSize(size_t size)
{

	std::ostringstream o;
	o << size;
	m_settings["highscore-list-size"] = o.str();

	while (m_highscores.size()>size) {
		m_highscores.pop_back();
	}

	save();
}
