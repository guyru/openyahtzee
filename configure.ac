AC_INIT([Open Yahtzee],[1.9],[guyrutenberg@gmail.com],[openyahtzee])

AM_INIT_AUTOMAKE([-Wall -Werror])
AC_CONFIG_SRCDIR([src/openyahtzee.cpp])
AC_CONFIG_HEADER([config.h])

m4_include(macros/wxwin.m4)

dnl AC_LANG_CPLUSPLUS
AC_PROG_CXX
dnl AM_PROG_LIBTOOL

AM_OPTIONS_WXCONFIG
#Detect wxWidgets
MIN_WX_VERSION="2.8.0"
AM_PATH_WXCONFIG($MIN_WX_VERSION, wxWin=1)
if test "$wxWin" != 1; then
AC_MSG_ERROR([
	wxWidgets must be installed on your system
	but wx-config script couldn't be found.

	Please check that wx-config is in path, the directory
	where wxWidgets libraries are installed (returned by
	'wx-config --libs' command) is in LD_LIBRARY_PATH or
	equivalent variable and wxWidgets version is $MIN_WX_VERSION or above.
])
fi


AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
