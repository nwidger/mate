// Niels Widger
// Time-stamp: <30 Nov 2009 at 19:19:10 by nwidger on macros.local>

#ifndef _TYPE_H
#define _TYPE_H

#include <string>

class ClassConstructorNode;
class ClassDecNode;
class ClassFieldNode;
class ClassMethodNode;
class ClassType;
class ReferenceType;
class Seq;
class StringPool;
class Type;
class TypeModule;
class VarDecNode;

class Type {
public:
	Type();
	Type(const char *n);
	virtual ~Type() { };
	void setTypeName(string *n);
	virtual void dump();
	virtual string * toString();
	virtual Type * analyze(void *param);
	virtual bool equal(Type *t);
	virtual string * getName();
	virtual bool canWidenTo(Type *t);
	virtual bool canNarrowTo(Type *t);
	virtual bool canCastTo(Type *t);
	virtual bool canAssignTo(Type *t);
protected:
	string typeName;
	int length;
};

class ReferenceType : public Type {
public:
	ReferenceType();
	ReferenceType(const char *n);
	bool equal(Type *t);
	virtual ReferenceType * getExtendType();
	virtual bool isDependant(ReferenceType *rt);
	virtual bool isCircular(ReferenceType *rt);
protected:
	ClassType *extendType;
};

class NullType : public ReferenceType {
public:
	NullType();
	void dump();
	string * toString();
	bool equal(Type *t);
	bool canWidenTo(Type *t);
};

class ClassType : public ReferenceType {
public:
	ClassType();
	ClassType(string *n);
	ClassType(string *n, string *e);
	~ClassType();
	string * getName();
	string * getExtendName();
	void addMethod(ClassMethodNode *cmn);
	ClassMethodNode * findMethod(ClassMethodNode *cmn, bool lookInExtends);
        ClassMethodNode * findMethod(string *n, Seq *a);
	Seq * findApplicableMethods(string *n, Seq *a, Seq *m);
	void addConstructor(ClassConstructorNode *ccn);
	ClassConstructorNode * findConstructor(ClassConstructorNode *ccn);
	ClassConstructorNode * findConstructor(Seq *a);
	Seq * findApplicableConstructors(Seq *a, Seq *m);	
	void addField(ClassFieldNode *cfn);
	ClassFieldNode * findField(string *n, bool lookInExtends);
	void addFields(Seq *f);
	void addExtendType(ClassType *e);
	void dump();
	string * toString();
	Type * analyze(void *param);
	bool equal(Type *t);
	bool canWidenTo(Type *t);
	bool canNarrowTo(Type *t);
	void encodeVMT();
	void encodeBody();
	void calculateOffsets();
	void calculateMethodOffsets();
	void calculateDepth();
	int getDepth();
	int getLength();
	int getOffset();
	void copyMethodTable(ClassMethodNode **mt);
	void setDeclaration(ClassDecNode *d);
	ClassDecNode * getDeclaration();
	bool getUseDefaultConstructor();	
protected:
	ClassDecNode *declaration;
	string *name;
	string *extends;
	Seq *constructors;
	Seq *fields;
	Seq *methods;
	int depth;
	int offset;
	bool useDefaultConstructor;
	ClassMethodNode **methodTable;
	int methodTableSize;
};

#endif
