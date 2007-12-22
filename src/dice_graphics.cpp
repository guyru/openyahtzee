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

#include "dice/theme1/one.xpm"
#include "dice/theme1/two.xpm"
#include "dice/theme1/three.xpm"
#include "dice/theme1/four.xpm"
#include "dice/theme1/five.xpm"
#include "dice/theme1/six.xpm"

#include "dice/theme2/one.xpm"
#include "dice/theme2/two.xpm"
#include "dice/theme2/three.xpm"
#include "dice/theme2/four.xpm"
#include "dice/theme2/five.xpm"
#include "dice/theme2/six.xpm"

#include "dice/theme3/1.xpm"
#include "dice/theme3/2.xpm"
#include "dice/theme3/3.xpm"
#include "dice/theme3/4.xpm"
#include "dice/theme3/5.xpm"
#include "dice/theme3/6.xpm"

DiceGraphics::DiceGraphics()
{
	m_theme = 0;

	LoadTheme1();
	LoadTheme2();
	LoadTheme3();
}

void DiceGraphics::LoadTheme1()
{
	m_bitmaps[0][0] = new wxBitmap(theme1_one_xpm);
	m_bitmaps[0][1] = new wxBitmap(theme1_two_xpm);
	m_bitmaps[0][2] = new wxBitmap(theme1_three_xpm);
	m_bitmaps[0][3] = new wxBitmap(theme1_four_xpm);
	m_bitmaps[0][4] = new wxBitmap(theme1_five_xpm);
	m_bitmaps[0][5] = new wxBitmap(theme1_six_xpm);
}

void DiceGraphics::LoadTheme2()
{
	m_bitmaps[1][0] = new wxBitmap(theme2_one_xpm);
	m_bitmaps[1][1] = new wxBitmap(theme2_two_xpm);
	m_bitmaps[1][2] = new wxBitmap(theme2_three_xpm);
	m_bitmaps[1][3] = new wxBitmap(theme2_four_xpm);
	m_bitmaps[1][4] = new wxBitmap(theme2_five_xpm);
	m_bitmaps[1][5] = new wxBitmap(theme2_six_xpm);
}

void DiceGraphics::LoadTheme3()
{
	m_bitmaps[2][0] = new wxBitmap(theme3_1_xpm);
	m_bitmaps[2][1] = new wxBitmap(theme3_2_xpm);
	m_bitmaps[2][2] = new wxBitmap(theme3_3_xpm);
	m_bitmaps[2][3] = new wxBitmap(theme3_4_xpm);
	m_bitmaps[2][4] = new wxBitmap(theme3_5_xpm);
	m_bitmaps[2][5] = new wxBitmap(theme3_6_xpm);
}
/**
 * \param i the number of the dice (1 to 6)
 * \return a pointer to wxBitmap
 */
wxBitmap *DiceGraphics::GetDice(int i)
{
	return m_bitmaps[m_theme][i-1];
}
