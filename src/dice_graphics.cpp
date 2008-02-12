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


DiceGraphics::DiceGraphics()
{
	m_theme = (DiceTheme*)new DiceTheme1();
}

DiceGraphics::~DiceGraphics()
{
	delete m_theme;
}

/**
 * \param i the number of the dice (1 to 6)
 * \return a pointer to wxBitmap
 */
wxBitmap *DiceGraphics::GetDice(int i)
{
	return m_theme->GetDice(i);
}

namespace dicetheme1 {
	#include "dice/theme1/one.xpm"
	#include "dice/theme1/two.xpm"
	#include "dice/theme1/three.xpm"
	#include "dice/theme1/four.xpm"
	#include "dice/theme1/five.xpm"
	#include "dice/theme1/six.xpm"
}
DiceTheme1::DiceTheme1() {
	m_bitmaps[0] = new wxBitmap(dicetheme1::one_xpm);
	m_bitmaps[1] = new wxBitmap(dicetheme1::two_xpm);
	m_bitmaps[2] = new wxBitmap(dicetheme1::three_xpm);
	m_bitmaps[3] = new wxBitmap(dicetheme1::four_xpm);
	m_bitmaps[4] = new wxBitmap(dicetheme1::five_xpm);
	m_bitmaps[5] = new wxBitmap(dicetheme1::six_xpm);
}

/**
 * \param i the number of the dice (1 to 6)
 * \return a pointer to wxBitmap
 */
wxBitmap* DiceTheme1::GetDice(int i)
{
	return m_bitmaps[i-1];
}

namespace dicetheme2 {
	#include "dice/theme2/one.xpm"
	#include "dice/theme2/two.xpm"
	#include "dice/theme2/three.xpm"
	#include "dice/theme2/four.xpm"
	#include "dice/theme2/five.xpm"
	#include "dice/theme2/six.xpm"
}
DiceTheme2::DiceTheme2() {
	m_bitmaps[0] = new wxBitmap(dicetheme2::one_xpm);
	m_bitmaps[1] = new wxBitmap(dicetheme2::two_xpm);
	m_bitmaps[2] = new wxBitmap(dicetheme2::three_xpm);
	m_bitmaps[3] = new wxBitmap(dicetheme2::four_xpm);
	m_bitmaps[4] = new wxBitmap(dicetheme2::five_xpm);
	m_bitmaps[5] = new wxBitmap(dicetheme2::six_xpm);
}

/**
 * \param i the number of the dice (1 to 6)
 * \return a pointer to wxBitmap
 */
wxBitmap* DiceTheme2::GetDice(int i)
{
	return m_bitmaps[i-1];
}

namespace dicetheme3 {
	#include "dice/theme3/1.xpm"
	#include "dice/theme3/2.xpm"
	#include "dice/theme3/3.xpm"
	#include "dice/theme3/4.xpm"
	#include "dice/theme3/5.xpm"
	#include "dice/theme3/6.xpm"
}
DiceTheme3::DiceTheme3() {
	m_bitmaps[0] = new wxBitmap(dicetheme3::one_xpm);
	m_bitmaps[1] = new wxBitmap(dicetheme3::two_xpm);
	m_bitmaps[2] = new wxBitmap(dicetheme3::three_xpm);
	m_bitmaps[3] = new wxBitmap(dicetheme3::four_xpm);
	m_bitmaps[4] = new wxBitmap(dicetheme3::five_xpm);
	m_bitmaps[5] = new wxBitmap(dicetheme3::six_xpm);
}

/**
 * \param i the number of the dice (1 to 6)
 * \return a pointer to wxBitmap
 */
wxBitmap *DiceTheme3::GetDice(int i)
{
	return m_bitmaps[i-1];
}
