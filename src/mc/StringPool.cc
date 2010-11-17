// Niels Widger
// Time-stamp: <30 Nov 2009 at 19:23:15 by nwidger on macros.local>

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <string.h>
#include <iostream>
using namespace std;

#include "StringPool.h"

StringPoolRecord::StringPoolRecord(string *s, StringPoolRecord *n) {
	str = s;
	next = n;
}

StringPoolRecord::~StringPoolRecord() {
	delete str;
	if (next != 0)
		delete next;
}

StringPoolRecord * StringPoolRecord::getNext() {
	return next;
}

string * StringPoolRecord::getString() {
	return str;
}

StringPool::StringPool() {
	head = 0;
	equalsOp = string("equals");
	plusOp = string("operator+");
	minusOp = string("operator-");
	lessThanOp = string("operator<");
	greaterThanOp = string("operator>");
	timesOp = string("operator*");
	divideOp = string("operator/");
	negateOp = string("operator!");
	newline = string("\n");
	tab = string("\t");
	empty = string("");
	objectName = string("Object");
	integerName = string("Integer");
	stringName = string("String");
	tableName = string("Table");	
}

StringPool::~StringPool() {
	if (head != 0)
		delete head;
}

string * StringPool::newString(string *s) {
	if (*s == objectName)
		return &objectName;
	if (*s == integerName)
		return &integerName;
	if (*s == stringName)
		return &stringName;
	if (*s == tableName)
		return &tableName;
	return newString(*s);
}

string * StringPool::newString(string &s) {
	string *str = new string(s);
	head = new StringPoolRecord(str, head);
	return str;
}

string * StringPool::newString(const char *s) {
	string tmp = string(s);
	return newString(tmp);
}

string * StringPool::newString(char *s) {
	string tmp = string(s);
	return newString(tmp);
}

string * StringPool::newIdentifier(string &s) {
	return newString(s);
}

string * StringPool::newStringLiteral(char *s) {
	string tmp;
	
	if (strcmp(s, "newline") == 0) {
		return &newline;
	} else if (strcmp(s, "tab") == 0) {
		return &tab;
	} else if (strcmp(s, "") == 0) {
		return &empty;
	} else {
		s[strlen(s)-1] = '\0';
		tmp = string(s+1);
		return newString(tmp);
	}
}

string * StringPool::getOpString(string *s) {
	return getOpString(s->c_str());
}

string * StringPool::getOpString(const char *s) {
	switch (s[0]) {
	case '=':
		if (s[1] == '=')
			return &equalsOp;
		break;
	case '+':
		return &plusOp;
	case '-':
		return &minusOp;
	case '<':
		return &lessThanOp;
	case '>':
		return &greaterThanOp;
	case '*':
		return &timesOp;
	case '/':
		return &divideOp;
	case '!':
		return &negateOp;
	}

	return 0;
}

void StringPool::dump() {
	StringPoolRecord *spr;

	for (spr = head; spr != 0; spr = spr->getNext())
		cerr << *(spr->getString()) << '\n';
}
