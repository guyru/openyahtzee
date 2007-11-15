bin_PROGRAMS = openyahtzee
openyahtzee_SOURCES = openyahtzee.cpp MainFrame.cpp five.xpm four.xpm one.xpm \
							six.xpm three.xpm two.xpm DBwrapper.cpp DBwrapper.h SettingsDB.cpp \
							HighScoreTableDB.cpp HighScoreDialog.cpp SettingsDialog.cpp icon.xpm icon32.xpm \
				openyahtzee.rc wxDynamicBitmap.cpp About.cpp UtilityFunctions.cpp

# set the include path found by configure
INCLUDES = $(all_includes)

# the library search path.

noinst_HEADERS = MainFrame.h ObjectsID.h SettingsDB.h HighScoreTableDB.h \
	HighScoreDialog.h SettingsDialog.h Icon.h icon32.ico wxDynamicBitmap.h UtilityFunctions.h

AM_CXXFLAGS = `wx-config --cxxflags`
openyahtzee_LDADD = -lsqlite3
openyahtzee_LDFLAGS = `wx-config --libs`
