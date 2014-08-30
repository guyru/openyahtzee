bin_PROGRAMS = openyahtzee
openyahtzee_SOURCES = \
	about.cpp \
	about.h \
	../config.h \
	configuration.cpp \
	configuration.h \
	MainFrame.cpp \
	highscores_dialog.cpp \
	highscores_dialog.h \
	icon32.xpm \
	icon64.xpm \
	openyahtzee.cpp \
	settings_dialog.cpp \
	settings_dialog.h \
	ScoreDice.cpp \
	simple_pie_plot.cpp \
	statistics.cpp \
	statistics_dialog.cpp \
	wxDynamicBitmap.cpp \
	one.xpm \
	two.xpm \
	three.xpm \
	four.xpm \
	five.xpm \
	six.xpm

# set the include path found by configure
AM_CPPFLAGS = -DLOCALEDIR=\"$(localedir)\" $(all_includes)

# the library search path.

noinst_HEADERS = \
	openyahtzee.ico \
	MainFrame.h \
	ScoreDice.h \
	simple_pie_plot.h \
	statistics.h \
	statistics_dialog.h \
	utility.h \
	wxDynamicBitmap.h \
	../openyahtzee.vcproj \
	../openyahtzee.sln \
	openyahtzee.ico \
	openyahtzee.rc

AM_CXXFLAGS = @WX_CXXFLAGS@
openyahtzee_LDADD = @WX_LIBS@ @LIBINTL@
