#!/bin/sh
# HMTsu
# Copyright (C) 2013 Lcferrum
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.

fn_prepare_dir() {
	rm -rf debian
	mkdir debian
	cp -r qtc_packaging/debian_harmattan/* debian/
}

fn_filter_rules() {
	sed "s/^#\(.*# Uncomment this line for use without Qt Creator\)\( with $1\)\?$/\1\2/" qtc_packaging/debian_harmattan/rules > debian/rules
}

case "$1" in
	release)
		fn_prepare_dir
		fn_filter_rules "debug disabled"
		exit 0
	;;

	debug)
		fn_prepare_dir
		fn_filter_rules "debug enabled"
		exit 0
	;;

	*)
		echo "script called with unknown parameter '$1'" >&2
		echo "should be 'release' or 'debug'" >&2
		exit 1
	;;
esac
