// Niels Widger
// Time-stamp: <22 Sep 2010 at 22:22:03 by nwidger on macros.local>

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <iostream>
using namespace std;

#include "ClassTable.h"
#include "globals.h"
#include "Node.h"
#include "Seq.h"
#include "StringPool.h"
#include "Type.h"
#include "TypeModule.h"

Type::Type() {
	typeName = string("");
	length = 0;
}

Type::Type(const char *n) {
	typeName = string(n);
	length = 0;
}

void Type::setTypeName(string *n) {
	typeName = string(*n);
}

void Type::dump() {
}

string * Type::toString() {
	return &typeName;
}

Type * Type::analyze(void *param) {
	return (Type *)this;
}

bool Type::equal(Type *t) {
	return false;
}

string * Type::getName() {
	return stringPool->newString("Type");
}

bool Type::canWidenTo(Type *t) {
	return false;
}

bool Type::canNarrowTo(Type *t) {
	return false;
}

bool Type::canCastTo(Type *t) {
	if (this->equal(t))
		return true;
	if (this->canWidenTo(t))
		return true;
	if (this->canNarrowTo(t))
		return true;

	return false;
}

bool Type::canAssignTo(Type *t) {
	if (this->equal(t))
		return true;
	if (this->canWidenTo(t))
		return true;

	return false;
}

ReferenceType::ReferenceType() : Type("ReferenceType") {
	extendType = 0;
}

ReferenceType::ReferenceType(const char *n) : Type(n) {
	extendType = 0;
}

bool ReferenceType::equal(Type *t) {
	ReferenceType *rt;

	return (rt = dynamic_cast<ReferenceType *>(t)) != 0;
}

ReferenceType * ReferenceType::getExtendType() {
	return extendType;
}

bool ReferenceType::isDependant(ReferenceType *rt) {
	ReferenceType *r;

	for (r = this; r != 0; r = r->getExtendType()) {
		if (*(r->getName()) == *(rt->getName()))
			return true;
	}

	return false;
}

bool ReferenceType::isCircular(ReferenceType *rt) {
	return isDependant(rt) && rt->isDependant(this);
}

NullType::NullType() : ReferenceType("NullType") {
}

void NullType::dump() {
	cerr << *(Type::toString()) << '\n';
}

string * NullType::toString() {
	return stringPool->newString("null");
}

bool NullType::equal(Type *t) {
	NullType *nt;

	return (nt = dynamic_cast<NullType *>(t)) != 0;
}

bool NullType::canWidenTo(Type *t) {
	ReferenceType *rt;

	return (rt = dynamic_cast<ReferenceType *>(t)) != 0;	
}

ClassType::ClassType() : ReferenceType("ClassType") {
	declaration = 0;
	name = 0;
	extends = &objectName;
	extendType = 0;
	constructors = new Seq(0, 0);
	fields = new Seq(0, 0);
	methods = new Seq(0, 0);
	depth = 0;
	offset = 0;
	useDefaultConstructor = true;
	methodTable = 0;
	methodTableSize = 0;
}

ClassType::ClassType(string *n) : ReferenceType("ClassType") {
	declaration = 0;	
	name = n;
	extends = &objectName;
	extendType = 0;
	constructors = new Seq(0, 0);
	fields = new Seq(0, 0);
	methods = new Seq(0, 0);
	depth = 0;
	offset = 0;
	useDefaultConstructor = true;
	methodTable = 0;
	methodTableSize = 0;	
}

ClassType::ClassType(string *n, string *e) : ReferenceType("ClassType") {
	declaration = 0;	
	name = n;
	extends = e;
	extendType = 0;
	constructors = new Seq(0, 0);
	fields = new Seq(0, 0);
	methods = new Seq(0, 0);
	depth = 0;
	offset = 0;
	useDefaultConstructor = true;
	methodTable = 0;
	methodTableSize = 0;	
}

ClassType::~ClassType() {
	if (constructors != 0)
		delete constructors;
	if (fields != 0)
		delete fields;
	if (methods != 0)
		delete methods;
	if (methodTable != 0)
		delete[] methodTable;
}

void ClassType::dump() {
	if (extendType != 0) {
		cerr << "class " << *(toString()) 
		     << " extends " << *(extendType->toString()) << "\n";
	} else{
		cerr << "class " << *(toString()) << "\n";
	}
// 	if (constructors != 0 && constructors->isEmpty() == false) {
// 		constructors->toString();
// 	}
// 	if (fields != 0 && fields->isEmpty() == false) {
// 		fields->toString();
// 	}
// 	if (methods != 0 && methods->isEmpty() == false) {
// 		methods->toString();
// 	}

// 	cerr << *name << ":\n";
// 	if (extendType != 0)
// 		cerr << 0 << ": " << *(extendType->toString()) << '\n';
// 	else
// 		cerr << 0 << ": 0\n";

// 	if (methodTable != 0 && methodTableSize > 0) {
// 		for (int i = 0; i < methodTableSize; i++)
// 			cerr << (i+2)*4 << ": " << *(methodTable[i]->getName()) << '\n';
// 	}
}

string * ClassType::getName() {
	return stringPool->newString(name);
}

string * ClassType::getExtendName() {
	return extends;
}

void ClassType::addMethod(ClassMethodNode *cmn) {
	if (findMethod(cmn, false) != 0) {
		cerr << cmn->getLineNumber() << ": method "
		     << *(cmn->getName()) << " with same signature already "
		     << "declared in class " << *name << '\n';
		err = true;
		return;
	}
	
	methods->add(cmn);
}

Seq * ClassType::findApplicableMethods(string *n, Seq *a, Seq *am) {
	Seq *m;
	ClassMethodNode *cmn;

	for (m = methods; m != 0 && m->getRight() != 0; m = m->getLeft()) {
		cmn = (ClassMethodNode *)(m->getRight());
		if (*n == *(cmn->getName()) &&
		    a->size() == cmn->getParameters()->size() &&
		    a->canAssignTo(cmn->getParameters()) &&
		    !am->containsEqualMethod(cmn)) {
			am->add(cmn);
		}
	}

	if (extendType != 0)
		am = extendType->findApplicableMethods(n, a, am);

	return am;
}

ClassMethodNode * ClassType::findMethod(string *n, Seq *a) {
	Seq *applicableMethods, *mostSpecificMethods;
	ClassMethodNode *cmn;

	applicableMethods = findApplicableMethods(n, a, new Seq(0, 0));

	if (applicableMethods->size() == 1) {
		cmn = (ClassMethodNode *)applicableMethods->getRight();
		
		applicableMethods->clear();
		
		return cmn;
	} else if (applicableMethods->size() > 1) {
		mostSpecificMethods = applicableMethods->getMostSpecific();

		if (mostSpecificMethods->size() > 1) {
			mostSpecificMethods->clear();
			applicableMethods->clear();
			
			return 0;
		}

		cmn = (ClassMethodNode *)mostSpecificMethods->getRight();
		
		mostSpecificMethods->clear();
		applicableMethods->clear();
		
		return cmn;
	}

	return 0;
}

ClassMethodNode * ClassType::findMethod(ClassMethodNode *cmn, bool lookInExtends) {
	Seq *m;
	ClassMethodNode *cmn2;

	for (m = methods; m != 0 && m->getRight() != 0; m = m->getLeft()) {
		cmn2 = (ClassMethodNode *)(m->getRight());
		if (cmn->equal(cmn2))
			return cmn2;
	}

	if (lookInExtends == true && extendType != 0)
		return extendType->findMethod(cmn, lookInExtends);

	return 0;
}

void ClassType::addConstructor(ClassConstructorNode *ccn) {
	if (findConstructor(ccn) != 0) {
		cerr << ccn->getLineNumber() << ": constructor "
		     << "with same signature already "
		     << "declared in class " << *name << '\n';
		err = true;
		return;
	}

	useDefaultConstructor = false;
	
	constructors->add(ccn);
}

ClassConstructorNode * ClassType::findConstructor(ClassConstructorNode *ccn) {
	Seq *c;
	ClassConstructorNode *ccn2;

	for (c = constructors; c != 0 && c->getRight() != 0; c = c->getLeft()) {
		ccn2 = (ClassConstructorNode *)(c->getRight());
		if (ccn->equal(ccn2))
			return ccn2;
	}

	return 0;
}

ClassConstructorNode * ClassType::findConstructor(Seq *a) {
	Seq *applicableConstructors, *mostSpecificConstructors;
	Seq *noArgs = new Seq(0,0);
	ClassConstructorNode noArgConst = ClassConstructorNode(&objectName, noArgs);
	ClassConstructorNode *ccn;

	if (a->size() == 0 && useDefaultConstructor == true)
		return types->classTable()->getObject()->findConstructor(&noArgConst);

	applicableConstructors = findApplicableConstructors(a, new Seq(0, 0));

	if (applicableConstructors->size() == 1) {
		ccn = (ClassConstructorNode *)applicableConstructors->getRight();
		applicableConstructors->clear();
		return ccn;
	} else if (applicableConstructors->size() > 1) {
		mostSpecificConstructors = applicableConstructors->getMostSpecific();

		if (mostSpecificConstructors->size() > 1) {
			mostSpecificConstructors->clear();
			applicableConstructors->clear();
			return 0;
		}

		ccn = (ClassConstructorNode *)mostSpecificConstructors->getRight();
		
		mostSpecificConstructors->clear();
		applicableConstructors->clear();
			
		return ccn;
	}

	return 0;
}

Seq * ClassType::findApplicableConstructors(Seq *a, Seq *ac) {
	Seq *c;
	ClassConstructorNode *ccn;

	for (c = constructors; c != 0 && c->getRight() != 0; c = c->getLeft()) {
		ccn = (ClassConstructorNode *)(c->getRight());
		if (a->size() == ccn->getParameters()->size() &&
		    a->canAssignTo(ccn->getParameters())) {
			ac->add(ccn);
		}
	}

	return ac;
}

void ClassType::addField(ClassFieldNode *cfn) {
	if (findField(cfn->getName(), false) == 0) {
		fields->add(cfn);
	} else {
		cerr << cfn->getLineNumber() << ": class field " << *(cfn->getName())
		     << " already declared in class " << *name << '\n';
		err = true;
	}
}

ClassFieldNode * ClassType::findField(string *n, bool lookInExtends) {
	Seq *s;
	ClassFieldNode *cfn;

	for (s = fields; s != 0; s = s->getLeft()) {
		cfn = (ClassFieldNode *)(s->getRight());

		if (cfn != 0 && *(cfn->getName()) == *n)
			return cfn;
	}

	if (extendType != 0 && lookInExtends)
		return extendType->findField(n, lookInExtends);
	
	return 0;
}

void ClassType::addFields(Seq *f) {
	VarNode *vn;

	if (f->getLeft() != 0)
		addFields(f->getLeft());
	
	if (f->getRight() != 0 && 
	    (vn = dynamic_cast<VarNode *>(f->getRight())) != 0) {
		return addField(new ClassFieldNode(vn));
	}
}

void ClassType::addExtendType(ClassType *e) {
	extendType = e;
}

string * ClassType::toString() {
	return stringPool->newString(name);
}

Type * ClassType::analyze(void *param) {
	ClassType *ct;
	long line;

	line = (long)param;

	if ((ct = types->classTable()->find(name)) == 0) {
		cerr << line << ": class " << *name << " has not been declared!\n";
		err = true;
		// DELETE
		delete this;
		return types->classTable()->getObject();
	}

	return ct;
}

bool ClassType::equal(Type *t) {
	ClassType *ct;

	return (ct = dynamic_cast<ClassType *>(t)) != 0 &&
		*(ct->getName()) == *name;
}

bool ClassType::canWidenTo(Type *t) {
	ClassType *ct;
	
	if ((ct = dynamic_cast<ClassType *>(t)) != 0) {
		if (*(ct->getName()) == objectName)
			return true;
		if (isDependant(ct))
			return true;
	}

	return false;
}

bool ClassType::canNarrowTo(Type *t) {
	ClassType *ct;
	
	if ((ct = dynamic_cast<ClassType *>(t)) != 0) {
		if (*name == objectName)
			return true;
		if (ct->isDependant(this))
			return true;
	}

	return false;	
}

void ClassType::calculateDepth() {
	ClassType *ct;

	depth = 0;
	for (ct = extendType; ct != 0; ct = ct->extendType) {
		depth++;
	}
}

int ClassType::getDepth() {
	return depth;
}

void ClassType::calculateOffsets() {
	ClassFieldNode *cfn;
	Seq *s;
	int o;

	if (extendType == 0)
		offset = 0;
	else
		offset = extendType->offset + extendType->fields->size();

	o = offset + fields->size() - 1;
	for (s = fields; s != 0; s = s->getLeft()) {
		cfn = (ClassFieldNode *)s->getRight();
		if (cfn != 0)
			cfn->setOffset(o);
		o--;
	}
}

void ClassType::calculateMethodOffsets() {
	ClassMethodNode *cmn, *cmn2;
	Seq *s;
	int o;

	if (*name == objectName) {
		methodTable = new ClassMethodNode*[methods->size()];

		o = 0;
		for (s = methods; s != 0 && s->getRight() != 0; s = s->getLeft()) {
			cmn = (ClassMethodNode *)s->getRight();
			cmn->setOffset(o);
			methodTable[o] = cmn;
			o++;
		}
		
		methodTableSize = methods->size();
	} else {
		methodTable = new ClassMethodNode*[methods->size() +
					  extendType->methodTableSize];
		extendType->copyMethodTable(methodTable);		

		o = extendType->methodTableSize;

		for (s = methods; s != 0 && s->getRight() != 0; s = s->getLeft()) {
			cmn = (ClassMethodNode *)s->getRight();
			if ((cmn2 = extendType->findMethod(cmn, true)) != 0) {
				cmn->setOffset(cmn2->getOffset());
				methodTable[cmn2->getOffset()] = cmn;
			} else {
				cmn->setOffset(o);
				methodTable[o] = cmn;
				o++;
			}
		}

		methodTableSize = o;
	}
}

int ClassType::getLength() {
	return length;
}

void ClassType::copyMethodTable(ClassMethodNode **mt) {
	for (int i = 0; i < methodTableSize; i++) {
		mt[i] = methodTable[i];
	}
}

void ClassType::setDeclaration(ClassDecNode *d) {
	declaration = d;
}

ClassDecNode * ClassType::getDeclaration() {
	return declaration;
}

bool ClassType::getUseDefaultConstructor() {
	return useDefaultConstructor;
}
