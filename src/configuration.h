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

#ifndef OPENYAHTZEE_CONFIGURATION_INC
#define OPENYAHTZEE_CONFIGURATION_INC

#include <string>
#include <fstream>
#include <map>
#include <list>

namespace configuration {

struct HighscoreItem;

typedef std::list<HighscoreItem> HighscoreList;

class Configuration {
public:
	Configuration(std::string file);
	/**
	 * Reads Open Yahtzee configurations file and parses it.
	 */
	void load(std::string file);

	void save();
private:
	/**
	 * Imports old style configuration from sqllite database.
	 */
	void importOldFile();

	void parseSettings(std::ifstream *file);
	void parseHighscores(std::ifstream *file);

	void saveSettings(std::ofstream *file);
	void saveHighscores(std::ofstream *file);
	
	std::map<std::string, std::string> m_settings;
	HighscoreList m_highscores;

	std::string m_file;
};

struct HighscoreItem {
	int score;
	std::string name;
	std::string date;
};

}
	

#endif
