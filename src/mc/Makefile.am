AUTOMAKE_OPTIONS = foreign
bin_PROGRAMS = mc
man_MANS = mc.1

CFLAGS = -Wall -DDATA_PATH=\"$(pkgdatadir)\"
CXXFLAGS = -Wall -DDATA_PATH=\"$(pkgdatadir)\"

if OPTIMIZE1
CFLAGS += -O1
CXXFLAGS += -O1
endif

if OPTIMIZE2
CFLAGS += -O2
CXXFLAGS += -O2
endif

if OPTIMIZE3
CFLAGS += -O3
CXXFLAGS += -O3
endif

if DEBUG
CFLAGS += -g
CXXFLAGS += -g
endif

if PROFILE
CFLAGS += -pg
CXXFLAGS += -pg
endif

DEFAULT_INCLUDES = -I../..
AM_YFLAGS = -d
BUILT_SOURCES = parse.h
mc_SOURCES = parse.yy scan.ll ClassTable.cc LabelStack.cc LocalVariableStack.cc \
MonitorStack.cc Node.cc Seq.cc StringPool.cc Type.cc TypeModule.cc encode.cc main.cc

noinst_HEADERS = parse.h globals.h ClassTable.h LabelStack.h LocalVariableStack.h \
MonitorStack.h Node.h Seq.h StringPool.h Type.h TypeModule.h mc.1


