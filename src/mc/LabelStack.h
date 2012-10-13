// Niels Widger
// Time-stamp: <09 Oct 2012 at 19:03:50 by nwidger on macros.local>

#ifndef _LABELSTACK
#define _LABELSTACK

#include <string>

class StringPool;
class LabelStack;
class LabelStackRecord;
class MonitorRecord;

class MonitorRecord {
public:
	MonitorRecord(int p, MonitorRecord *n);
	~MonitorRecord();
	int getPosition();
	MonitorRecord * getNext();
protected:
	int position;
	MonitorRecord *next;
};

class LabelStackRecord {
public:
	LabelStackRecord(string *entry, string *exit, LabelStackRecord *n);
	~LabelStackRecord();
	string * getEntryLabel();
	string * getExitLabel();
	LabelStackRecord * getNext();
	MonitorRecord * getMonitors();
	void addMonitor(int p);
	void removeMonitor();
	int getMonitorCount();
protected:
	string *entryLabel;
	string *exitLabel;
	MonitorRecord *monitors;
	int monitorCount;
	LabelStackRecord *next;
};

class LabelStack {
public:
	LabelStack(const char *p, int c);
	~LabelStack();
	LabelStackRecord * push();
	LabelStackRecord * peek();
	LabelStackRecord * pop();
	void addMonitor(int p);
	void removeMonitor();
	int * getMonitors();	
	int * getAllMonitors();	
	bool empty();
protected:
	LabelStackRecord *head;
	string prefix;
	int counter;
};

#endif
