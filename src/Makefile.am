bin_PROGRAMS = openyahtzee
openyahtzee_SOURCES = openyahtzee.cpp MainFrame.cpp DBwrapper.cpp \
	HighScoreTableDB.cpp HighScoreDialog.cpp SettingsDialog.cpp \
	SettingsDB.cpp icon.xpm icon32.xpm wxDynamicBitmap.cpp About.cpp \
	UtilityFunctions.cpp ScoreDice.cpp dice_graphics.cpp \
	dice_theme_dialog.cpp

# set the include path found by configure
AM_CPPFLAGS = $(all_includes)

# the library search path.

dice_graphics =  dice/theme1/one.xpm dice/theme1/two.xpm dice/theme1/three.xpm \
	dice/theme1/four.xpm dice/theme1/five.xpm dice/theme1/six.xpm \
	dice/theme2/one.xpm dice/theme2/two.xpm dice/theme2/three.xpm \
	dice/theme2/four.xpm dice/theme2/five.xpm dice/theme2/six.xpm \
	dice/theme3/1.xpm dice/theme3/2.xpm dice/theme3/3.xpm \
	dice/theme3/4.xpm dice/theme3/5.xpm dice/theme3/6.xpm

noinst_HEADERS = MainFrame.h ObjectsID.h SettingsDB.h HighScoreTableDB.h \
	HighScoreDialog.h SettingsDialog.h Icon.h icon32.ico wxDynamicBitmap.h \
	About.h UtilityFunctions.h ScoreDice.h dice_graphics.h DBwrapper.h \
	dice_theme_dialog.h $(dice_graphics)

AM_CXXFLAGS = `wx-config --cxxflags`
openyahtzee_LDADD = -lsqlite3
openyahtzee_LDFLAGS = `wx-config --libs`
