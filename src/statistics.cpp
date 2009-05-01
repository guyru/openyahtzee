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

#include <string>
#include <cstdlib>
#include "statistics.h"
#include "utility.h"

using namespace std;
using namespace statistics;

Statistics::Statistics(configuration::Configuration *backend)
{
	string tmp;
	this->backend = backend;
	
	tmp = backend->get("statistics_games_started");
	games_started = atoi(tmp.c_str());

	tmp = backend->get("statistics_games_finished");
	games_finished = atoi(tmp.c_str());
	
}

void Statistics::game_started()
{	
	games_started++;
	save();
}

void Statistics::game_finished(int score)
{	
	games_finished++;
	save();
}

void Statistics::save() {
	backend->set("statistics_games_started", stringify(games_started));
	backend->set("statistics_games_finished", stringify(games_finished));

	backend->save();
}

void Statistics::reset() {
	backend->set("statistics_games_started", 0);
	backend->set("statistics_games_finished", 0);

	backend->save();
}
