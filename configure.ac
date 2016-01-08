AC_INIT([Open Yahtzee],[1.9.3],[guyrutenberg@gmail.com],[openyahtzee])

AM_INIT_AUTOMAKE([-Wall -Werror])
AC_CONFIG_SRCDIR([src/openyahtzee.cpp])
AC_CONFIG_HEADER([config.h])

m4_include(macros/wxwin.m4)
m4_include(macros/ax_check_compile_flag.m4)
m4_include(macros/ax_boost_base.m4)

# Checks for programs.
AC_PROG_CXX

AX_CHECK_COMPILE_FLAG([-std=c++0x], [CXXFLAGS="$CXXFLAGS -std=c++0x"])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for library functions.
AC_CHECK_FUNCS([sqrt])

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

AX_BOOST_BASE([1.32])


AC_CONFIG_FILES([Makefile
                 man/Makefile
                 src/Makefile])
AC_OUTPUT
