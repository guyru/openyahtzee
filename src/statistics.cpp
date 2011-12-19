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
#include <sstream>
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
	this->backend = backend;
	try {
		load_data();
	} catch (std::exception &e) {
		reset();
	}
}

void Statistics::load_data() 
{
	string tmp;
	vector<string> tmp_vec;
	
	tmp = backend->get("statistics_games_started");
	_games_started = atoi(tmp.c_str());

	tmp = backend->get("statistics_games_finished");
	_games_finished = atoi(tmp.c_str());

	tmp = backend->get("statistics_score_distribution");
	split(tmp_vec, tmp, is_any_of(","));
	if (tmp_vec.size() != score_distributions_slots)
		throw BadConfiguration();

	for (string &i : tmp_vec) {
		_score_distribution.push_back(atoi(i.c_str()));
	}

	istringstream i( backend->get("statistics_last_reset"));
	i >> _last_reset;
	if (!_last_reset)
		throw BadConfiguration();
		
}

void Statistics::game_started()
{	
	_games_started++;
	save();
}

void Statistics::game_finished(int score)
{	
	int score_slot;
	_games_finished++;

	score_slot = score/score_distribution_granuality;
	score_slot = score_slot<score_distributions_slots ? score_slot : score_slot;
	_score_distribution[score_slot]++;
	save();
}

void Statistics::save() {
	string tmp;
	backend->set("statistics_games_started", stringify(_games_started));
	backend->set("statistics_games_finished", stringify(_games_finished));
	backend->set("statistics_last_reset", stringify(_last_reset));

	tmp = "";
	for (int &i : _score_distribution) {
		tmp += stringify(i) + ",";
	}
	//delete trailing comma
	tmp = tmp.substr(0, tmp.size()-1);
	backend->set("statistics_score_distribution", tmp);

	backend->save();
}

void Statistics::reset() {
	_games_started = 0;
	_games_finished = 0;
	_last_reset = time(NULL);

	_score_distribution = vector<int>(score_distributions_slots, 0);
	
	save();
}
