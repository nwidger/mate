// Niels Widger
// Time-stamp: <30 Nov 2009 at 19:22:49 by nwidger on macros.local>

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <sstream>
#include <iostream>
using namespace std;

#include "globals.h"
#include "LabelStack.h"
#include "StringPool.h"

LabelStackRecord::LabelStackRecord(string *entry, string *exit, LabelStackRecord *n) {
	entryLabel = entry;
	exitLabel = exit;
	next = n;
}

LabelStackRecord::~LabelStackRecord() {
	if (next != 0)
		delete next;
}

string * LabelStackRecord::getEntryLabel() {
	return entryLabel;
}

string * LabelStackRecord::getExitLabel() {
	return exitLabel;
}

LabelStackRecord * LabelStackRecord::getNext() {
	return next;
}

LabelStack::LabelStack(const char *p, int c) {
	prefix = string(p);
	counter = c;
	head = 0;
}

LabelStack::~LabelStack() {
	if (head != 0)
		delete head;
}

LabelStackRecord * LabelStack::push() {
	string *entry, *exit, str;
	ostringstream exitO;
	ostringstream entryO;

	entryO << prefix << counter;
	str = entryO.str();
	entry = stringPool->newString(str);
	exitO << prefix << (counter+1);
	str = exitO.str();
	exit = stringPool->newString(str);

	head = new LabelStackRecord(entry, exit, head);
	
	counter += 2;
	return head;
}

LabelStackRecord * LabelStack::pop() {
	LabelStackRecord *lsr;

	if (head == 0)
		return 0;
	
	lsr = head;
	head = head->getNext();

	return lsr;
}

LabelStackRecord * LabelStack::peek() {
	return head;
}

bool LabelStack::empty() {
	return head == 0;
}
