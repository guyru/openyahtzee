#!/bin/sh
autopoint --force
aclocal
autoheader
#libtoolize --automake --force --copy
automake -a -c
autoconf
