// Niels Widger
// Time-stamp: <02 Dec 2010 at 13:18:19 by nwidger on macros.local>

#ifndef _MONITORSTACK
#define _MONITORSTACK

class MonitorStack;
class MonitorStackRecord;

class MonitorStackRecord {
public:
	MonitorStackRecord(int p, MonitorStackRecord *n);
	~MonitorStackRecord();
	int getPosition();
	MonitorStackRecord * getNext();
protected:
	int position;
	MonitorStackRecord *next;
};

class MonitorStack {
public:
	MonitorStack();
	~MonitorStack();
	void push(int p);
	int peek();
	int pop();
	int * toArray();
	int size();
	bool empty();
	void clear();
protected:
	MonitorStackRecord *head;
	int count;
};

#endif
