/* Niels Widger
 * Time-stamp: <13 Mar 2011 at 22:54:13 by nwidger on macros.local>
 */

#ifndef _MVM_BARRIER_H
#define _MVM_BARRIER_H

#include <pthread.h>

struct barrier;

struct barrier * barrier_create(int p);

void barrier_destroy(struct barrier *b);

void barrier_clear(struct barrier *b);

int barrier_set_hook(struct barrier *b, void * (*h)(int, void *), void *a);

int barrier_clear_hook(struct barrier *b);

int barrier_await(struct barrier *b);

void barrier_reset(struct barrier *b);

void barrier_reset_parties(struct barrier *b, int p);

int barrier_get_waiting(struct barrier *b);

int barrier_get_parties(struct barrier *b);

int barrier_inc_parties(struct barrier *b);

int barrier_dec_parties(struct barrier *b);

#endif
