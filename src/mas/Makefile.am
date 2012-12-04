AUTOMAKE_OPTIONS = foreign
bin_PROGRAMS = mas
man_MANS = mas.1

CFLAGS = -Wall

if OPTIMIZE1
CFLAGS += -O1
endif

if OPTIMIZE2
CFLAGS += -O2
endif

if OPTIMIZE3
CFLAGS += -O3
endif

if DEBUG
CFLAGS += -g
endif

if PROFILE
CFLAGS += -pg
endif

EXTRA_DIST =
DEFAULT_INCLUDES = -I../..
AM_YFLAGS = -d
BUILT_SOURCES = parse.h
mas_SOURCES = parse.y scan.l instruction_table.c main.c passes.c string_pool.c \
symbol_table.c

noinst_HEADERS = instruction_table.h parse.h passes.h string_pool.h \
symbol_table.h mas.1
