// Niels Widger
// Time-stamp: <30 Nov 2009 at 19:22:54 by nwidger on macros.local>

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <iostream>
using namespace std;

#include "globals.h"
#include "Node.h"
#include "LocalVariableStack.h"
#include "Seq.h"

LocalVariableStackRecord::LocalVariableStackRecord(int b, VarNode *v, int p, LocalVariableStackRecord *n) {
	block = b;
	
	variable = v;
	variable->setPosition(p);
	
	next = n;
}

int LocalVariableStackRecord::getBlock() {
	return block;
}

VarNode * LocalVariableStackRecord::getVariable() {
	return variable;
}

LocalVariableStackRecord * LocalVariableStackRecord::getNext() {
	return next;
}

GarbageRecord::GarbageRecord(VarNode *v, GarbageRecord *n) {
	variable = v;
	next = n;
}

GarbageRecord::~GarbageRecord() {
	delete variable;
	if (next != 0)
		delete next;
}

LocalVariableStack::LocalVariableStack(int o) {
	offset = o;
	block = 0;
	count = 0;
	max = 0;
	head = 0;
	garbageHead = 0;
}

LocalVariableStack::~LocalVariableStack() {
	while (head != 0)
		pop();
	if (garbageHead != 0)
		delete garbageHead;
}

void LocalVariableStack::enterBody() {
	clear();
}

int LocalVariableStack::leaveBody() {
	int tmp = max;

	clear();
	
	return tmp;
}

void LocalVariableStack::enterBlock() {
	block++;
}

int LocalVariableStack::leaveBlock() {
	int i = 0;
	
	while (head != 0 && head->getBlock() == block) {
		pop();
		i++;
	}
	
	block--;

	return i;
}

int LocalVariableStack::push(VarNode *v) {
	if (findVariableCurrentBlock(v->getName())) {
		cerr << v->getLineNumber() << ": redeclaration of local variable " 
		     << *(v->getName()) << "!\n";
		err = true;
		return -1;
	}
	
	head = new LocalVariableStackRecord(block, v, count+offset, head);
	
	count++;

	if (count > max)
		max = count;

	return 1;
}

int LocalVariableStack::push(LocalVarDecNode *lvdn) {
	Seq *s;

	for (s = lvdn->getVariables(); s != 0 && s->getRight() != 0; s = s->getLeft()) {
		if (push((VarNode *)s->getRight()) == -1)
			return -1;
	}

	return s->size();
}

VarNode * LocalVariableStack::peek() {
	return head->getVariable();
}

void LocalVariableStack::pop() {
	LocalVariableStackRecord *tmp;
	
	if (head != 0) {
		garbageHead = new GarbageRecord(head->getVariable(), garbageHead);
		tmp = head;
		head = head->getNext();
		delete tmp;
		count--;
	}
}

void LocalVariableStack::pop(int i) {
	while (i-- > 0)
		pop();
}

VarNode * LocalVariableStack::findVariable(string *name) {
	LocalVariableStackRecord *p;

	for (p = head; p != 0; p = p->getNext()) {
		if (*(p->getVariable()->getName()) == *name)
			return p->getVariable();
	}

	return 0;
}

VarNode * LocalVariableStack::findVariableCurrentBlock(string *name) {
	LocalVariableStackRecord *p;

	for (p = head; p != 0; p = p->getNext()) {
		if (*(p->getVariable()->getName()) == *name &&
		    p->getBlock() == block)
			return p->getVariable();
	}

	return 0;
}

int LocalVariableStack::getMax() {
	return max;
}

bool LocalVariableStack::empty() {
	return head == 0;
}

void LocalVariableStack::setOffset(int o) {
	offset = o;
}

int LocalVariableStack::size() {
	return count;
}

int LocalVariableStack::getBlock() {
	return block;
}

void LocalVariableStack::clear() {
	while (head != 0)
		pop();

	head = 0;
	block = 0;
	count = 0;
	max = 0;
}
