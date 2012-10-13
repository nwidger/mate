// Niels Widger
// Time-stamp: <09 Oct 2012 at 19:03:24 by nwidger on macros.local>

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

void LabelStackRecord::addMonitor(int p) {
	MonitorRecord *mr;

	mr = new MonitorRecord(p, monitors);
	monitors = mr;

	monitorCount++;
}

void LabelStackRecord::removeMonitor() {
	if (monitors == 0)
		return;

	monitors = monitors->getNext();
	monitorCount--;
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
	if (head == 0)
		return;

	head->addMonitor(p);
}

void LabelStack::removeMonitor() {
	if (head == 0)
		return;

	head->removeMonitor();
}

int * LabelStack::getMonitors() {
	int i, count, *monitors;
	LabelStackRecord *lsr;
	MonitorRecord *mr;

	if (head == 0)
		return 0;

	lsr = head;
	count = lsr->getMonitorCount();

	if (count == 0)
		return 0;

	monitors = new int[count+1];

	for (i = 0, mr = lsr->getMonitors(); mr != 0; mr = mr->getNext())
		monitors[i++] = mr->getPosition();

	monitors[count] = -1;
	return monitors;
}

int * LabelStack::getAllMonitors() {
	int i, count, *monitors;
	LabelStackRecord *lsr;
	MonitorRecord *mr;

	if (head == 0)
		return 0;

	for (count = 0, lsr = head; lsr != 0; lsr = lsr->getNext())
		count += lsr->getMonitorCount();

	if (count == 0)
		return 0;

	monitors = new int[count+1];

	for (i = 0, lsr = head; lsr != 0; lsr = lsr->getNext()) {
		for (mr = lsr->getMonitors(); mr != 0; mr = mr->getNext())
			monitors[i++] = mr->getPosition();
	}

	monitors[count] = -1;
	return monitors;
}

bool LabelStack::empty() {
	return head == 0;
}
