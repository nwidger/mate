// Niels Widger
// Time-stamp: <30 Nov 2009 at 19:19:31 by nwidger on macros.local>

#ifndef _LABELSTACK
#define _LABELSTACK

#include <string>

class StringPool;
class LabelStack;
class LabelStackRecord;

class LabelStackRecord {
public:
	LabelStackRecord(string *entry, string *exit, LabelStackRecord *n);
	~LabelStackRecord();
	string * getEntryLabel();
	string * getExitLabel();
	LabelStackRecord * getNext();
protected:
	string *entryLabel;
	string *exitLabel;
	LabelStackRecord *next;
};

class LabelStack {
public:
	LabelStack(const char *p, int c);
	~LabelStack();
	LabelStackRecord * push();
	LabelStackRecord * peek();
	LabelStackRecord * pop();
	bool empty();
protected:
	LabelStackRecord *head;
	string prefix;
	int counter;
};

#endif
