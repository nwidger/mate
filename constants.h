/* Niels Widger
 * Time-stamp: <28 Nov 2010 at 22:02:06 by nwidger on macros.local>
 */

#ifndef _MVM_CONSTANTS_H
#define _MVM_CONSTANTS_H

#include <stdint.h>

/* garbage collector */
#define GARBAGE_COLLECTOR_DEFAULT_INTERVAL 5
#define GARBAGE_COLLECTOR_DEFAULT_TYPE serial_type

/* instructions */
#define NUM_INSTRUCTIONS 24

/* instruction opcodes */
#define MAX_OPCODE 0xfd

#define ACONST_NULL_OPCODE 0x01
#define ALOAD_OPCODE 0x19
#define ASTORE_OPCODE 0x3a
#define DUP_OPCODE 0x59
#define IFEQ_OPCODE 0x99
#define GOTO_OPCODE 0xa7
#define ARETURN_OPCODE 0xb0
#define GETFIELD_OPCODE 0xb4
#define PUTFIELD_OPCODE 0xb5
#define INVOKEVIRTUAL_OPCODE 0xb6
#define INVOKESPECIAL_OPCODE 0xb7
#define NEW_OPCODE 0xbb
#define CHECKCAST_OPCODE 0xc0
#define IN_OPCODE 0xf3
#define INVOKENATIVE_OPCODE 0xf4
#define OUT_OPCODE 0xf5
#define RETURN_OPCODE 0xf6
#define NEWINT_OPCODE 0xf7
#define NEWSTR_OPCODE 0xf8
#define REFCMP_OPCODE 0xf9
#define POP_OPCODE 0xfa
#define DUP_X1_OPCODE 0xfb
#define MONITORENTER_OPCODE 0xfc
#define MONITOREXIT_OPCODE 0xfd

/* instruction names */
#define ACONST_NULL_NAME "aconst_null"
#define ALOAD_NAME "aload"
#define ASTORE_NAME "astore"
#define DUP_NAME "dup"
#define IFEQ_NAME "ifeq"
#define GOTO_NAME "goto"
#define ARETURN_NAME "areturn"
#define GETFIELD_NAME "getfield"
#define PUTFIELD_NAME "putfield"
#define INVOKEVIRTUAL_NAME "invokevirtual"
#define INVOKESPECIAL_NAME "invokespecial"
#define NEW_NAME "new"
#define CHECKCAST_NAME "checkcast"
#define IN_NAME "in"
#define INVOKENATIVE_NAME "invokenative"
#define OUT_NAME "out"
#define RETURN_NAME "return"
#define NEWINT_NAME "newint"
#define NEWSTR_NAME "newstr"
#define REFCMP_NAME "refcmp"
#define POP_NAME "pop"
#define DUP_X1_NAME "dup_x1"
#define MONITORENTER_NAME "monitorenter"
#define MONITOREXIT_NAME "monitorexit"

/* native methods */
#define NUM_NATIVES 51
#define MAX_NATIVE_INDEX 50

/* native method indices */
#define OBJECT_CONSTRUCTOR_NATIVE_INDEX 0
#define OBJECT_EQUALS_NATIVE_INDEX 1
#define OBJECT_HASH_CODE_NATIVE_INDEX 2
#define OBJECT_TO_STRING_NATIVE_INDEX 3
#define OBJECT_NOTIFY_NATIVE_INDEX 4
#define OBJECT_NOTIFY_ALL_NATIVE_INDEX 5
#define OBJECT_WAIT_NATIVE_INDEX 6
#define OBJECT_WAIT_INTEGER_NATIVE_INDEX 7

#define INTEGER_CONSTRUCTOR_NATIVE_INDEX 8
#define INTEGER_CONSTRUCTOR_INTEGER_NATIVE_INDEX 9
#define INTEGER_ADD_NATIVE_INDEX 10
#define INTEGER_SUBTRACT_NATIVE_INDEX 11
#define INTEGER_MULTIPLY_NATIVE_INDEX 12
#define INTEGER_DIVIDE_NATIVE_INDEX 13
#define INTEGER_GREATER_THAN_NATIVE_INDEX 14
#define INTEGER_LESS_THAN_NATIVE_INDEX 15
#define INTEGER_NOT_NATIVE_INDEX 16
#define INTEGER_MINUS_NATIVE_INDEX 17
#define INTEGER_ADD_OP_NATIVE_INDEX 18
#define INTEGER_SUBTRACT_OP_NATIVE_INDEX 19
#define INTEGER_MULTIPLY_OP_NATIVE_INDEX 20
#define INTEGER_DIVIDE_OP_NATIVE_INDEX 21
#define INTEGER_GREATER_THAN_OP_NATIVE_INDEX 22
#define INTEGER_LESS_THAN_OP_NATIVE_INDEX 23
#define INTEGER_NOT_OP_NATIVE_INDEX 24
#define INTEGER_MINUS_OP_NATIVE_INDEX 25
#define INTEGER_EQUALS_NATIVE_INDEX 26
#define INTEGER_HASH_CODE_NATIVE_INDEX 27
#define INTEGER_TO_STRING_NATIVE_INDEX 28

#define STRING_CONSTRUCTOR_STRING_NATIVE_INDEX 29
#define STRING_LENGTH_NATIVE_INDEX 30
#define STRING_SUBSTR_NATIVE_INDEX 31
#define STRING_TO_INTEGER_NATIVE_INDEX 32
#define STRING_CONCAT_NATIVE_INDEX 33
#define STRING_CONCAT_OP_NATIVE_INDEX 34
#define STRING_GREATER_THAN_NATIVE_INDEX 35
#define STRING_LESS_THAN_NATIVE_INDEX 36
#define STRING_HASH_CODE_NATIVE_INDEX 37
#define STRING_EQUALS_NATIVE_INDEX 38
#define STRING_TO_STRING_NATIVE_INDEX 39

#define TABLE_CONSTRUCTOR_NATIVE_INDEX 40
#define TABLE_CONSTRUCTOR_INTEGER_NATIVE_INDEX 41
#define TABLE_GET_NATIVE_INDEX 42
#define TABLE_PUT_NATIVE_INDEX 43
#define TABLE_REMOVE_NATIVE_INDEX 44
#define TABLE_FIRST_KEY_NATIVE_INDEX 45
#define TABLE_NEXT_KEY_NATIVE_INDEX 46

#define THREAD_CONSTRUCTOR_NATIVE_INDEX 47
#define THREAD_START_NATIVE_INDEX 48
#define THREAD_RUN_NATIVE_INDEX 49
#define THREAD_JOIN_NATIVE_INDEX 50

/* native method names */
#define OBJECT_CONSTRUCTOR_NATIVE_NAME "Object_constructor"
#define OBJECT_EQUALS_NATIVE_NAME "Object$equals$Object"
#define OBJECT_HASH_CODE_NATIVE_NAME "Object$hashCode"
#define OBJECT_TO_STRING_NATIVE_NAME "Object$toString"
#define OBJECT_NOTIFY_NATIVE_NAME "Object$notify"
#define OBJECT_NOTIFY_ALL_NATIVE_NAME "Object$notifyAll"
#define OBJECT_WAIT_NATIVE_NAME "Object$wait"
#define OBJECT_WAIT_INTEGER_NATIVE_NAME "Object$wait$Integer"

#define INTEGER_CONSTRUCTOR_NATIVE_NAME "Integer_constructor"
#define INTEGER_CONSTRUCTOR_INTEGER_NATIVE_NAME "Integer_constructor$Integer"
#define INTEGER_ADD_NATIVE_NAME "Integer$add$Integer"
#define INTEGER_SUBTRACT_NATIVE_NAME "Integer$subtract$Integer"
#define INTEGER_MULTIPLY_NATIVE_NAME "Integer$multiply$Integer"
#define INTEGER_DIVIDE_NATIVE_NAME "Integer$divide$Integer"
#define INTEGER_GREATER_THAN_NATIVE_NAME "Integer$greaterThan$Integer"
#define INTEGER_LESS_THAN_NATIVE_NAME "Integer$lessThan$Integer"
#define INTEGER_NOT_NATIVE_NAME "Integer$not"
#define INTEGER_MINUS_NATIVE_NAME "Integer$minus"
#define INTEGER_ADD_OP_NATIVE_NAME "Integer$operator+$Integer"
#define INTEGER_SUBTRACT_OP_NATIVE_NAME "Integer$operator-$Integer"
#define INTEGER_MULTIPLY_OP_NATIVE_NAME "Integer$operator*$Integer"
#define INTEGER_DIVIDE_OP_NATIVE_NAME "Integer$operator/$Integer"
#define INTEGER_GREATER_THAN_OP_NATIVE_NAME "Integer$operator>$Integer"
#define INTEGER_LESS_THAN_OP_NATIVE_NAME "Integer$operator<$Integer"
#define INTEGER_NOT_OP_NATIVE_NAME "Integer$operator!"
#define INTEGER_MINUS_OP_NATIVE_NAME "Integer$operator-"
#define INTEGER_EQUALS_NATIVE_NAME "Integer$equals$Object"
#define INTEGER_HASH_CODE_NATIVE_NAME "Integer$hashCode"
#define INTEGER_TO_STRING_NATIVE_NAME "Integer$toString"

#define STRING_CONSTRUCTOR_STRING_NATIVE_NAME "String_constructor$String"
#define STRING_LENGTH_NATIVE_NAME "String$length"
#define STRING_SUBSTR_NATIVE_NAME "String$substr$Integer$Integer"
#define STRING_TO_INTEGER_NATIVE_NAME "String$toInteger"
#define STRING_CONCAT_NATIVE_NAME "String$concat$String"
#define STRING_CONCAT_OP_NATIVE_NAME "String$operator+$String"
#define STRING_GREATER_THAN_NATIVE_NAME "String$operator>$String"
#define STRING_LESS_THAN_NATIVE_NAME "String$operator<$String"
#define STRING_HASH_CODE_NATIVE_NAME "String$hashCode"
#define STRING_EQUALS_NATIVE_NAME "String$equals$Object"
#define STRING_TO_STRING_NATIVE_NAME "String$toString"

#define TABLE_CONSTRUCTOR_NATIVE_NAME "Table_constructor"
#define TABLE_CONSTRUCTOR_INTEGER_NATIVE_NAME "Table_constructor$Integer"
#define TABLE_GET_NATIVE_NAME "Table$get$Object"
#define TABLE_PUT_NATIVE_NAME "Table$put$Object$Object"
#define TABLE_REMOVE_NATIVE_NAME "Table$remove$Object"
#define TABLE_FIRST_KEY_NATIVE_NAME "Table$firstKey"
#define TABLE_NEXT_KEY_NATIVE_NAME "Table$nextKey"

#define THREAD_CONSTRUCTOR_NATIVE_NAME "Thread_constructor"
#define THREAD_START_NATIVE_NAME "Thread$start"
#define THREAD_RUN_NATIVE_NAME "Thread$run"
#define THREAD_JOIN_NATIVE_NAME "Thread$join"

/* ref_set constants */
#define REF_SET_DEFAULT_INITIAL_CAPACITY 300
#define REF_SET_DEFAULT_LOAD_FACTOR 0.75

/* heap constants */
#define HEAP_DEFAULT_SIZE 67108864 /* 64MB */
#define HEAP_NUM_BUCKETS 16384

/* method area constants */
#define METHOD_AREA_INITIAL_BUFFER_SIZE 5
#define METHOD_AREA_BUFFER_RESIZE_FACTOR 2

/* class file constants */
#define NUM_PREDEFINED_CLASSES 5

#define OBJECT_PREDEFINED_CLASS_NAME "Object"
#define INTEGER_PREDEFINED_CLASS_NAME "Integer"
#define STRING_PREDEFINED_CLASS_NAME "String"
#define TABLE_PREDEFINED_CLASS_NAME "Table"
#define THREAD_PREDEFINED_CLASS_NAME "Thread"

#define NATIVE_ADDRESS 0

/* class file indices */
#define MAIN_BLOCK_DEC_ADDRESS 0
#define CLASS_TABLE_SIZE_ADDRESS 8

/* instruction constants */
#define INSTRUCTION_INITIAL_BUFFER_SIZE 5
#define INSTRUCTION_BUFFER_RESIZE_FACTOR 2
#define IN_INSTRUCTION_WHITESPACE " \r\n\t\f"

/* predefined integer constants */
#define INTEGER_MAX_INTEGER 2147483648U
#define INTEGER_MIN_INTEGER 0
#define INTEGER_TO_STRING_BUFFER_SIZE 12 /* strlen("-2147483648") == 11 */

/* predefined table constants */
#define TABLE_DEFAULT_INITIAL_CAPACITY 16
#define TABLE_DEFAULT_LOAD_FACTOR 0.75

#endif
