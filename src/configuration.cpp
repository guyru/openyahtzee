/***************************************************************************
 *   Copyright (C) 2006-2012 by Guy Rutenberg   *
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
#include <iostream>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include "configuration.h"
#include "../config.h"

using namespace std;
using namespace configuration;
using namespace boost;


Configuration::Configuration(string file)
{
	load(file);
}

void Configuration::load(string file)
{
	m_file = file;

	ifstream conf_file(file.c_str());
	if (!conf_file.is_open()) {
		// file couldn't be opened, this is due to missing file or
		// permission error.
		loadDefaultSettings();
		save();
		return;
	}

	lines_container conf_lines;
	string line;
	while(getline(conf_file, line).good()) {
		conf_lines.push_back(line);
	}

	if (conf_file.is_open())
		conf_file.close();
	
	// check it's an Open Yahtzee configuration file
	if (conf_lines.begin()->substr(0,11) != "openyahtzee") {
		/* The file might be an old configuration file or
		 * corrupted, anyway re-create it
		 */
		loadDefaultSettings();
		save();
		return;
	}

	lines_iterator conf_line = conf_lines.begin();
	while (conf_line != conf_lines.end()) {
		if (*conf_line == "[settings]") {
			conf_line = parseSettings(make_iterator_range(conf_line, conf_lines.end()));
		} else if (*conf_line == "[highscores]") {
			conf_line = 
				parseHighscores(make_iterator_range(conf_line, conf_lines.end()));
		} else {
			++conf_line;
		}
	}
}

Configuration::lines_iterator Configuration::parseSettings(iterator_range<Configuration::lines_iterator> range)
{
	loadDefaultSettings();
	auto start = range.begin();
	while (++start != range.end()) {
		// check if we reached the start of a new section
		if (algorithm::starts_with(*start, "[") &&
			algorithm::ends_with(*start, "]"))
			break;
		size_t pos = start->find('=');
		if (pos == std::string::npos)
			continue;

		m_settings[start->substr(0,pos)] = 
			start->substr(pos + 1);
	}
	return start;
}

Configuration::lines_iterator Configuration::parseHighscores(iterator_range<Configuration::lines_iterator> range)
{

	HighscoreItem temp_item;
	auto start = range.begin();

	while (++start != range.end()) {
		// check if we reached the start of a new section
		if (algorithm::starts_with(*start, "[") &&
			algorithm::ends_with(*start, "]"))
			break;
		vector<string> line_parts;
		boost::split(line_parts, *start, boost::is_any_of(" "));
		if (line_parts.size() < 4) {
			// The line has missing parts, ignore it
			cerr << boost::format("%s:%s Couldn't parse: %s") % __FILE__ % __LINE__ % *start << endl;
			continue;
		}
		auto line_parts_itr = line_parts.begin();
		temp_item.score = atoi((line_parts_itr++)->c_str());
		temp_item.date = *line_parts_itr + " " + *(line_parts_itr+1);
		line_parts_itr += 2;
		temp_item.name = algorithm::join(make_iterator_range(line_parts_itr,
		                                                     line_parts.end()),
		                                 " ");

		m_highscores.push_back(temp_item);
	}

	return start;
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
	for (auto &it : m_settings) {
		(*file) << it.first << "=" << it.second << endl;
	}
}

void Configuration::saveHighscores(ofstream *file)
{
	for (auto &it : m_highscores) {
		(*file) << it.score << " ";
		(*file) << it.date << " ";
		(*file) << it.name << endl;
	}
}

string Configuration::get(string key, string default_value)
{
	auto obj = m_settings.find(key);

	if (obj == m_settings.end()) {
		// key is missing return empty string
		return default_value;
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
	if (m_highscores.size() < highscore_list_size) {
		// we have extra room in the highscore list
		return true;
	}

	auto it= m_highscores.end();
	if ((--it)->score < score) {
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
