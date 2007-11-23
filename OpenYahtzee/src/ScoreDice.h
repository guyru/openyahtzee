
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


class ScoreDice {
public:
	ScoreDice();
	ScoreDice(short int dice[5]);
	void SetDice(short int dice[5]);
	short int Aces();
	short int Twos();
	short int Threes();
	short int Fours();
	short int Fives();
	short int Sixes();

	short int ThreeOfAKind();
	short int FourOfAKind();
	short int FullHouse();
	short int SmallSequence();
	short int LargeSequence();
	short int Yahtzee();
	short int Chance();

	bool IsYahtzee();

private:
	short int m_dice[5]; 
	short int m_dicehash[6];

};
