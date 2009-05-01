/***************************************************************************
 *   Copyright (C) 2006-2009 by Guy Rutenberg   *
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

#ifndef OPENYAHTZEE_STATISTICS_INC
#define OPENYAHTZEE_STATISTICS_INC

#include "configuration.h"
namespace statistics {

class Statistics {
public:
	Statistics(configuration::Configuration *backend);
	/**
	 * Records that a new game has been started.
	 */
	void game_started();

	/**
	 * Records that a game was ended.
	 * \param score The score the game ended with.
	 */
	void game_finished(int score);

	/**
	 * Resets the statistics data.
	 */
	void reset();

private:
	/**
	 * Saves the current status of the statistics object.
	 */
	void save();

	int games_started;
	int games_finished;
	configuration::Configuration *backend;
};

} //namespace

#endif

