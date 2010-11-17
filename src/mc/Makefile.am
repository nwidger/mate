AUTOMAKE_OPTIONS = foreign
bin_PROGRAMS = mc
man_MANS = mc.1

CFLAGS = -Wall -g
CXXFLAGS = -Wall -g
DEFAULT_INCLUDES = -I../..
AM_YFLAGS = -d
BUILT_SOURCES = parse.h
mc_SOURCES = parse.yy scan.ll ClassTable.cc LabelStack.cc LocalVariableStack.cc \
Node.cc Seq.cc StringPool.cc Type.cc TypeModule.cc encode.cc main.cc

noinst_HEADERS = parse.h globals.h ClassTable.h LabelStack.h LocalVariableStack.h \
Node.h Seq.h StringPool.h Type.h TypeModule.h mc.1


