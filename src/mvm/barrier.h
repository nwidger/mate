/* Niels Widger
 * Time-stamp: <29 Jan 2011 at 22:53:09 by nwidger on macros.local>
 */

#ifndef _MVM_BARRIER_H
#define _MVM_BARRIER_H

struct barrier;

struct barrier * barrier_create(int p);

void barrier_destroy(struct barrier *b);

void barrier_clear(struct barrier *b);

int barrier_await(struct barrier *b);

void barrier_reset(struct barrier *b);

void barrier_reset_parties(struct barrier *b, int p);

int barrier_get_waiting(struct barrier *b);

int barrier_get_parties(struct barrier *b);

#endif
