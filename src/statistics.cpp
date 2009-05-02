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
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include "statistics.h"
#include "utility.h"

using namespace std;
using namespace statistics;
using namespace boost;

Statistics::Statistics(configuration::Configuration *backend)
{
	string tmp;
	vector<string> tmp_vec;
	this->backend = backend;
	
	tmp = backend->get("statistics_games_started");
	games_started = atoi(tmp.c_str());

	tmp = backend->get("statistics_games_finished");
	games_finished = atoi(tmp.c_str());

	tmp = backend->get("statistics_score_distribution");
	split(tmp_vec, tmp, is_any_of(","));
	if (tmp_vec.size() != score_distributions_slots) {
		reset();
		return;
	}

	for (vector<string>::iterator iter = tmp_vec.begin();
		iter != tmp_vec.end(); iter++) {
		score_distribution.push_back(atoi(iter->c_str()));
	}
	
}

void Statistics::game_started()
{	
	games_started++;
	save();
}

void Statistics::game_finished(int score)
{	
	int score_slot;
	games_finished++;

	score_slot = score/score_distribution_granuality;
	score_slot = score_slot<score_distributions_slots ? score_slot : score_slot;
	score_distribution[score_slot]++;
	save();
}

void Statistics::save() {
	string tmp;
	backend->set("statistics_games_started", stringify(games_started));
	backend->set("statistics_games_finished", stringify(games_finished));

	tmp = "";
	for (vector<int>::iterator iter = score_distribution.begin();
		iter != score_distribution.end(); iter++) {
		tmp += stringify(*iter) + ",";
	}
	//delete trailing comma
	tmp = tmp.substr(0, tmp.size()-1);
	backend->set("statistics_score_distribution", tmp);

	backend->save();
}

void Statistics::reset() {
	games_started = 0;
	games_finished = 0;

	score_distribution = vector<int>(score_distributions_slots, 0);
	
	save();
}
