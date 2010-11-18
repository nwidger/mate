// Niels Widger
// Time-stamp: <17 Nov 2010 at 21:01:08 by nwidger on macros.local>

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

	monitors = 0;
	monitorCount = 0;
	
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

int LabelStackRecord::getMonitorCount() {
	return monitorCount;
}

MonitorRecord * LabelStackRecord::getMonitors() {
	return monitors;
}

int * LabelStackRecord::getMonitorsArray() {
	MonitorRecord *mr;
	int *array, i;

	if (monitors == 0)
		return 0;

	array = new int[monitorCount+1];

	for (i = 0, mr = monitors; mr != 0; mr = mr->getNext())
		array[i++] = mr->getPosition();

	array[monitorCount] = -1;

	return array;
}

void LabelStackRecord::setMonitors(MonitorRecord *m) {
	monitors = m;
}

void LabelStackRecord::incMonitorCount() {
	monitorCount++;
}

MonitorRecord::MonitorRecord(int p, MonitorRecord *n) {
	position = p;
	next = n;
}

MonitorRecord::~MonitorRecord() {
	if (next != 0)
		delete next;
}

int MonitorRecord::getPosition() {
	return position;
}

MonitorRecord * MonitorRecord::getNext() {
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

void LabelStack::addMonitor(int p) {
	LabelStackRecord *lsr;
	
	if (head == 0)
		return;

	lsr = head;

	lsr->setMonitors(new MonitorRecord(p, lsr->getMonitors()));
	lsr->incMonitorCount();
}

bool LabelStack::empty() {
	return head == 0;
}
