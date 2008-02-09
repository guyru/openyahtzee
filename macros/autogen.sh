#!/bin/sh
aclocal
autoheader
#libtoolize --automake --force --copy
automake -a -c
autoconf

