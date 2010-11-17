// Niels Widger
// Time-stamp: <30 Nov 2009 at 19:23:08 by nwidger on macros.local>

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

Seq::Seq() {
	left = 0;
	right = 0;
}

Seq::Seq(Seq *l, Node *r) {
	left = l;
	right = r;
}

Seq::~Seq() {
	if (left != 0)
		delete left;
	if (right != 0)
		delete right;
}

void Seq::dump() {
	if (left != 0)
		left->dump();
	if (right != 0)
		right->dump();
}

void Seq::addClasses() {
	ClassDecNode *classDec;
	ClassType *classType;

	if (left != 0)
		left->addClasses();

	if ((classDec = dynamic_cast<ClassDecNode *>(right)) != 0) {
		classType = new ClassType(classDec->getName());
		classType->setDeclaration(classDec);
		if (types->classTable()->add(classType) == 0) {
			cerr << classDec->getLineNumber() << ": class "
			     << *(classDec->getName()) << " has already "
			     << "been declared!\n";
			err = true;
		}
	}
}

Seq * Seq::add(Node *n) {
	if (right == 0)
		right = n;
	else if (left == 0)
		left = new Seq(0, n);
	else
		left->add(n);

	return this;
}

Seq * Seq::add(Seq *s) {
	if (left == 0)
		left = s;
	else
		left->add(s);

	return this;
}

Seq * Seq::getLeft() {
	return left;
}

Node * Seq::getRight() {
	return right;
}

void Seq::clear() {
	if (left != 0)
		left->clear();
	left = 0;
	right = 0;
	delete this;
}

void Seq::toString() {
	if (left != 0)
		left->toString();
	if (right != 0)
		cerr << *(right->toString()) << '\n';
}

bool Seq::isEmpty() {
	return left == 0 && right == 0;
}

int Seq::size() {
	Seq *v;
	int size;

	v = this;
	for (size = 0; v != 0 && v->right != 0; size++, v = v->left);

	return size;
}

bool Seq::equalParameters(Seq *q) {
	VarDecNode *pvdn, *qvdn;

	pvdn = (VarDecNode *)right;
	qvdn = (VarDecNode *)q->right;

	if (size() == 0 && q->size() == 0)
		return true;
	if (size() != q->size())
		return false;
	if (!pvdn->equalType(qvdn))
		return false;

	if (left != 0)
		return left->equalParameters(q->left);
	else
		return true;
}

VarNode * Seq::findParameter(string *n) {
	VarDecNode *vdn;
	VarNode *vn;

	if (right != 0) {
		vdn = (VarDecNode *)right;
		vn = (VarNode *)(vdn->getVariables()->getRight());
		if (vn != 0 && *n == *(vn->getName()))
			return vn;

	}

	if (left != 0)
		return left->findParameter(n);
	
	return 0;
}

bool Seq::uniqueParameters() {
	Seq *v, *w;
	VarDecNode *vvdn, *wvdn;
	VarNode *vvn, *wvn;

	for (v = this; v != 0 && v->getRight() != 0; v = v->getLeft()) {
		vvdn = (VarDecNode *)(v->getRight());
		vvn = (VarNode *)(vvdn->getVariables()->getRight());
		for (w = v->getLeft(); 
		     w != 0 && w->getRight() != 0; w = w->getLeft()) {
			wvdn = (VarDecNode *)(w->getRight());
			wvn = (VarNode *)(wvdn->getVariables()->getRight());
			if (wvn->equal(vvn))
				return false;
		}
	}
	
	return true;
}

Seq * Seq::analyze(void *param) {
	ClassDecNode *cdn;
	
	if (left != 0)
		left = left->analyze(param);
	if (right != 0) {
		if ((cdn = dynamic_cast<ClassDecNode *>(right)) == 0)
			right = right->analyze(param);
	}
	
	return (Seq *)this;
}

bool Seq::canAssignTo(Seq *p) {
	Type *lt, *rt;
	
	// this = seq of expressionnode's
	// p = seq of (vardecnode -> varnode)

	if (size() == 0 && p->size() == 0)
		return true;

	if (size() != p->size())
		return false;

	lt = getRightType();
	rt = p->getRightType();

	if (lt == 0 || rt == 0)
		return false;

	if (!lt->canAssignTo(rt))
		return false;

	if (left != 0)
		return left->canAssignTo(p->getLeft());

	return true;
}

Seq * Seq::getMostSpecific() {
	Seq *p, *q, *mostSpecific;
	Seq *pArgs, *qArgs;
	bool changed;

	setMarked(true);

	do {
		changed = false;
		
		for (p = this; p != 0 && p->getRight() != 0; p = p->getLeft()) {
			if (p->getRight()->getMarked() == false)
				continue;

			pArgs = p->getParameters();

			for (q = this; q != 0 && q->getRight() != 0; q = q->getLeft()) {
				if (q->getRight()->getMarked() == false)
					continue;
				
				qArgs = q->getParameters();
				
				if (pArgs->canAssignTo(qArgs) &&
				    !qArgs->canAssignTo(pArgs)) {
					q->getRight()->setMarked(false);
					changed = true;
					continue;
				}
			}
		}
	} while (changed == true);

	mostSpecific = getMarked();

	setMarked(false);

	return mostSpecific;
}

Type * Seq::getRightType() {
	VarDecNode *vdn;
	ExpressionNode *en;

	if ((vdn = dynamic_cast<VarDecNode *>(right)) != 0)
		return ((VarNode *)(vdn->getVariables()->getRight()))->getType();
	if ((en = dynamic_cast<ExpressionNode *>(right)) != 0)
		return en->getType();

	return 0;
}

void Seq::setMarked(bool m) {
	if (left != 0)
		left->setMarked(m);

	if (right != 0)
		((ClassMethodNode *)right)->setMarked(m);
}

Seq * Seq::getMarked() {
	Seq *p, *q;
	ClassMethodNode *cmn;

	q = new Seq(0, 0);

	for (p = this; p != 0 && p->getRight() != 0; p = p->getLeft()) {
		cmn = (ClassMethodNode *)p->getRight();
		if (cmn->getMarked() == true)
			q->add(cmn);
	}

	return q;
}

Seq * Seq::getParameters() {
	ClassMethodNode *cmn;
	ClassConstructorNode *ccn;

	if ((cmn = dynamic_cast<ClassMethodNode *>(right)) != 0)
		return cmn->getParameters();
	if ((ccn = dynamic_cast<ClassConstructorNode *>(right)) != 0)
		return ccn->getParameters();

	return 0;
}

string * Seq::getMungedParameters(string *m) {
	string str;
	VarDecNode *vdn;
	VarNode *vn;
	
	if (left != 0)
		m = left->getMungedParameters(m);
	if (right != 0) {
		vdn = (VarDecNode *)right;
		vn = (VarNode *)(vdn->getVariables()->getRight());
		str = string(*m + "$" + *(vn->getType()->toString()));
		m = stringPool->newString(str);
	}

	return m;
}

void Seq::calculatePositions(int p) {
	VarDecNode *vdn;
	VarNode *vn;
	
	if (left != 0)
		left->calculatePositions(p-1);

	if (right != 0) {
		vdn = (VarDecNode *)right;
		vn = (VarNode *)(vdn->getVariables()->getRight());
		vn->setPosition(p);
	}
}

bool Seq::containsEqualMethod(ClassMethodNode *cmn) {
	bool retval;

	retval = false;
	
	if (left != 0)
		retval |= left->containsEqualMethod(cmn);
	if (right != 0)
		retval |= ((ClassMethodNode *)right)->equal(cmn);

	return retval;
}

string * Seq::toStringParams() {
	string str = string();
	
	toStringAuxParams(&str);

	return stringPool->newString(str);
}

void Seq::toStringAuxParams(string *str) {
	ExpressionNode *e;

	if (left != 0)
		left->toStringAuxParams(str);
	if (right != 0) {
		e = (ExpressionNode *)right;
		*str += *(e->getType()->toString()) + " ";
	}
}
