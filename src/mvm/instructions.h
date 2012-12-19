/* Niels Widger
 * Time-stamp: <18 Dec 2012 at 20:05:24 by nwidger on macros.local>
 */

#include <inttypes.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "vm_stack.h"

/* these functions implement each instruction of the maTe language.
 * returns: 0 on success, non-zero on failure
 */

int aaload_instruction(uint32_t o, struct vm_stack *v);
int aastore_instruction(uint32_t o, struct vm_stack *v);
int aconst_null_instruction(uint32_t o, struct vm_stack *v);
int aload_instruction(uint32_t o, struct vm_stack *v);
int areturn_instruction(uint32_t o, struct vm_stack *v);
int astore_instruction(uint32_t o, struct vm_stack *v);
int checkcast_instruction(uint32_t o, struct vm_stack *v);
int dup_instruction(uint32_t o, struct vm_stack *v);
int dup_x1_instruction(uint32_t o, struct vm_stack *v);
int getfield_instruction(uint32_t o, struct vm_stack *v);
int goto_instruction(uint32_t o, struct vm_stack *v);
int ifeq_instruction(uint32_t o, struct vm_stack *v);
int in_instruction(uint32_t o, struct vm_stack *v);
int invokespecial_instruction(uint32_t o, struct vm_stack *v);
int invokenative_instruction(uint32_t o, struct vm_stack *v);
int invokevirtual_instruction(uint32_t o, struct vm_stack *v);
int monitorenter_instruction(uint32_t o, struct vm_stack *v);
int monitorexit_instruction(uint32_t o, struct vm_stack *v);
int new_instruction(uint32_t o, struct vm_stack *v);
int newint_instruction(uint32_t o, struct vm_stack *v);
int newreal_instruction(uint32_t o, struct vm_stack *v);
int newstr_instruction(uint32_t o, struct vm_stack *v);
int out_instruction(uint32_t o, struct vm_stack *v);
int pop_instruction(uint32_t a, struct vm_stack *v);
int putfield_instruction(uint32_t o, struct vm_stack *v);
int refcmp_instruction(uint32_t o, struct vm_stack *v);
int return_instruction(uint32_t o, struct vm_stack *v);

/* these functions decode each instruction of the maTe language.
 * returns: size of decoded instruction in words
 */

int aaload_decode(uint32_t a);
int aastore_decode(uint32_t a);
int aconst_null_decode(uint32_t a);
int aload_decode(uint32_t a);
int areturn_decode(uint32_t a);
int astore_decode(uint32_t a);
int checkcast_decode(uint32_t a);
int dup_decode(uint32_t a);
int dup_x1_decode(uint32_t a);
int getfield_decode(uint32_t a);
int goto_decode(uint32_t a);
int ifeq_decode(uint32_t a);
int in_decode(uint32_t a);
int invokespecial_decode(uint32_t a);
int invokenative_decode(uint32_t a);
int invokevirtual_decode(uint32_t a);
int monitorenter_decode(uint32_t o);
int monitorexit_decode(uint32_t o);
int new_decode(uint32_t a);
int newint_decode(uint32_t a);
int newreal_decode(uint32_t a);
int newstr_decode(uint32_t a);
int out_decode(uint32_t a);
int pop_decode(uint32_t a);
int putfield_decode(uint32_t a);
int refcmp_decode(uint32_t a);
int return_decode(uint32_t a);

/* these functions return the size of each instruction
 * returns: size of decoded instruction in words
 */

int aaload_decode_size(uint32_t a);
int aastore_decode_size(uint32_t a);
int aconst_null_decode_size(uint32_t a);
int aload_decode_size(uint32_t a);
int areturn_decode_size(uint32_t a);
int astore_decode_size(uint32_t a);
int checkcast_decode_size(uint32_t a);
int dup_decode_size(uint32_t a);
int dup_x1_decode_size(uint32_t a);
int getfield_decode_size(uint32_t a);
int goto_decode_size(uint32_t a);
int ifeq_decode_size(uint32_t a);
int in_decode_size(uint32_t a);
int invokespecial_decode_size(uint32_t a);
int invokenative_decode_size(uint32_t a);
int invokevirtual_decode_size(uint32_t a);
int monitorenter_decode_size(uint32_t o);
int monitorexit_decode_size(uint32_t o);
int new_decode_size(uint32_t a);
int newint_decode_size(uint32_t a);
int newreal_decode_size(uint32_t a);
int newstr_decode_size(uint32_t a);
int out_decode_size(uint32_t a);
int pop_decode_size(uint32_t a);
int putfield_decode_size(uint32_t a);
int refcmp_decode_size(uint32_t a);
int return_decode_size(uint32_t a);

/** adds all necessary instructions to the given instruction_table.
 *
 * @param i - the instruction_table to add to
 *
 * @return 0 on success, non-zero on failure
 */

int add_instructions(struct instruction_table *i);
