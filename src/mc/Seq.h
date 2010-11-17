// Niels Widger
// Time-stamp: <30 Nov 2009 at 19:18:53 by nwidger on macros.local>

#ifndef _SEQ_H
#define _SEQ_H

#include <string>

class ClassMethodNode;
class ClassMethodNode;
class ClassTable;
class ClassType;
class Node;
class Seq;
class Type;
class TypeModule;
class VarNode;

class Seq {
public:
	Seq();
	Seq(Seq *l, Node *r);
	~Seq();
	Seq * add(Node *n);
	Seq * add(Seq *s);
	Seq * getLeft();
	Node * getRight();
	void clear();
	void dump();
	void encode();
	void toString();
	bool isEmpty();
	VarNode * findParameter(string *n);
	bool equalParameters(Seq *q);
	bool uniqueParameters();
	Seq * analyze(void *param);
	void addClasses();
	void analyzeClassBodies();
	int size();
	bool canAssignTo(Seq *p);
	Seq * getMostSpecific();
	Type * getRightType();
	void setMarked(bool m);
	Seq * getMarked();
	Seq * getParameters();
	string * getMungedParameters(string *m);
	void calculatePositions(int p);
	bool containsEqualMethod(ClassMethodNode *cmn);
	string * toStringParams();
	void toStringAuxParams(string *str);
protected:
	Seq *left;
	Node *right;
};

#endif
