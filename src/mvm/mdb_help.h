/* Niels Widger
 * Time-stamp: <08 Apr 2010 at 11:52:07 by nwidger on macros.local>
 */

#ifndef _MVM_MDB_HELP_H
#define _MVM_MDB_HELP_H

/* help text for mdb commands used by 'help' and 'info' commands. */

#define ADD_SYMBOL_FILE_SHORT_HELP "Load symbol table from symbol table file FILE."
#define ADD_SYMBOL_FILE_HELP ""

#define BT_SHORT_HELP "Alias for \"backtrace\" command."
#define BT_HELP ""

#define BACKTRACE_SHORT_HELP "Print backtrace of all stack frames."
#define BACKTRACE_HELP "Use of the 'full' qualifier also prints the values of the local variable\n\
array and operand stack of each frame.\n"

#define B_SHORT_HELP "Alias for \"break\" command."
#define B_HELP ""

#define BREAK_SHORT_HELP "Set a breakpoint at specified address or method."
#define BREAK_HELP "Argument may be an address, symbol or method name.\n\
If address is specified, break at instruction for that address.\n\
If symbol is specified, break at instruction at that label.\n\
If method is specified, break at start of code for that method.\n\
With no arg, uses current execution address of selected stack frame.\n\
This is useful for breaking on return to a stack frame.\n\
\n\
Multiple breakpoints at one place are permitted.\n"

#define CONTINUE_SHORT_HELP "Continue program being debugged, after breakpoint."
#define CONTINUE_HELP ""

#define D_SHORT_HELP "Alias for \"delete\" command."
#define D_HELP ""

#define DELETE_SHORT_HELP "Delete some breakpoints."
#define DELETE_HELP "Arguments are breakpoint numbers with spaces in between.\n\
To delete all breakpoints, give no argument.\n"

#define DIS_SHORT_HELP "Alias for \"disable\" command."
#define DIS_HELP ""

#define DISABLE_SHORT_HELP "Disable some breakpoints."
#define DISABLE_HELP "Arguments are breakpoint numbers with spaces in between.\n\
To disable all breakpoints, give no argument.\n\
A disabled breakpoint is not forgotten, but has no effect until reenabled.\n"

#define LIST_SHORT_HELP "Alias for \"disassemble\" command."
#define LIST_HELP ""

#define DISASSEMBLE_SHORT_HELP "Disassemble a specified section of the class file."
#define DISASSEMBLE_HELP "With no argument, disassemble ten more instructions after or around previous listing.\n\
One argument specifies an address or symbol, and ten instructions are listed from there.\n"

#define DOWN_SHORT_HELP "Select and print stack frame called by this one."
#define DOWN_HELP "An argument says how many frames down to go.\n"

#define ENABLE_SHORT_HELP "Enable some breakpoints."
#define ENABLE_HELP "Give breakpoint numbers (separated by spaces) as arguments.\n\
This is used to cancel the effect of the \"disable\" command.\n"

#define FINISH_SHORT_HELP "Execute until selected stack frame returns."
#define FINISH_HELP ""

#define F_SHORT_HELP "Alias for \"frame\" command."
#define F_HELP ""

#define FRAME_SHORT_HELP "Select and print a stack frame."
#define FRAME_HELP "With no argument, print the selected stack frame.  (See also \"info frame\").\n\
An argument specifies the frame to select.\n"

#define HELP_SHORT_HELP "Print list of commands."
#define HELP_HELP ""

#define INFO_SHORT_HELP "Generic command for showing things about the program being debugged."
#define INFO_HELP ""

#define JUMP_SHORT_HELP "Continue program being debugged at specified address."
#define JUMP_HELP "Give as argument either address or symbol for the address to start at.\n"

#define NEXTI_SHORT_HELP "Step one instruction, but proceed through method calls."
#define NEXTI_HELP ""

#define PRINT_SHORT_HELP "Print value of object reference REF."
#define PRINT_HELP "REF may be preceded by /FMT, where FMT is one of:\n\
\n\
o -- print REF as an Object (also lists fields).\n\
i -- print REF as an Integer.\n\
s -- print REF as a String.\n\
t -- print REF as a Table.\n"

#define RUN_SHORT_HELP "Start debugged program."
#define RUN_HELP "Input and output redirection with \">\" or \"<\" is allowed.\n"

#define S_SHORT_HELP "Alias for \"stepi\" command."
#define S_HELP ""

#define SET_SHORT_HELP "Write to class file in memory: set ADDRESS VALUE"
#define SET_HELP ""

#define STEPI_SHORT_HELP "Step one instruction exactly."
#define STEPI_HELP ""

#define UNTIL_SHORT_HELP "Execute until the program reaches an address greater than the current"
#define UNTIL_HELP "or a specified location (given as an address or symbol) within the current frame.\n"

#define U_SHORT_HELP "Alias for \"up\" command."
#define U_HELP ""

#define UP_SHORT_HELP "Select and print stack frame that called this one."
#define UP_HELP "An argument says how many frames up to go.\n"

#define QUIT_SHORT_HELP "Exit mdb."
#define QUIT_HELP ""

#define X_SHORT_HELP "Examine memory: x /FMT ADDRESS"
#define X_HELP "ADDRESS is an address in the class file.\n\
FMT is a format letter.\n\
Format letters are o(octal), x(hex), u(unsigned decimal),\n\
i(instruction), c(char) and s(string).\n"

#define INFO_BREAKPOINTS_SHORT_HELP "Status of user-settable breakpoints."
#define INFO_BREAKPOINTS_HELP ""

#define INFO_CLASS_TABLE_SHORT_HELP "List classes in class file."
#define INFO_CLASS_TABLE_HELP ""

#define INFO_FRAME_SHORT_HELP "All about selected stack frame."
#define INFO_FRAME_HELP ""

#define INFO_GARBAGE_COLLECTOR_SHORT_HELP "List garbage collector information."
#define INFO_GARBAGE_COLLECTOR_HELP ""

#define INFO_HEAP_SHORT_HELP "List heap information."
#define INFO_HEAP_HELP ""

#define INFO_INSTRUCTION_TABLE_SHORT_HELP "List instructions and their opcodes."
#define INFO_INSTRUCTION_TABLE_HELP ""

#define INFO_LOCAL_VARIABLE_ARRAY_SHORT_HELP "List local variable array of current stack frame."
#define INFO_LOCAL_VARIABLE_ARRAY_HELP ""

#define INFO_METHOD_AREA_SHORT_HELP "List information about currently loaded class file."
#define INFO_METHOD_AREA_HELP ""

#define INFO_NATIVE_METHOD_ARRAY_SHORT_HELP "List native methods and their indices."
#define INFO_NATIVE_METHOD_ARRAY_HELP ""

#define INFO_OPERAND_STACK_SHORT_HELP "List operand stack of current stack frame."
#define INFO_OPERAND_STACK_HELP ""

#define INFO_REGISTERS_SHORT_HELP "List of all registers and their contents."
#define INFO_REGISTERS_HELP ""

#define INFO_SYMBOL_TABLE_SHORT_HELP "List all symbols in symbol table."
#define INFO_SYMBOL_TABLE_HELP "Load symbol table with add-symbol-file command, or -s switch.\n"

#define INFO_VM_STACK_SHORT_HELP "Backtrace of the stack."
#define INFO_VM_STACK_HELP ""

#endif
