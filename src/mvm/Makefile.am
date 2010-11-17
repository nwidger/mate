AUTOMAKE_OPTIONS = foreign
bin_PROGRAMS = mvm
man_MANS = mvm.1

CFLAGS = -Wall -g
DEFAULT_INCLUDES = -I../..

EXTRA_DIST = 

mvm_SOURCES = breakpoint.c class.c class_table.c disassemble.c \
frame.c garbage_collector.c heap.c instruction_table.c \
instructions.c integer.c invoke_method.c local_variable_array.c \
main.c mdb.c method_area.c native_method_array.c native_methods.c \
nlock.c object.c operand_stack.c ref_set.c string.c symbol_table.c table.c \
vm_stack.c

noinst_HEADERS = breakpoint.h class.h class_table.h disassemble.h \
frame.h garbage_collector.h globals.h heap.h instruction_table.h \
instructions.h integer.h invoke_method.h local_variable_array.h \
mdb.h mdb_help.h method_area.h native_method_array.h native_methods.h \
nlock.h object.h operand_stack.h ref_set.h string.h symbol_table.h table.h \
vm_stack.h mvm.1
