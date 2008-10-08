
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
void ScoreDice::SetDice(const short int dice[5])
{
	for (int i=0; i<5; i++) 
		m_dice[i] = dice[i];
	
	//fill the dice hash
	for (int i=0; i<6; i++)
		m_dicehash[i] = 0;
	for (int i=0; i<5; i++)
		m_dicehash[dice[i]-1] += 1;	
	
	m_yahtzee_joker = false;
}

/**
 * Tells the scoring class wheter or not we have a Yahtzee Joker.
 * \param is_yahtzee_joker [bool] true if there is a Yahtzee Joker.
 * \note checking whether we got a yahtzee joker is done by an outside function,
 * and it needs the dice set before it can operate.
 * \see MainFrame::YahtzeeJoker()
 */
void ScoreDice::SetYahtzeeJoker(const bool is_yahtzee_joker)
{
	m_yahtzee_joker = is_yahtzee_joker;
}

/**
 * \brief Gets the value of a dice.
 * \param number the index of the dice wanted.
 * \return the value of the dice. 0 if wrong input.
 */
short int ScoreDice::GetDice(const short int number)
{
	if (number<=5 && number >=1)
		return m_dice[number-1];
	return 0;
}
/**
 * \return the score for the aces box.
 */
short int ScoreDice::Aces() const
{
	return m_dicehash[0];
}

/**
 * \return the score for the twos box.
 */
short int ScoreDice::Twos() const
{
	return 2*m_dicehash[1];
}


/**
 * \return the score for the threes box.
 */
short int ScoreDice::Threes() const
{
	return 3*m_dicehash[2];
}

/**
 * \return the score for the fours box.
 */
short int ScoreDice::Fours() const
{
	return 4*m_dicehash[3];
}

/**
 *
 * \return the score for the fives box.
 */
short int ScoreDice::Fives() const
{
	return 5*m_dicehash[4];
}

/**
 *
 * \return the score for the sixes box.
 */
short int ScoreDice::Sixes() const
{
	return 6*m_dicehash[5];
}

/**
 *
 * \return the score for the "Three of A Kind" box.
 */
short int ScoreDice::ThreeOfAKind() const
{
	bool three = false;
	
	for (int i=0; i<6; i++)
		if (m_dicehash[i] >= 3)
			three = true;
	if (three)
		return Chance();
	return 0;
}

/**
 *
 * \return the score for the "Four of A Kind" box.
 */
short int ScoreDice::FourOfAKind() const
{
	bool four = false;
	
	for (int i=0; i<6; i++)
		if (m_dicehash[i] >= 4)
			four = true;
	if (four)
		return Chance();
	return 0;
}

/**
 *
 * \return the score for the "Full House" box.
 */
short int ScoreDice::FullHouse() const
{
	bool two = false;
	bool three = false;

	for (int i=0; i<6; i++)
		if (m_dicehash[i] == 2)
			two = true;
	for (int i=0; i<6; i++)
		if (m_dicehash[i] == 3)
			three = true;
	if ((two && three) || m_yahtzee_joker)
		return 25;
	return 0;
}

/**
 *
 * \return the score for the small sequence box.
 */
short int ScoreDice::SmallSequence() const
{
	if ( (m_dicehash[0]>=1 && m_dicehash[1]>=1 && m_dicehash[2]>=1 &&
		m_dicehash[3]>=1) || (m_dicehash[1]>=1 && m_dicehash[2]>=1 &&
		m_dicehash[3]>=1 && m_dicehash[4]>=1) || (m_dicehash[2]>=1 &&
		m_dicehash[3]>=1 && m_dicehash[4]>=1 && m_dicehash[5]>=1))
		return 30;
	else if (m_yahtzee_joker)
		return 30;
	return 0;
}

/**
 *
 * \return the score for the large sequence box.
 */
short int ScoreDice::LargeSequence() const
{
	if ( (m_dicehash[0]==1 && m_dicehash[1]==1 && m_dicehash[2]==1 &&
		m_dicehash[3]==1 && m_dicehash[4]==1) || (m_dicehash[1]==1 &&
		m_dicehash[2]==1 && m_dicehash[3]==1 && m_dicehash[4]==1 &&
		m_dicehash[5]==1))
		return 40;
	else if (m_yahtzee_joker)
		return 40;
	return 0;
}
/**
 *
 * \return the score for the Yahtzee box.
 */
short int ScoreDice::Yahtzee() const
{
	if (IsYahtzee()) 
		return 50;
	return 0;
}


/**
 *
 * \return the score for the chance box.
 */
short int ScoreDice::Chance() const
{
	short int temp = 0;
	for(int i = 0; i<5; i++) 
		temp += m_dice[i];
	
	return temp;
}

/**
 * \brief determines wheter the current dice hold a Yahtzee or not.
 * \return true or false depending on whether there is a Yahtzee or not.
 */
bool ScoreDice::IsYahtzee() const
{
	if ((m_dice[0]==m_dice[1]) && (m_dice[1]==m_dice[2]) && \
			(m_dice[1]==m_dice[3]) && (m_dice[1]==m_dice[4]))
		return true;
	return false;
}
