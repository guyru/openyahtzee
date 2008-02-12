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
#ifndef _DICE_GRAPHICS_INC_
#define _DICE_GRAPHICS_INC_

#include <wx/bitmap.h>
#define NUM_OF_THEMES 3

/**
 * This is the base class for the DiceThemes. It implements a Strategy design
 * pattern
 */
class DiceTheme {
public:
	virtual wxBitmap* GetDice(int i) = 0;
};

class DiceGraphics {
public:
	DiceGraphics();
	DiceGraphics(DiceTheme* theme);
	~DiceGraphics();
	wxBitmap *GetDice(int i);

private:
	DiceTheme *m_theme;
};

class DiceTheme1 : public DiceTheme {
	wxBitmap *m_bitmaps[6];
public:
	DiceTheme1();
	virtual wxBitmap* GetDice(int i);
};

class DiceTheme2 : public DiceTheme {
	wxBitmap *m_bitmaps[6];
public:
	DiceTheme2();
	virtual wxBitmap* GetDice(int i);
};

class DiceTheme3 : public DiceTheme {
	wxBitmap *m_bitmaps[6];
public:
	virtual wxBitmap* GetDice(int i);
	DiceTheme3();
};


#endif // _DICE_GRAPHICS_INC_
