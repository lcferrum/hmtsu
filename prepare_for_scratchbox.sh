#!/bin/sh

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
