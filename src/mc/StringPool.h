// Niels Widger
// Time-stamp: <10 May 2011 at 21:02:14 by nwidger on macros.local>

#ifndef _STRINGPOOL_H
#define _STRINGPOOL_H

#include <string>

class StringPool;
class StringPoolRecord;

class StringPoolRecord {
public:
	StringPoolRecord(string *s, StringPoolRecord *n);
	~StringPoolRecord();
	StringPoolRecord * getNext();
	string * getString();
private:
	string *str;
	StringPoolRecord *next;
};

class StringPool {
public:
	StringPool();
	~StringPool();
	string * newString(string *s);
	string * newString(string &s);
	string * newString(const char *s);
	string * newString(char *s);
	string * newIdentifier(string &s);
	string * newStringLiteral(char *s);
	string * getOpString(string *s);
	string * getOpString(const char *s);
	void dump();
private:
	StringPoolRecord *head;
	string plusOp;
	string minusOp;
	string lessThanOp;
	string lessThanEqualOp;
	string greaterThanOp;
	string greaterThanEqualOp;
	string timesOp;
	string divideOp;
	string negateOp;
	string newline;
	string tab;
	string empty;
	string objectName;
	string integerName;
	string stringName;
	string tableName;
};

#endif
