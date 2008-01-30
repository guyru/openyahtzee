
/***************************************************************************
 *   Copyright (C) 2006-2007 by Guy Rutenberg   *
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
#ifndef SCORE_DICE_H
#define SCORE_DICE_H

class ScoreDice {
public:
	ScoreDice();
	ScoreDice(short int dice[5]);
	void SetDice(const short int dice[5]);
	void SetYahtzeeJoker(bool is_yahtzee_joker);
	short int GetDice(short int number);

	short int Aces() const;
	short int Twos() const;
	short int Threes() const;
	short int Fours() const;
	short int Fives() const;
	short int Sixes() const;

	short int ThreeOfAKind() const;
	short int FourOfAKind() const;
	short int FullHouse() const;
	short int SmallSequence() const;
	short int LargeSequence() const;
	short int Yahtzee() const;
	short int Chance() const;

	bool IsYahtzee() const;

private:
	short int m_dice[5]; 
	short int m_dicehash[6];
	bool m_yahtzee_joker;

};

#endif //SCORE_DICE_H
