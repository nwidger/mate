/* Niels Widger
 * Time-stamp: <19 Nov 2010 at 19:41:38 by nwidger on macros.local>
 */

#ifndef _MVM_INVOKEMETHOD_H
#define _MVM_INVOKEMETHOD_H

#include <inttypes.h>

/** invokes a method on the object with reference r.  The method to be
 * invoked must be named m and take n arguments.  The class type of
 * the nth argument must correspond to the nth argument after n.  When
 * this function returns, the invoked method's arguments shall have
 * been popped off the previous frame's operand_stack, the given
 * method shall have been executed, its return value pushed onto the
 * previous frame's operand_stack, and the pc set to a.
 *
 * @param r - the reference of the object to invoke the method on
 * @param a - the return address of the method
 * @param m - the name of the method to invoke as a c-string
 * @param n - the number of arguments to this method
 * ... - the class type of each argument, each as as c-string
 *
 * examples:
 * invoking the two-arg substr method on the reference 1:
 * invoke_virtual_method_by_name(1, ret, "substr", 2, "Integer", "Integer");
 *
 * invoking the no-arg hashCode method on the reference 2:
 * invoke_virtual_method_by_name(2, ret, "hashCode", 0);
 * 
 * @return 0 on success, non-zero on failure
 */

int invoke_virtual_method_by_name(int r, uint32_t a, char *m, int n, ...);

/** invokes a method on the object with reference r.  The method to be
 * invoked is the method at method table index i of the object's class
 * type.  When this function returns, n arguments shall have been
 * popped off the previous frame's operand_stack, the given method
 * shall have been executed, its return value pushed onto the previous
 * frame's operand_stack, and the pc set to a.
 *
 * @param r - the reference of the object to invoke the method on
 * @param i - the method table index of the method to be invoked
 * @param n - the number of arguments to the method
 * @param a - the return address of the method
 *
 * @return 0 on success, non-zero on failure
 */

int invoke_virtual_method(int r, int i, int n, uint32_t a);

/** invokes a native method.  The native method to be invoked is the
 * native method at native method array index i.  When this function
 * returns, n arguments shall have been popped off the previous
 * frame's operand_stack, the given native method shall have been
 * executed, its return value pushed onto the previous frame's
 * operand_stack, and the pc set to r.
 *
 * @param i - the native method array index
 * @param r - the return address of the method
 *
 * @return 0 on success, non-zero on failure
 */

int invoke_native_method(int i, uint32_t r);

/** invokes the method at address a that takes n arguments and has a
 * max locals count of m.  When this function returns, n arguments
 * shall have been popped off the previous frame's operand_stack, the
 * given method shall have been executed, its return value pushed onto
 * the previous frame's operand_stack, and the pc set to r.
 *
 * @param e - the method name as a c-string 
 * @param a - the address of the method
 * @param b - the end address of the method
 * @param n - the number of arguments to the method
 * @param m - the max locals counts of the method
 * @param r - the return address of the method
 *
 * @return 0 on success, non-zero on failure
 */

int invoke_method(char *e, uint32_t a, uint32_t b, int n, int m, uint32_t r);

#endif
