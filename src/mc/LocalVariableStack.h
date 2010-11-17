// Niels Widger
// Time-stamp: <30 Nov 2009 at 19:19:47 by nwidger on macros.local>

#ifndef _LOCALVARIABLESTACK
#define _LOCALVARIABLESTACK

#include <string>

class GarbageRecord;
class LocalVarDecNode;
class LocalVariableStackRecord;
class Seq;
class VarNode;

class LocalVariableStackRecord {
public:
	LocalVariableStackRecord(int b, VarNode *v, int p, LocalVariableStackRecord *n);
	VarNode * getVariable();
	int getBlock();
	LocalVariableStackRecord * getNext();
protected:
	int block;
	VarNode *variable;
	LocalVariableStackRecord *next;
};

class GarbageRecord {
public:
	GarbageRecord(VarNode *v, GarbageRecord *n);
	~GarbageRecord();
protected:
	VarNode *variable;
	GarbageRecord *next;
};

class LocalVariableStack {
public:
	LocalVariableStack(int o);
	~LocalVariableStack();
	void enterBody();
	int leaveBody();	
	void enterBlock();
	int leaveBlock();
	int push(VarNode *v);
	int push(LocalVarDecNode *vdn);
	VarNode * peek();
	void pop();
	void pop(int i);
	VarNode * findVariable(string *name);
	VarNode * findVariableCurrentBlock(string *name);
	bool empty();
	void setOffset(int o);
	int getMax();
	int size();
	int getBlock();
	void clear();
protected:
	LocalVariableStackRecord *head;
	GarbageRecord *garbageHead;
	int block;
	int count;
	int offset;
	int max;
};

#endif
