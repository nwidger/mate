AUTOMAKE_OPTIONS = foreign
bin_PROGRAMS = mvm
man_MANS = mvm.1

CFLAGS = -Wall -DDATA_PATH=\"$(pkgdatadir)\"

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

DEFAULT_INCLUDES = -I../..

EXTRA_DIST = 
AM_YFLAGS = -d
BUILT_SOURCES = parse.h

mvm_SOURCES = parse.y scan.l \
barrier.c breakpoint.c class.c class_table.c disassemble.c \
frame.c garbage_collector.c heap.c instruction_table.c \
instructions.c integer.c invoke_method.c local_variable_array.c \
main.c mdb.c method_area.c native_method_array.c native_methods.c \
nlock.c object.c operand_stack.c real.c ref_set.c string.c \
symbol_table.c table.c thread.c vm_stack.c

noinst_HEADERS = parse.h \
barrier.h breakpoint.h class.h class_table.h disassemble.h \
frame.h garbage_collector.h globals.h heap.h instruction_table.h \
instructions.h integer.h invoke_method.h local_variable_array.h \
mdb.h mdb_help.h method_area.h native_method_array.h native_methods.h \
nlock.h object.h operand_stack.h real.h ref_set.h string.h \
symbol_table.h table.h thread.h vm_stack.h mvm.1

if DMP
CFLAGS += -DDMP
mvm_SOURCES += dmp.c nlock_dmp.c object_dmp.c table_dmp.c thread_dmp.c
noinst_HEADERS += dmp.h nlock_dmp.h object_dmp.h table_dmp.h thread_dmp.h
endif
