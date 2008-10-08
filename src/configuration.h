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

#include "DBwrapper.h" // used in the back-compatibility code
#include <string>
#include <fstream>
#include <map>
#include <list>

namespace configuration {

struct HighscoreItem;

typedef std::list<HighscoreItem> HighscoresList;

static const char* DEFAULT_HIGHSCORE_SIZE = "20";

class Configuration {
public:
	Configuration(std::string file);
	/**
	 * Reads Open Yahtzee configurations file and parses it.
	 */
	void load(std::string file);

	/**
	 * Saves the configurations (setting and highscores) to the file
	 * specified when the class was constructed
	 */
	void save();

	/**
	 * \return the value associated with the specified key, or empty
	 * string if that key is missing
	 */
	std::string get(std::string key);
	/**
	 * Associates the given value with the given key. You must call
	 * save() in order to make the change permanent.
	 * \return pointer to self.
	 */
	Configuration *set(std::string key, std::string value);

	/**
	 * Checks if a given score quallifies to the high score list
	 */
	bool isHighscore(int score);
	
	/**
	 * Adds a given entry to the highscore list.
	 * \return the place in the highscore list for the new entry, or
	 * zero if it didn't quallify.
	 */
	int submitHighscore(int score, std::string name, std::string date);

	const HighscoresList* getHighscores() const;

	void clearHighscores();

	void setHighscoresSize(size_t size);
private:
	/**
	 * Imports old style configuration from sqllite database.
	 */
	void importOldFile();

	void parseSettings(std::ifstream *file);
	void parseHighscores(std::ifstream *file);

	void saveSettings(std::ofstream *file);
	void saveHighscores(std::ofstream *file);

	/**
	 * Loads default settings
	 */
	void loadDefaultSettings();
	
	std::map<std::string, std::string> m_settings;
	HighscoresList m_highscores;

	std::string m_file;

	/* The following functions and variables are used for legacy SQLite
	 * configuration file, and could be dropped.
	 */
	DBwrapper old_db;
	std::string getKeyFromDb(std::string key);
	void importSettings();
	void importHighscores();
};

struct HighscoreItem {
	int score;
	std::string name;
	std::string date;
};

}
	

#endif
