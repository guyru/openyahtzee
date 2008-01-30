# not a GNU package. You can remove this line, if
# have all needed files, that a GNU package needs
AUTOMAKE_OPTIONS = foreign 1.4

SUBDIRS = src

appicondir = $(datarootdir)/pixmaps
dist_appicon_DATA = icons/openyahtzee.png

menudir = $(datarootdir)/applications
dist_menu_DATA = openyahtzee.desktop

AM_CXXFLAGS = `wx-config --cxxflags`
