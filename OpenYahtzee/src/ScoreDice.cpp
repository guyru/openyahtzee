
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
 
#include "ScoreDice.h"

/**
 * \brief constructor
 *
 * This constructor also sets the values of the dice.
 */
ScoreDice::ScoreDice(short int dice[5])
{
	SetDice(dice);
}

/**
 * \brief default constructor
 * \note if you use this constructor you must call ScoreDice::SetDice before
 * using the class.
 * \see ScoreDice::SetDice
 */
ScoreDice::ScoreDice()
{
	//nothing to do here
}
/**
 * \brief Sets the dice values
 * \param dice An array of 5 short ints containing the values of the dice.
 */
void ScoreDice::SetDice(short int dice[5])
{
	for (int i=0; i<5; i++) 
		m_dice[i] = dice[i];
	

	//fill the dice hash
	for (int i=0; i<6; i++)
		m_dicehash[i] = 0;
	for (int i=0; i<5; i++)
		m_dicehash[dice[i]] += 1;	
}

/**
 *
 */
short int ScoreDice::Aces()
{
	return m_dicehash[0];
}

/**
 *
 */
short int ScoreDice::Twos()
{
	return 2*m_dicehash[1];
}


/**
 *
 */
short int ScoreDice::Threes()
{
	return 3*m_dicehash[2];
}

/**
 *
 */
short int ScoreDice::Fours()
{
	return 4*m_dicehash[3];
}

/**
 *
 */
short int ScoreDice::Fives()
{
	return 5*m_dicehash[4];
}

/**
 *
 */
short int ScoreDice::Sixes()
{
	return 6*m_dicehash[5];
}

/**
 *
 */
short int ScoreDice::Yahtzee()
{
	if (IsYahtzee()) 
		return 50;
	return 0;
}


/**
 *
 */
short int ScoreDice::Chance()
{
	short int temp = 0;
	for(int i = 0; i<5; i++) 
		temp += m_dice[i]+1;
	
	return temp;
}

/**
 *
 */
bool ScoreDice::IsYahtzee()
{
	if ((m_dice[0]==m_dice[1]) && (m_dice[1]==m_dice[2]) && \
			(m_dice[1]==m_dice[3]) && (m_dice[1]==m_dice[4]))
		return true;
	return false;
}
