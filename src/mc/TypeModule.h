// Niels Widger
// Time-stamp: <30 Nov 2009 at 19:04:10 by nwidger on macros.local>

#ifndef _TYPEMODULE_H
#define _TYPEMODULE_H

class ClassTable;
class NullType;

class TypeModule {
public:
	TypeModule();
	~TypeModule();
	ClassTable * classTable();
	NullType * getNullType();
protected:
	ClassTable *cTable;
	NullType *nullType;
};

#endif
