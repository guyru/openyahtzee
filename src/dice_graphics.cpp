/***************************************************************************
 *   Copyright (C) 2006-2007 by Guy Rutenberg   *
 *   guyrutenberg@gmail.com   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *               `                                                          *
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

#include "dice_graphics.h"

#include "one.xpm"
#include "two.xpm"
#include "three.xpm"
#include "four.xpm"
#include "five.xpm"
#include "six.xpm"

DiceGraphics::DiceGraphics()
{
	m_theme = 0;

	LoadTheme1();
}
void DiceGraphics::LoadTheme1()
{
	m_bitmaps[0][0] = new wxBitmap(one_xpm);
	m_bitmaps[0][1] = new wxBitmap(two_xpm);
	m_bitmaps[0][2] = new wxBitmap(three_xpm);
	m_bitmaps[0][3] = new wxBitmap(four_xpm);
	m_bitmaps[0][4] = new wxBitmap(five_xpm);
	m_bitmaps[0][5] = new wxBitmap(six_xpm);
}
/**
 * \param i the number of the dice (1 to 6)
 * \return a pointer to wxBitmap
 */
wxBitmap *DiceGraphics::GetDice(int i)
{
	return m_bitmaps[m_theme][i-1];
}
