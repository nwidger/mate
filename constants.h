/* Niels Widger
 * Time-stamp: <16 Mar 2013 at 11:16:40 by nwidger on macros.local>
 */

#ifndef _MVM_CONSTANTS_H
#define _MVM_CONSTANTS_H

#include <stdint.h>

/* garbage collector */
#define GARBAGE_COLLECTOR_DEFAULT_INTERVAL 5
#define GARBAGE_COLLECTOR_DEFAULT_TYPE serial_type

/* instructions */
#define NUM_INSTRUCTIONS 25

/* instruction opcodes */
#define MAX_OPCODE 0xfe

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
#define NEWREAL_OPCODE 0xfe

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
#define NEWREAL_NAME "newreal"

/* ref_set constants */
#define REF_SET_DEFAULT_INITIAL_CAPACITY 300
#define REF_SET_DEFAULT_LOAD_FACTOR 0.75

/* heap constants */
#define HEAP_DEFAULT_SIZE 67108864 /* 64MB */
#define HEAP_NUM_BUCKETS 65536

/* thread constants */
#define THREAD_NUM_FREE_BUCKETS 100
#define THREAD_MAX_FREE_BUCKET_SIZE 100

/* method area constants */
#define METHOD_AREA_INITIAL_BUFFER_SIZE 5
#define METHOD_AREA_BUFFER_RESIZE_FACTOR 2

/* class file constants */
#define NUM_PREDEFINED_CLASSES 6

#define OBJECT_PREDEFINED_CLASS_NAME "Object"
#define INTEGER_PREDEFINED_CLASS_NAME "Integer"
#define STRING_PREDEFINED_CLASS_NAME "String"
#define TABLE_PREDEFINED_CLASS_NAME "Table"
#define THREAD_PREDEFINED_CLASS_NAME "Thread"
#define REAL_PREDEFINED_CLASS_NAME "Real"

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

/* predefined real constants */
#define REAL_TO_STRING_BUFFER_SIZE 500

/* predefined table constants */
#define TABLE_DEFAULT_INITIAL_CAPACITY 16
#define TABLE_DEFAULT_LOAD_FACTOR 0.75
#define TABLE_USE_RWLOCK

/* class table constants */
#define CLASS_TABLE_INTEGER_CACHE_SIZE 65536
#define CLASS_TABLE_STRING_CACHE_SIZE 512

#ifdef DMP
/* dmp constants */
#define DMP_GARBAGE_COLLECTOR_LOAD_FACTOR 0.90

#define OBJECT_DMP_DEFAULT_OWNER -1 /* -1 == allocating thread */
#define OBJECT_DMP_DEFAULT_DEPTH 1

#define TABLE_DMP_DEFAULT_OWNER -1 /* -1 == allocating thread */

#define THREAD_DMP_DEFAULT_REDUCED_SERIAL_MODE full_mode
#define THREAD_DMP_DEFAULT_LOCK_COUNT          0
#define THREAD_DMP_DEFAULT_QUANTUM_SIZE        1000
#define THREAD_DMP_DEFAULT_INSTRUCTION_COUNTER 0

/** for accessing owner fields */
#define DMP_SHARED(x) (x == 0)
#define DMP_PRIVATE(x) (x != 0)

#endif

#endif
