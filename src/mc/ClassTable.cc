// Niels Widger
// Time-stamp: <22 Sep 2010 at 22:48:31 by nwidger on macros.local>

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <string.h>
#include <iostream>
using namespace std;

#include <constants.h>

#include "ClassTable.h"
#include "globals.h"
#include "LocalVariableStack.h"
#include "Node.h"
#include "Seq.h"
#include "StringPool.h"
#include "Type.h"

ClassTableRecord::ClassTableRecord(ClassType *c, ClassTableRecord *n) {
	classType = c;
	next = n;
}

ClassTableRecord::~ClassTableRecord() {
	delete classType;
	if (next != 0)
		delete next;
}

ClassType * ClassTableRecord::getClass() {
	return classType;
}

void ClassTableRecord::setNext(ClassTableRecord *n) {
	next = n;
}

ClassTableRecord * ClassTableRecord::getNext() {
	return next;
}

ClassTable::ClassTable() {
	head = 0;
	size = 0;
	objectClass = 0;
}

ClassTable::~ClassTable() {
	delete head;
}

ClassType * ClassTable::find(const char *n) {
	string str = string(n);
	return find(&str);
}

ClassType * ClassTable::find(string *n) {
	ClassTableRecord *p;

	for (p = head; p != 0; p = p->getNext()) {
		if (*n == *(p->getClass()->getName()))
			return p->getClass();
	}

	return 0;
}

int ClassTable::add(ClassType *c) {
	ClassTableRecord *p;
	
	string *name = c->getName();
	
	if (find(name) != 0)
		return 0;

	if (*name == objectName)
		objectClass = c;

	if (head == 0) {
		head = new ClassTableRecord(c, head);
	} else {
		for (p = head; p->getNext() != 0; p = p->getNext());
		p->setNext(new ClassTableRecord(c, 0));
	}

	size++;
	return 1;
}

void ClassTable::dump() {
	ClassTableRecord *p;

	for (p = head; p != 0; p = p->getNext()) {
		p->getClass()->dump();
	}
}

void ClassTable::encodeVMTs() {
	ClassTableRecord *p;

 	cout << size << '\n';
	
	for (p = head; p != 0; p = p->getNext()) {
		cout << "$" << *(p->getClass()->getName()) << " \"" << *(p->getClass()->getName()) << "\"\n";
	}

	for (p = head; p != 0; p = p->getNext()) {
		p->getClass()->encodeVMT();
	}
}

void ClassTable::encodeBodies() {
	ClassTableRecord *p;
	
	for (p = head; p != 0; p = p->getNext()) {
		p->getClass()->encodeBody();
	}
}

void ClassTable::calculateDepth() {
	ClassTableRecord *p;
	ClassType *ct;

	for (p = head; p != 0; p = p->getNext()) {
		ct = p->getClass();
		ct->calculateDepth();
	}	
}

void ClassTable::calculateLength() {
	ClassTableRecord *p;
	ClassType *ct;
	int depth, done;

	// a class's depth is the number of parent classes it has.

	done = 0;
	depth = 0;
	while (done < size) {
		for (p = head; p != 0; p = p->getNext()) {
			ct = p->getClass();

			if (ct->getDepth() != depth)
				continue;

			ct->calculateOffsets();

			done++;

			if (done == size)
				return;
		}

		depth++;
	}
}

void ClassTable::calculateMethodOffsets() {
	ClassTableRecord *p;
	ClassType *ct;
	int done, depth;

	done = 0;
	depth = 0;
	while (done < size) {
		for (p = head; p != 0; p = p->getNext()) {
			ct = p->getClass();

			if (ct->getDepth() != depth)
				continue;

			ct->calculateMethodOffsets();

			done++;

			if (done == size)
				return;
		}

		depth++;
	}
}

void ClassTable::analyze(Seq *ast) {
	// add classes
	ast->addClasses();
	
	// analyze all class declarations
	analyzeClassDecs();

	// calculate depth for all classes
	calculateDepth();

	// breadth-first traversal of class tree.  add fields, analyze
	// them, and calculate their offsets.  add method, constructor
	// and destructor declarations and analyze them.  calculate
	// method offsets.

	analyzeClassBodies();
	calculateLength();
	calculateMethodOffsets();

	// analyze bodies of all methods, constructors and
	// destructors.
	analyzeBodies();
}

void ClassTable::analyzeClassDecs() {
	ClassTableRecord *p;

	for (p = head; p != 0; p = p->getNext())
		p->getClass()->getDeclaration()->analyze(0);
}

void ClassTable::analyzeClassBodies() {
	ClassConstructorNode *ccn;
	ClassMethodNode *cmn;
	ClassFieldDecNode *cfdn;
	ClassTableRecord *p;
	ClassType *ct, *extendType;
	Seq *body, *q, *noArgs;
	int done, depth;

	done = 0;
	depth = 0;
	while (done < size) {
		for (p = head; p != 0; p = p->getNext()) {
			ct = p->getClass();

			if (ct->getDepth() != depth)
				continue;

			body = ct->getDeclaration()->getBody();

			analyzingClassDec = true;
			
			for (q = body; q != 0 && q->getRight() != 0; q = q->getLeft()) {
				if ((cfdn = dynamic_cast<ClassFieldDecNode *>(q->getRight())) != 0) {
					cfdn->analyze((void *)ct);
				}
				if ((ccn = dynamic_cast<ClassConstructorNode *>(q->getRight())) != 0) {
					constructorAnalyzing = ccn;
					ccn->analyze((void *)ct);
					constructorAnalyzing = 0;
				}
				if ((cmn = dynamic_cast<ClassMethodNode *>(q->getRight())) != 0) {
					methodAnalyzing = cmn;
					cmn->analyze((void *)ct);
					methodAnalyzing = 0;				
				}				
			}
			
			analyzingClassDec = false;

			done++;

			if (done == size)
				break;
		}

		depth++;
	}

	// now we know what constructors have been declared.  Ensure
	// that if a class declares no constructors, its parent must
	// have a no argument constructor to invoke.
	noArgs = new Seq(0, 0);

	for (p = head; p != 0; p = p->getNext()) {
		ct = p->getClass();
		extendType = (ClassType *)ct->getExtendType();

		if (ct->getUseDefaultConstructor() == true && extendType != 0 &&
		    extendType->findConstructor(noArgs) == 0) {
			cerr << ct->getDeclaration()->getLineNumber() << ": "
			     << " no matching constructor "
			     << *(extendType->getName()) << "() in class "
			     << *(extendType->getName()) << '\n';
			err = true;
		}
	}

	// DELETE
	delete noArgs;
}

void ClassTable::analyzeBodies() {
	ClassTableRecord *p;
	ClassConstructorNode *ccn;
	ClassMethodNode *cmn;
	ClassType *ct;
	Seq *body, *q;

	analyzingClassDec = true;

	for (p = head; p != 0; p = p->getNext()) {
		ct = p->getClass();
		body = ct->getDeclaration()->getBody();

		for (q = body; q != 0 && q->getRight() != 0; q = q->getLeft()) {
			if ((ccn = dynamic_cast<ClassConstructorNode *>(q->getRight())) != 0) {
				localVariableStack->enterBody();
				localVariableStack->setOffset(1+ccn->getParameters()->size());
				
				constructorAnalyzing = ccn;
				analyzingClassConst = true;
				ccn->getBody()->analyze((void *)ct);
				analyzingClassConst = false;
				constructorAnalyzing = 0;
				
				ccn->setLocals(localVariableStack->leaveBody());
				ccn->setLocals(1+ccn->getParameters()->size()+ccn->getLocals());
			}
			if ((cmn = dynamic_cast<ClassMethodNode *>(q->getRight())) != 0) {
				localVariableStack->enterBody();
				localVariableStack->setOffset(1+cmn->getParameters()->size());
				
				methodAnalyzing = cmn;
				analyzingClassMeth = true;
				cmn->getBody()->analyze((void *)ct);
				analyzingClassMeth = false;
				methodAnalyzing = 0;
				
				cmn->setLocals(localVariableStack->leaveBody());
				cmn->setLocals(1+cmn->getParameters()->size()+cmn->getLocals());
			}
		}
	}

	analyzingClassDec = true;
}

ClassType * ClassTable::getObject() {
	return objectClass;
}
