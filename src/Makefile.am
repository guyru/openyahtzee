bin_PROGRAMS = openyahtzee
openyahtzee_SOURCES = openyahtzee.cpp MainFrame.cpp five.xpm four.xpm one.xpm \
			six.xpm three.xpm two.xpm DBwrapper.cpp DBwrapper.h SettingsDB.cpp \
			HighScoreTableDB.cpp HighScoreDialog.cpp SettingsDialog.cpp icon.xpm icon32.xpm

# set the include path found by configure
INCLUDES = $(all_includes)

# the library search path.

noinst_HEADERS = MainFrame.h ObjectsID.h SettingsDB.h HighScoreTableDB.h \
			HighScoreDialog.h SettingsDialog.h Icon.h

openyahtzee_LDFLAGS = `wx-config --libs` -lsqlite3
AM_CXXFLAGS = `wx-config --cxxflags`
