AUTOMAKE_OPTIONS = foreign
SUBDIRS = src/mc src/mas src/mvm

EXTRA_DIST = doc tests

dist_bin_SCRIPTS = mcc
dist_pkgdata_DATA = src/mc/predefined_classes.m
noinst_HEADERS = constants.h

TESTS = tests/run_tests.sh

.PHONY: src/mc/predefined_classes.m
