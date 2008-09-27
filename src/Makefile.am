bin_PROGRAMS = openyahtzee
openyahtzee_SOURCES = \
	About.cpp \
	configuration.cpp \
	configuration.h \
	DBwrapper.cpp \
	dice_graphics.cpp \
	dice_theme_dialog.cpp \
	MainFrame.cpp \
	highscores_dialog.cpp \
	highscores_dialog.h \
	HighScoreTableDB.cpp \
	icon.xpm \
	icon32.xpm \
	openyahtzee.cpp \
	settings_dialog.cpp \
	settings_dialog.h \
	ScoreDice.cpp \
	UtilityFunctions.cpp \
	wxDynamicBitmap.cpp

# set the include path found by configure
AM_CPPFLAGS = $(all_includes)

# the library search path.

dice_graphics =  \
	dice/theme1/one.xpm \
	dice/theme1/two.xpm \
	dice/theme1/three.xpm \
	dice/theme1/four.xpm \
	dice/theme1/five.xpm \
	dice/theme1/six.xpm \
	dice/theme2/one.xpm \
	dice/theme2/two.xpm \
	dice/theme2/three.xpm \
	dice/theme2/four.xpm \
	dice/theme2/five.xpm \
	dice/theme2/six.xpm \
	dice/theme3/1.xpm \
	dice/theme3/2.xpm \
	dice/theme3/3.xpm \
	dice/theme3/4.xpm \
	dice/theme3/5.xpm \
	dice/theme3/6.xpm

noinst_HEADERS = \
	About.h \
	DBwrapper.h \
	dice_graphics.h \
	dice_theme_dialog.h \
	HighScoreTableDB.h \
	Icon.h \
	icon32.ico \
	MainFrame.h \
	ObjectsID.h \
	ScoreDice.h \
	UtilityFunctions.h \
	wxDynamicBitmap.h \
	$(dice_graphics)

AM_CXXFLAGS = `wx-config --cxxflags`
openyahtzee_LDADD = -lsqlite3
openyahtzee_LDFLAGS = `wx-config --libs`
