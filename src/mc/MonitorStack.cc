// Niels Widger
// Time-stamp: <02 Dec 2010 at 13:55:18 by nwidger on macros.local>

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <iostream>
using namespace std;

#include "MonitorStack.h"

MonitorStackRecord::MonitorStackRecord(int p, MonitorStackRecord *n) {
	position = p;
	next = n;
}

MonitorStackRecord::~MonitorStackRecord() {
	if (next != 0)
		delete next;
}

int MonitorStackRecord::getPosition() {
	return position;
}

MonitorStackRecord * MonitorStackRecord::getNext() {
	return next;
}

MonitorStack::MonitorStack() {
	head = 0;
	count = 0;
}

MonitorStack::~MonitorStack() {
	if (head != 0) {
		delete head;
		count = 0;
	}
}

void MonitorStack::push(int p) {
	head = new MonitorStackRecord(p, head);
	count++;
}

int MonitorStack::peek() {
	if (count == 0)
		return -1;

	return head->getPosition();
}

int MonitorStack::pop() {
	int position;
	MonitorStackRecord *mr;
	
	if (count == 0)
		return -1;

	position = head->getPosition();
	mr = head;
	head = head->getNext();
	count--;
	
	return position;
}

int * MonitorStack::toArray() {
	MonitorStackRecord *mr;
	int i, *monitors;

	if (count == 0)
		return 0;

	monitors = new int[count+1];

	for (i = 0, mr = head; mr != 0; mr = mr->getNext())
		monitors[i++] = mr->getPosition();

	monitors[count] = -1;
	return monitors;
}

int MonitorStack::size() {
	return count;
}

bool MonitorStack::empty() {
	return count == 0;
}

void MonitorStack::clear() {
	while (count > 0)
		pop();
}
