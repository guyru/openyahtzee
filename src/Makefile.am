bin_PROGRAMS = openyahtzee
openyahtzee_SOURCES = \
	About.cpp \
	../config.h \
	configuration.cpp \
	configuration.h \
	DBwrapper.cpp \
	MainFrame.cpp \
	highscores_dialog.cpp \
	highscores_dialog.h \
	icon32.xpm \
	icon64.xpm \
	openyahtzee.cpp \
	settings_dialog.cpp \
	settings_dialog.h \
	ScoreDice.cpp \
	wxDynamicBitmap.cpp \
	one.xpm \
	two.xpm \
	three.xpm \
	four.xpm \
	five.xpm \
	six.xpm \
	sqlite/sqlite3.c \
	sqlite/sqlite3.h

# set the include path found by configure
AM_CPPFLAGS = $(all_includes)

# the library search path.

noinst_HEADERS = \
	About.h \
	DBwrapper.h \
	openyahtzee.ico \
	MainFrame.h \
	ObjectsID.h \
	ScoreDice.h \
	wxDynamicBitmap.h \
	../openyahtzee.vcproj \
	../openyahtzee.sln \
	openyahtzee.ico \
	openyahtzee.rc

AM_CXXFLAGS = `wx-config --cxxflags`
openyahtzee_LDFLAGS = `wx-config --libs`
