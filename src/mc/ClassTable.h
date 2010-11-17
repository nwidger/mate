// Niels Widger
// Time-stamp: <22 Sep 2010 at 21:33:40 by nwidger on macros.local>

#ifndef _CLASSTABLE_H
#define _CLASSTABLE_H

#include <string>
using namespace std;

class ClassConstructorNode;
class ClassDecNode;
class ClassMethodNode;
class ClassTable;
class ClassTableRecord;
class ClassType;
class LocalVariableStack;
class Seq;
class StringPool;
class Type;
class VarNode;

class ClassTableRecord {
public:
	ClassTableRecord(ClassType *c, ClassTableRecord *n);
	~ClassTableRecord();
	ClassType * getClass();
	ClassTableRecord * getNext();
	void setNext(ClassTableRecord *n);
private:
	ClassType *classType;
	ClassTableRecord *next;
};

class ClassTable {
public:
	ClassTable();
	~ClassTable();
	int add(ClassType *ct);
	ClassType * find(const char *n);	
	ClassType * find(string *n);
	void dump();
	void encodeVMTs();
	void encodeBodies();
	void analyze(Seq *ast);
	void analyzeClassDecs();
	void analyzeClassBodies();
	void analyzeBodies();
	void calculateDepth();
	void calculateLength();
	void calculateMethodOffsets();
	ClassType * getObject();
private:
	ClassTableRecord *head;
	int size;
	ClassType *objectClass;
};

#endif
