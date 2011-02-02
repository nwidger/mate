// Niels Widger
// Time-stamp: <27 Jan 2011 at 20:32:14 by nwidger on macros.local>

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <iostream>
#include <sstream>
using namespace std;

#include "ClassTable.h"
#include "globals.h"
#include "LabelStack.h"
#include "LocalVariableStack.h"
#include "MonitorStack.h"
#include "Node.h"
#include "Seq.h"
#include "StringPool.h"
#include "Type.h"
#include "TypeModule.h"

// -----------------------------------------------------------------------------

Node::Node() {
	line = 0;
	marked = false;
}

Node::Node(const char *n) {
	nodeName = string(n);
	line = getCurrentSourceLineNumber();
	marked = false;
}

Node::~Node() {
}

void Node::dump() {
}

string * Node::toString() {
	return &nodeName;
}

Node * Node::analyze(void *param) {
	return (Node *)this;
}

void Node::setLineNumber(int l) {
	line = l;
}

int Node::getLineNumber() {
	return line;
}

void Node::setMarked(bool m) {
	marked = m;
}

bool Node::getMarked() {
	return marked;
}

void Node::setLocals(int l) {
	locals = l;
}

int Node::getLocals() {
	return locals;
}

// -----------------------------------------------------------------------------

MainBlockNode::MainBlockNode(BlockStatementNode *b) : Node("MainBlockNode") {
	body = b;
}

MainBlockNode::~MainBlockNode() {
	if (body != 0)
		delete body;
}

void MainBlockNode::dump() {
	cerr << *(this->toString()) << '\n';
	body->dump();
}

Node * MainBlockNode::analyze(void *param) {
	localVariableStack->enterBody();
	localVariableStack->setOffset(0);

	mainBlockAnalyzing = this;
	analyzingMainBlock = true;
	body = (BlockStatementNode *)body->analyze(param);
	analyzingMainBlock = false;
	mainBlockAnalyzing = 0;

	setLocals(localVariableStack->leaveBody());

	return (Node *)this;
}

// -----------------------------------------------------------------------------
// VarDecNode
// -----------------------------------------------------------------------------

VarDecNode::VarDecNode(Type *t, Seq *v) : Node("VarDecNode") {
	type = t;
	variables = v;
	doNotDelete = false;
}

VarDecNode::~VarDecNode() {
	if (doNotDelete)
		variables->clear();
	else
		delete variables;
}

void VarDecNode::dump() {
	cerr << *(Node::toString()) << '\n';
	type->dump();
	variables->dump();
}

Type * VarDecNode::getType() {
	return type;
}

Seq * VarDecNode::getVariables() {
	return variables;
}

bool VarDecNode::equalType(VarDecNode *vdn) {
	return type->equal(vdn->type);
}

VarNode * VarDecNode::findVariable(string *n) {
	Seq *v;
	VarNode vn = VarNode(n);
	VarNode *vn2;

	for (v = variables; v != 0 && v->getRight() != 0; v = v->getLeft()) {
		vn2 = ((VarNode *)(v->getRight()));
		if (vn2->equal(&vn))
			return vn2;
	}

	return 0;
}

string * VarDecNode::toString() {
	string str = string();

	toStringAux(variables, &str);

	return stringPool->newString(str);
}

void VarDecNode::toStringAux(Seq *v, string *str) {
	VarNode *vn;

	if (v->getLeft() != 0)
		toStringAux(v->getLeft(), str);

	if (v->getRight() != 0) {
		vn = (VarNode *)v->getRight();
		*str += *(vn->toString()) + " ";
	}
}

Node * VarDecNode::analyze(void *param) {
	type = type->analyze(param);
	variables = variables->analyze(type);

	return (Node *)this;
}

LocalVarDecNode::LocalVarDecNode(Type *t, Seq *v) : VarDecNode(t, v) {
	doNotDelete = true;
	nodeName = "LocalVarDecNode";
}

Node * LocalVarDecNode::analyze(void *param) {
	analyzingLocalVarDec = true;
	VarDecNode::analyze(param);
	analyzingLocalVarDec = false;

	return (Node *)this;
}

// -----------------------------------------------------------------------------
// VarNode
// -----------------------------------------------------------------------------

VarNode::VarNode(string *n) {
	name = n;
	type = 0;
	position = 0;
	isLocal = false;
	nodeName = "VarNode";
}

void VarNode::setPosition(int p) {
	position = p;
}

int VarNode::getPosition() {
	return position;
}

string * VarNode::getName() {
	return name;
}

void VarNode::dump() {
	if (type != 0)
		cerr << *(Node::toString()) << " " << *(type->toString()) << " "
		     << *name << " [" << position << "] [" << parentIsDeref << "]\n";
	else
		cerr << *(Node::toString()) << " " << *name << " [" << parentIsDeref << "] \n";

}

string * VarNode::toString() {
	string str;

	if (type != 0) {
		str = string(*(type->toString()) + " " + *name);
	} else {
		str = string(*name);
	}

	return stringPool->newString(str);
}

Node * VarNode::analyze(void *param) {
	Node *node;

	if (analyzingLocalVarDec == true ||
	    (analyzingMainBlock == false && analyzingClassMeth == false &&
	     analyzingClassConst == false)) {
		if (param == 0)
			return (Node *)this;

		type = (Type *)param;
		type = type->analyze((void *)line);

		return (Node *)this;
	}

	if (analyzingClassMeth == true) {
		node = new ClassVarNode(name);
		node->setLineNumber(line);
		node = node->analyze(methodAnalyzing);
		// DELETE
		delete this;
		return node;
	}

	if (analyzingClassConst == true) {
		node = new ClassVarNode(name);
		node->setLineNumber(line);
		node = node->analyze(constructorAnalyzing);
		// DELETE
		delete this;
		return node;
	}

	if (analyzingMainBlock == true) {
		node = new MainVarNode(name);
		node->setLineNumber(line);
		node = node->analyze(param);
		// DELETE
		delete this;
		return node;
	}

	return (Node *)this;
}

bool VarNode::equal(VarNode *vn) {
	return *name == *(vn->name);
}

MainVarNode::MainVarNode(string *n) : VarNode(n) {
	type = 0;
	nodeName = "MainVarNode";
}

Node * MainVarNode::analyze(void *param) {
	VarNode *vn;

	if ((vn = localVariableStack->findVariable(name)) == 0) {
		cerr << line << ": variable " << *name
		     << " has not been declared in main block!\n";
		err = true;
		
		// DELETE
		delete this;
		return (Node *)new IntegerLiteralNode(0);
	}

	// DELETE
	delete this;
	return (Node *)new VarNode(*vn);
}

// -----------------------------------------------------------------------------

ClassDecNode::ClassDecNode(string *n, Seq *b) : Node("ClassDecNode" ) {
	name = n;
	extends = &objectName;
	body = b;
}

ClassDecNode::ClassDecNode(string *n, string *e, Seq *b) : Node("ClassDecNode") {
	name = n;
	extends = e;
	body = b;
}

ClassDecNode::~ClassDecNode() {
}

void ClassDecNode::dump() {
	cerr << *(Node::toString()) << " " << *name << " extends " << *extends << '\n';
	body->dump();
}

string * ClassDecNode::getName() {
	return name;
}

Seq * ClassDecNode::getBody() {
	return body;
}

Node * ClassDecNode::analyze(void *param) {
	ClassType *ct, *et;

	analyzingClassDec = true;

	if (*name == objectName)
		return (Node *)this;

	if ((ct = types->classTable()->find(name)) == 0) {
		cerr << line << ": class " << *name << " has not been declared!\n";
		err = true;
		ct = types->classTable()->getObject();
	}

	if (*name == *extends) {
		cerr << line << ": superclass name is same as subclass name in "
		     << *name << " class declaration!\n";
		err = true;
		extends = &objectName;
		ct->addExtendType(types->classTable()->find(extends));
	}

	if ((et = types->classTable()->find(extends)) != 0) {
		ct->addExtendType(et);
	} else {
		cerr << line << ": superclass " << *extends << " of subclass "
		     << *name << " has not been declared!\n";
		err = true;
		extends = &objectName;
		et = types->classTable()->find(extends);
		ct->addExtendType(et);
	}

	if (ct->isCircular(et)) {
		cerr << line << ": circular dependency between class " << *extends
		     << " and class " + *name << '\n';
		err = true;
		extends = &objectName;
		ct->addExtendType(types->classTable()->find(extends));
	}

	analyzingClassDec = false;

	return (Node *)this;
}

ClassConstructorNode::ClassConstructorNode(string *n, Seq *p) : Node("ClassConstructorNode") {
	string str;

	name = n;
	parameters = p;
	body = 0;
	classType = 0;

	str = string(*name + "_constructor");
	mungedName = stringPool->newString(str);

	isNative = false;
}

ClassConstructorNode::~ClassConstructorNode() {
	if (parameters != 0)
		delete parameters;
	if (body != 0)
		delete body;
}

void ClassConstructorNode::setBody(BlockStatementNode *b) {
	if (body != 0)
		delete body;
	body = b;
}

BlockStatementNode * ClassConstructorNode::getBody() {
	return body;
}

void ClassConstructorNode::dump() {
	cerr << *(Node::toString()) << " " << *name << '\n';
	parameters->dump();
	body->dump();
}

string * ClassConstructorNode::toString() {
	string str = string(*name);

	str += "(";

	toStringAux(parameters, &str);

	str += ") { }";

	return stringPool->newString(str);
}

void ClassConstructorNode::toStringAux(Seq *p, string *str) {
	VarDecNode *vdn;

	if (p->getLeft() != 0)
		toStringAux(p->getLeft(), str);

	if (p->getRight() != 0) {
		vdn = (VarDecNode *)p->getRight();
		*str += *(vdn->toString()) + " ";
	}
}

ClassType * ClassConstructorNode::getClassType() {
	return classType;
}

Node * ClassConstructorNode::analyze(void *param) {
	ClassType *ct = (ClassType *)param;

	constructorAnalyzing = this;

	classType = ct;

	if (*name != *(ct->getName())) {
		cerr << line << ": declaration of constructor for class "
		     << *name << " in declaration of class "
		     << *(ct->getName()) << '\n';
		err = true;
		name = ct->getName();
	}

	parameters = parameters->analyze(param);
	parameters->calculatePositions(parameters->size());

	generateMungedName();

	if (!parameters->uniqueParameters()) {
		err = true;
		cerr << line << ": declaration of constructor "
		     << *name << " in class " << *(ct->getName())
		     << " contains non-unique formal parameter names\n";
		analyzingClassConst = false;
		return (Node *)this;
	}

	ct->addConstructor(this);

	constructorAnalyzing = 0;

	return (Node *)this;
}

bool ClassConstructorNode::equal(ClassConstructorNode *ccn) {
	return parameters->equalParameters(ccn->parameters);
}

Seq * ClassConstructorNode::getParameters() {
	return parameters;
}

VarNode * ClassConstructorNode::findParameter(string *n) {
	return parameters->findParameter(n);
}

void ClassConstructorNode::generateMungedName() {
	ostringstream os;
	string str = string(*name + "_constructor");

	mungedName = parameters->getMungedParameters(stringPool->newString(str));
}

string * ClassConstructorNode::getMungedName() {
	return mungedName;
}

string * ClassConstructorNode::getName() {
	return name;
}

void ClassConstructorNode::setNativeIndex(int i) {
	isNative = true;
	nativeIndex = i;
}

bool ClassConstructorNode::getIsNative() {
	return isNative;
}

int ClassConstructorNode::getNativeIndex() {
	return nativeIndex;
}

ClassMethodNode::ClassMethodNode(string *n, Seq *p) : Node("ClassMethodNode") {
	type = 0;
	name = n;
	mungedName = 0;
	parameters = p;
	offset = 0;
	body = 0;
	isNative = false;
}

ClassMethodNode::~ClassMethodNode() {
	if (parameters != 0)
		delete parameters;
	if (body != 0)
		delete body;
}

void ClassMethodNode::setType(Type *t) {
	type = t;
}

Type * ClassMethodNode::getType() {
	return type;
}

void ClassMethodNode::setBody(BlockStatementNode *b) {
	if (body != 0)
		delete body;
	body = b;
}

string * ClassMethodNode::getName() {
	return name;
}

BlockStatementNode * ClassMethodNode::getBody() {
	return body;
}

ClassType * ClassMethodNode::getClassType() {
	return classType;
}

void ClassMethodNode::setClassType(ClassType *c) {
	classType = c;
}

void ClassMethodNode::dump() {
	cerr << *(Node::toString()) << " " << *name << '\n';
	type->dump();
	parameters->dump();
	body->dump();
}

VarNode * ClassMethodNode::findParameter(string *n) {
	return parameters->findParameter(n);
}

Seq * ClassMethodNode::getParameters() {
	return parameters;
}

void ClassMethodNode::setOffset(int o) {
	offset = o;
}

int ClassMethodNode::getOffset() {
	return offset;
}

string * ClassMethodNode::toString() {
	string str = string(*(type->toString()));

	str += " " + *name + "(";

	toStringAux(parameters, &str);

	str += ") { }";

	return stringPool->newString(str);
}

void ClassMethodNode::toStringAux(Seq *p, string *str) {
	VarDecNode *vdn;

	if (p->getLeft() != 0)
		toStringAux(p->getLeft(), str);

	if (p->getRight() != 0) {
		vdn = (VarDecNode *)p->getRight();
		*str += *(vdn->toString()) + " ";
	}
}

Node * ClassMethodNode::analyze(void *param) {
	ClassMethodNode *cmn;
	ClassType *extendType, *ct;

	ct = (ClassType *)param;
	methodAnalyzing = this;

	classType = ct;
	type = type->analyze((void *)line);

	parameters = parameters->analyze(param);
	parameters->calculatePositions(parameters->size());

	generateMungedName();

	if (!parameters->uniqueParameters()) {
		cerr << line << ": declaration of method "
		     << *name << " in class " << *(ct->getName())
		     << " contains non-unique formal parameter names!\n";
		err = true;
		mungedName = stringPool->newString("Object$Object");
		return (Node *)this;
	}

	extendType = (ClassType *)ct->getExtendType();

	if (extendType != 0 && (cmn = extendType->findMethod(this, true)) != 0 &&
	    type->equal(cmn->getType()) == false) {
		cerr << line << ": declaration of method "
		     << *name << " in class " << *(ct->getName())
		     << " has return type " << *(type->getName())
		     << " but overrides inherited method "
		     << *(cmn->getName()) << " with return type "
		     << *(cmn->getType()->getName()) << "!\n";
		err = true;
	}

	ct->addMethod(this);

	methodAnalyzing = 0;

	return (Node *)this;
}

bool ClassMethodNode::equal(ClassMethodNode *cmn) {
	if (*(cmn->name) != *name)
		return false;

	if (!parameters->equalParameters(cmn->parameters))
		return false;

	return true;
}

void ClassMethodNode::generateMungedName() {
	ostringstream os;
	string str = string(*(classType->getName()) + "$" + *name);
	mungedName = parameters->getMungedParameters(stringPool->newString(str));
}

string * ClassMethodNode::getMungedName() {
	return mungedName;
}

void ClassMethodNode::setMungedName(string *m) {
	mungedName = m;
}

void ClassMethodNode::setNativeIndex(int i) {
	isNative = true;
	nativeIndex = i;
}

bool ClassMethodNode::getIsNative() {
	return isNative;
}

int ClassMethodNode::getNativeIndex() {
	return nativeIndex;
}

ClassFieldDecNode::ClassFieldDecNode(Type *t, Seq *v) : VarDecNode(t, v) {
	nodeName = "ClassFieldDecNode";
}

Node * ClassFieldDecNode::analyze(void *param) {
	ClassType *ct = (ClassType *)param;

	type = type->analyze((void *)line);
	variables = variables->analyze(type);

	ct->addFields(variables);

	return (Node *)this;
}

ClassFieldNode::ClassFieldNode(string *n) : VarNode(n) {
	offset = 0;
	nodeName = "ClassFieldNode";
}

ClassFieldNode::ClassFieldNode(VarNode *vn) : VarNode(vn->getName()) {
	type = vn->getType();
	line = vn->getLineNumber();
	offset = 0;
	nodeName = "ClassFieldNode";
}

string * ClassFieldNode::toString() {
	string str;

	if (type != 0)
		str = string(*(type->toString()) + " " + *name);
	else
		str = string(*name);

	return stringPool->newString(str);
}

void ClassFieldNode::setOffset(int o) {
	offset = o;
}

int ClassFieldNode::getOffset() {
	return offset;
}

Node * ClassFieldNode::analyze(void *param, void *param2) {
	Type *t = (Type *)param;

	type = t;

	return (Node *)this;
}

ClassVarNode::ClassVarNode(string *n) : VarNode(n) {
	type = 0;
	nodeName = "ClassVarNode";
}

Node * ClassVarNode::analyze(void *param) {
	ClassMethodNode *cmn;
	ClassConstructorNode *ccn;
	ClassFieldNode *cfn;
	VarNode *vn;

	if ((cmn = dynamic_cast<ClassMethodNode *>((Node *)param)) != 0) {
		classType = cmn->getClassType();

		if ((vn = localVariableStack->findVariable(name)) != 0) {
			// DELETE
			delete this;
			return (Node *)new VarNode(*vn);
		} else if ((vn = cmn->findParameter(name)) != 0) {
			// DELETE
			delete this;
			return (Node *)new VarNode(*vn);
		} else if ((cfn = classType->findField(name, true)) != 0) {
			// DELETE
			delete this;
			return (Node *)new ClassFieldNode(*cfn);
		} else {
			cerr << line << ": variable/parameter/field "
			     << *name << " has not been declared in method "
			     << *(cmn->getName()) << " of class "
			     << *(classType->getName()) << "!\n";
			err = true;
		}
	} else if ((ccn = dynamic_cast<ClassConstructorNode *>((Node *)param)) != 0) {
		classType = ccn->getClassType();

		if ((vn = localVariableStack->findVariable(name)) != 0) {
			// DELETE
			delete this;
			return (Node *)new VarNode(*vn);
		} else if ((vn = ccn->findParameter(name)) != 0) {
			// DELETE
			delete this;
			return (Node *)new VarNode(*vn);
		} else if ((cfn = classType->findField(name, true)) != 0) {
			// DELETE
			delete this;
			return (Node *)new ClassFieldNode(*cfn);
		} else {
			cerr << line << ": variable/parameter/field "
			     << *name << " has not been declared in constructor "
			     << *(ccn->getName()) << " of class "
			     << *(classType->getName()) << "!\n";
			err = true;
		}
	}

	// DELETE
	delete this;
	return (Node *)new IntegerLiteralNode(0);
}

// -----------------------------------------------------------------------------
// ExpressionNode
// -----------------------------------------------------------------------------

ExpressionNode::ExpressionNode() : Node("ExpressionNode") {
	parentIsNegate = false;
	parentIsDeref = false;
	isParenthesized = false;
	isStatement = false;
	type = 0;
}

void ExpressionNode::setParentIsNegate(bool p) {
	parentIsNegate = p;
}

void ExpressionNode::setParentIsDeref(bool p) {
	parentIsDeref = p;
}

void ExpressionNode::setIsParenthesized(bool i) {
	isParenthesized = i;
}

void ExpressionNode::setType(Type *t) {
	type = t;
}

Type * ExpressionNode::getType() {
	return type;
}

void ExpressionNode::dump() {
	if (type != 0)
		cerr << *(Node::toString()) << " [" << *(type->toString()) << "]\n";
	else
		cerr << *(Node::toString()) << '\n';
}

bool ExpressionNode::equalType(ExpressionNode *e) {
	return type->equal(e->type);
}


void ExpressionNode::setIsStatement(bool i) {
	isStatement = i;
}

bool ExpressionNode::getIsStatement() {
	return isStatement;
}

// -----------------------------------------------------------------------------
// InExpressionNode
// -----------------------------------------------------------------------------


InExpressionNode::InExpressionNode() {
	nodeName = "InExpressionNode";
}

Node * InExpressionNode::analyze(void *param) {
	type = types->classTable()->find("String");

	return (Node *)this;
}

// -----------------------------------------------------------------------------
// InstanceOfExpressionNode
// -----------------------------------------------------------------------------

InstanceOfExpressionNode::InstanceOfExpressionNode(ExpressionNode *e, Type *t) {
	expression = e;
	toType = (ClassType *)t;
	toVmt = 0;
	nodeName = "InstanceOfExpressionNode";
}

InstanceOfExpressionNode::~InstanceOfExpressionNode() {
	if (expression != 0)
		delete expression;
}

void InstanceOfExpressionNode::dump() {
	ExpressionNode::dump();

	if (expression != 0)
		expression->dump();
	if (toType != 0)
		toType->dump();
}

Node * InstanceOfExpressionNode::analyze(void *param) {
	Type *expressionType;
	string str;

	expression = (ExpressionNode *)expression->analyze(param);
	expressionType = expression->getType();

	toType = (ClassType *)toType->analyze((void *)line);
	toVmt = toType->getName();

	if (expressionType->canCastTo(toType) == false) {
		cerr << line << ": expression type " << *(expressionType->toString())
		     << " is not convertable by casting conversion to type "
		     << *(toType->toString()) << " in instanceof operator!\n";
		err = true;
		toType = types->classTable()->getObject();
		str = string(*(toType->getName()));
		toVmt = stringPool->newString(str);
	}

	type = types->classTable()->find("Integer");
	return (Node *)this;
}

// -----------------------------------------------------------------------------
// CastExpressionNode
// -----------------------------------------------------------------------------

CastExpressionNode::CastExpressionNode(ExpressionNode *c, ExpressionNode *e) {
	castType = 0;
	toVmt = 0;
	rank = 0;
	expressionType = 0;
	className = c;
	expression = e;
	type = 0;
	nodeName = "CastExpressionNode";
}

CastExpressionNode::~CastExpressionNode() {
	if (className != 0)
		delete className;
	if (expression != 0)
		delete expression;
}

void CastExpressionNode::dump() {
	ExpressionNode::dump();

	if (castType != 0)
		castType->dump();
	else if (className != 0)
		className->dump();
	if (expression != 0)
		expression->dump();
}

Node * CastExpressionNode::analyze(void *param) {
	DerefNode *dn;
	VarNode *vn;
	string str;

	expression = (ExpressionNode *)expression->analyze(param);
	expressionType = expression->getType();
	expressionType = expressionType->analyze((void *)line);

	if ((dn = dynamic_cast<DerefNode *>(className)) != 0 &&
		   (vn = dynamic_cast<VarNode *>(dn->getExpression())) != 0) {
		className = vn;
		if((castType = types->classTable()->find(vn->getName())) == 0) {
			cerr << line << ": class " << *(vn->getName()) << " of cast operator "
			     << "has not been declared!\n";
			err = true;
			castType = types->classTable()->getObject();
		}
		str = string(*(castType->getName()));
		toVmt = stringPool->newString(str);
	} else {
		cerr << line << ": type of cast operator must be a reference type!\n";
		err = true;
		castType = types->classTable()->getObject();
		str = string(*(castType->getName()));
		toVmt = stringPool->newString(str);
		type = castType;
		return (Node *)this;
	}

	if (expressionType->canCastTo(castType) == false) {
		cerr << line << ": expression type " << *(expressionType->toString())
		     << " is not convertable by casting conversion to type "
		     << *(castType->toString()) << " in cast operator!\n";
		err = true;
		castType = types->classTable()->getObject();
		str = string(*(castType->getName()));
		toVmt = stringPool->newString(str);
	}

	type = castType;

	return (Node *)this;
}

// -----------------------------------------------------------------------------
// ClassInstanceCreationExpressionNode
// -----------------------------------------------------------------------------

ClassInstanceCreationExpressionNode::ClassInstanceCreationExpressionNode(string *n, Seq *a) {
	name = n;
	arguments = a;
	type = 0;
	goodLabel = 0;
	mungedName = 0;
	callNative = false;
	callDefault = false;
	nativeIndex = 0;
	nodeName = "ClassInstanceCreationExpressionNode";
}

ClassInstanceCreationExpressionNode::~ClassInstanceCreationExpressionNode() {
	if (arguments != 0)
		delete arguments;
}

void ClassInstanceCreationExpressionNode::dump() {
	ExpressionNode::dump();

	if (classType != 0)
		classType->dump();
	else
		cerr << "class " << *name << '\n';
	if (arguments != 0)
		arguments->dump();
}

Node * ClassInstanceCreationExpressionNode::analyze(void *param) {
	labelCounter++;
	goodLabel = labelCounter-1;

	arguments = arguments->analyze(param);

	if ((classType = types->classTable()->find(name)) == 0) {
		cerr << line << ": class " << *name << " in class instance "
		     << "creation expression has not been declared!\n";
		err = true;
		
		classType = types->classTable()->getObject();
		type = classType;

		delete arguments;
		arguments = new Seq(0, 0);

		mungedName = stringPool->newString("Object_constructor");
		callNative = true;
		nativeIndex = 0;
		return (Node *)this;
	}

	type = classType;

	if (arguments->size() == 0 && classType->getUseDefaultConstructor() == true) {
		callDefault = true;
		return (Node *)this;
	}

	if ((constructor = classType->findConstructor(arguments)) == 0) {
		cerr << line << ": no matching constructor "
		     << *name << "(" << *(arguments->toStringParams()) << ")"
		     << " in class " << *(classType->getName()) << "!\n";
		err = true;
		mungedName = stringPool->newString("Object_constructor");
		// DELETE
		delete this;
		return (Node *)new IntegerLiteralNode(0);
	}

	if (constructor->getIsNative() == true) {
		callNative = true;
		nativeIndex = constructor->getNativeIndex();
	}

	mungedName = constructor->getMungedName();

	return (Node *)this;
}

// -----------------------------------------------------------------------------
// MethodInvocationExpressionNode
// -----------------------------------------------------------------------------

MethodInvocationExpressionNode::MethodInvocationExpressionNode(ExpressionNode *e, string *m, Seq *a) {
	expression = e;
	methodName = m;
	arguments = a;
	method = 0;
	offset = 0;
	type = 0;
	callDirectly = false;
	mungedName = 0;
	callNative = false;
	expressionAnalyzed = false;
	nativeIndex = 0;
	nodeName = "MethodInvocationExpressionNode";
}

MethodInvocationExpressionNode::~MethodInvocationExpressionNode() {
	if (expression != 0)
		delete expression;
	if (arguments != 0)
		delete arguments;
}

void MethodInvocationExpressionNode::dump() {
	if (type != 0)
		cerr << "MethodInvocationExpressionNode [" << *methodName << "] ["
		     <<  *(type->toString()) << "]\n";
	else
		cerr << "MethodInvocationExpressionNode [" << *methodName << "]\n";

	if (expression != 0)
		expression->dump();
	if (arguments != 0)
		arguments->dump();
}

void MethodInvocationExpressionNode::setExpressionAnalyzed(bool e) {
	expressionAnalyzed = e;
}

bool MethodInvocationExpressionNode::getExpressionAnalyzed() {
	return expressionAnalyzed;
}

Node * MethodInvocationExpressionNode::analyze(void *param) {
	SuperExpressionNode *sen;
	string *className;
	ClassType *ct, *classType;
	Type *expressionType;

	arguments = arguments->analyze(param);

	if (expressionAnalyzed == false)
		expression = (ExpressionNode *)expression->analyze(param);
	expressionType = expression->getType();

	if ((ct = dynamic_cast<ClassType *>(expressionType)) != 0) {
		classType = ct;
		className = classType->getName();
	} else {
		cerr << line << ": expression type "
		     << *(expression->getType()->toString())
		     << " of method invocation must be of "
		     << "reference type!\n";
		err = true;
		// DELETE
		delete this;
		return (Node *)new IntegerLiteralNode(0);
	}

	if ((method = classType->findMethod(methodName, arguments)) == 0) {
		cerr << line << ": no matching method for call to "
		     << *methodName
		     << "(" << *(arguments->toStringParams()) << ")"
		     <<" in class " << *className << "!\n";
		err = true;
		// DELETE
		delete this;
		return (Node *)new IntegerLiteralNode(0);
	}

	if ((sen = dynamic_cast<SuperExpressionNode *>(expression)) != 0)
		callDirectly = true;

	type = method->getType();
	offset = method->getOffset();
	mungedName = method->getMungedName();

	return (Node *)this;
}

// -----------------------------------------------------------------------------
// ThisExpressionNode
// -----------------------------------------------------------------------------

ThisExpressionNode::ThisExpressionNode() {
	nodeName = "ThisExpressionNode";
}

string * ThisExpressionNode::toString() {
	return stringPool->newString("this");
}

void ThisExpressionNode::dump() {
	Node::dump();
}

Node * ThisExpressionNode::analyze(void *param) {
	if (analyzingClassMeth == true) {
		type = methodAnalyzing->getClassType();
	} else if (analyzingClassConst == true) {
		type = constructorAnalyzing->getClassType();
	} else {
		cerr << line << ": keyword " << *(toString()) << " can only be used in the body of "
		     << "a method or constructor!\n";
		err = true;
		type = types->classTable()->getObject();
	}

	return (Node *)this;
}

// -----------------------------------------------------------------------------
// SuperExpressionNode
// -----------------------------------------------------------------------------

SuperExpressionNode::SuperExpressionNode() {
	nodeName = "SuperExpressionNode";
}

string * SuperExpressionNode::toString() {
	return stringPool->newString("super");
}

Node * SuperExpressionNode::analyze(void *param) {
	ThisExpressionNode::analyze(param);

	if (type != 0 && *(type->getName()) != objectName)
		type = ((ClassType *)type)->getExtendType();

	return (Node *)this;
}

// -----------------------------------------------------------------------------
// IntegerLiteralNode
// -----------------------------------------------------------------------------

IntegerLiteralNode::IntegerLiteralNode(unsigned long v) {
	value = v;
	type = types->classTable()->find("Integer");
	nodeName = "IntegerLiteralNode";
}

void IntegerLiteralNode::dump() {
	if (type != 0) {
		cerr << *(Node::toString())
		     << " [" << value << "] [" << *(type->toString()) << "]\n";
	} else {
		cerr << *(Node::toString()) << " [" << value << "]\n";
	}
}

Node * IntegerLiteralNode::analyze(void *param) {
	if ((parentIsNegate == false || isParenthesized == true) &&
	    value == 2147483648UL) {
		cerr << line << ": integer literal value 2147483648 may only appear"
		     << " as the operand of the unary negation operator '-'\n";
		err = true;
		value = 0;
	}

	type = types->classTable()->find("Integer");

	return (Node *)this;
}

// -----------------------------------------------------------------------------
// RealLiteralNode
// -----------------------------------------------------------------------------

RealLiteralNode::RealLiteralNode(float v) {
	value = v;
	type = types->classTable()->find("Real");
	nodeName = "RealLiteralNode";
}

void RealLiteralNode::dump() {
	if (type != 0) {
		cerr << *(Node::toString())
		     << " [" << value << "] [" << *(type->toString()) << "]\n";
	} else {
		cerr << *(Node::toString()) << " [" << value << "]\n";
	}
}

Node * RealLiteralNode::analyze(void *param) {
	// if ((parentIsNegate == false || isParenthesized == true) &&
	//     value == 2147483648UL) {
	// 	cerr << line << ": integer literal value 2147483648 may only appear"
	// 	     << " as the operand of the unary negation operator '-'\n";
	// 	err = true;
	// 	value = 0;
	// }

	type = types->classTable()->find("Real");

	return (Node *)this;
}

// -----------------------------------------------------------------------------
// NullLiteralNode
// -----------------------------------------------------------------------------

NullLiteralNode::NullLiteralNode() {
	type = types->getNullType();
	nodeName = "NullLiteralNode";
}

void NullLiteralNode::dump() {
	cerr << *(Node::toString()) << " [null]\n";
}

// -----------------------------------------------------------------------------
// StringLiteralNode
// -----------------------------------------------------------------------------

StringLiteralNode::StringLiteralNode(string *s) {
	str = s;
	nodeName = "StringLiteralNode";
}

void StringLiteralNode::dump() {
	if (str != 0)
		cerr << *(Node::toString()) << " [" << *str << "] [String]\n";
	else
		cerr << *(Node::toString()) << " [String]\n";
}

Node * StringLiteralNode::analyze(void *param) {
	type = types->classTable()->find("String");
	return (Node *)this;
}

// -----------------------------------------------------------------------------
// UnaryOperatorNode
// -----------------------------------------------------------------------------

UnaryOperatorNode::UnaryOperatorNode(ExpressionNode *e) {
	expression = e;
	type = 0;
	nodeName = "UnaryOperatorNode";
}

UnaryOperatorNode::~UnaryOperatorNode() {
	if (expression != 0)
		delete expression;
}

void UnaryOperatorNode::dump() {
	ExpressionNode::dump();
	expression->dump();
}

Node * UnaryOperatorNode::analyze(void *param) {
	expression = (ExpressionNode *)expression->analyze(param);
	type = expression->getType();

	return (Node *)this;
}

// -----------------------------------------------------------------------------
// FieldAccessOperatorNode
// -----------------------------------------------------------------------------

FieldAccessOperatorNode::FieldAccessOperatorNode(ExpressionNode *e, string *i) : UnaryOperatorNode(e) {
	identifier = i;
	offset = 0;
	nodeName = "FieldAccessOperatorNode";
}

void FieldAccessOperatorNode::dump() {
	if (identifier != 0)
		cerr << "FieldAccessOperatorNode [" << *identifier << "]";
	else
		cerr << "FieldAccessOperatorNode";

	if (type != 0)
		cerr << " [" << *(type->toString()) << "]\n";
	else
		cerr << '\n';

	if (expression != 0)
		expression->dump();
}

Node * FieldAccessOperatorNode::analyze(void *param) {
	ReferenceType *rt;
	ClassType *ct;
	ClassFieldNode *cfn;

	expression = (ExpressionNode *)expression->analyze(param);

	if ((rt = dynamic_cast<ReferenceType *>(expression->getType())) == 0) {
		cerr << line << ": field access operand must be of reference type!\n";
		err = true;
		type = types->classTable()->getObject();
		return (Node *)this;
	}

	type = expression->getType();
	type = type->analyze((void *)line);

	if ((ct = dynamic_cast<ClassType *>(type)) != 0) {
		if ((cfn = ct->findField(identifier, true)) == 0) {
			cerr << line << ": " << *identifier << " is not a field of "
			     << "class " << *(ct->getName()) << "!\n";
			err = true;
			offset = 0;
			return (Node *)this;
		}

		offset = cfn->getOffset();
		type = cfn->getType();
	}
	
	return (Node *)this;
}

int FieldAccessOperatorNode::getOffset() {
	return offset;
}

// -----------------------------------------------------------------------------
// BinaryOperatorNode
// -----------------------------------------------------------------------------

BinaryOperatorNode::BinaryOperatorNode(ExpressionNode *l, ExpressionNode *r) {
	lhs = l;
	rhs = r;
	type = 0;
	nodeName = "BinaryOperatorNode";
}

BinaryOperatorNode::~BinaryOperatorNode() {
	if (lhs != 0)
		delete lhs;
	if (rhs != 0)
		delete rhs;
}

void BinaryOperatorNode::dump() {
	ExpressionNode::dump();
	lhs->dump();
	rhs->dump();
}

Node * BinaryOperatorNode::analyze(void *param) {
	lhs = (ExpressionNode *)lhs->analyze(param);
	rhs = (ExpressionNode *)rhs->analyze(param);

	type = lhs->getType();

	return (Node *)this;
}

// -----------------------------------------------------------------------------
// EqualityOperatorNode
// -----------------------------------------------------------------------------

EqualityOperatorNode::EqualityOperatorNode(ExpressionNode *l, ExpressionNode *r) : BinaryOperatorNode(l, r) {
	nodeName = "EqualityOperatorNode";
}

Node * EqualityOperatorNode::analyze(void *param) {
	lhs = (ExpressionNode *)lhs->analyze(param);
	rhs = (ExpressionNode *)rhs->analyze(param);

	type = types->classTable()->find("Integer");
	return (Node *)this;
}

// -----------------------------------------------------------------------------
// AssignmentOperatorNode
// -----------------------------------------------------------------------------

AssignmentOperatorNode::AssignmentOperatorNode(ExpressionNode *l, ExpressionNode *r) : BinaryOperatorNode(l, r) {
	storeInstruction = 0;
	usingPutfield = false;
	nodeName = "AssignmentOperatorNode";
}

Node * AssignmentOperatorNode::analyze(void *param) {
	VarNode *vn;
	ClassFieldNode *cfn;
	FieldAccessOperatorNode *faon;
	DerefNode *dn;
	string str;
	ostringstream storeO;

	lhs = (ExpressionNode *)lhs->analyze(param);
	rhs = (ExpressionNode *)rhs->analyze(param);

	if (!rhs->getType()->equal(lhs->getType()) &&
	    !rhs->getType()->canWidenTo(lhs->getType())) {
		cerr << line << ": right-hand operand type "
		     << *(rhs->getType()->toString()) << " is not assignment "
		     << "compatible with the left-hand operand type "
		     << *(lhs->getType()->toString()) << " in assignment operator!\n";
		err = true;
	}

	type = lhs->getType();
	type = type->analyze((void *)line);

	if ((dn = dynamic_cast<DerefNode *>(lhs)) != 0) {
		lhs = dn->getExpression();
		lhs->setParentIsDeref(false);
		// DELETE
		delete dn;
	}

	if ((cfn = dynamic_cast<ClassFieldNode *>(lhs)) != 0) {
		storeO << "putfield " << cfn->getOffset();
		str = storeO.str();
		storeInstruction = stringPool->newString(str);
		usingPutfield = true;
	} else if ((vn = dynamic_cast<VarNode *>(lhs)) != 0) {
		storeO << "astore " << vn->getPosition();
		str = storeO.str();
		storeInstruction = stringPool->newString(str);
		usingPutfield = false;
	} else if ((faon = dynamic_cast<FieldAccessOperatorNode *>(lhs)) != 0) {
		storeO << "putfield " << faon->getOffset();
		str = storeO.str();
		storeInstruction = stringPool->newString(str);
		usingPutfield = true;		
	}

	return (Node *)this;
}

// -----------------------------------------------------------------------------
// StatementNode
// -----------------------------------------------------------------------------

StatementNode::StatementNode() : Node("StatementNode") {
}

void StatementNode::dump() {
	cerr << *(Node::toString()) << '\n';
}

SynchronizedStatementNode::SynchronizedStatementNode(ExpressionNode *e, Seq *s) {
	expression = e;
	statements = s;
	varNode = 0;
	position = 0;
	nodeName = "SynchronizedStatementNode";
}

SynchronizedStatementNode::~SynchronizedStatementNode() {
	delete expression;

	if (statements != 0)
		delete statements;
}

void SynchronizedStatementNode::dump() {
	StatementNode::dump();	
	expression->dump();
	statements->dump();
}

Node * SynchronizedStatementNode::analyze(void *param) {
	string *name, str;
	ostringstream nameO;

	expression = (ExpressionNode *)expression->analyze(param);

	localVariableStack->enterBlock();

	nameO << "synchronized$." << synchronizedCounter++;
	str = nameO.str();
	name = stringPool->newString(str);
	varNode = new VarNode(name);
	varNode->setParentIsDeref(true);
	localVariableStack->push(varNode);
	position = varNode->getPosition();
	monitorStack->push(position);
	labelStack->addMonitor(position);

	statements = statements->analyze(param);

	localVariableStack->leaveBlock();
	monitorStack->pop();
	
	return (Node *)this;
}

ExpressionNode * SynchronizedStatementNode::getExpression() {
	return expression;
}

Seq * SynchronizedStatementNode::getStatements() {
	return statements;
}

BlockStatementNode::BlockStatementNode() {
	statements = new Seq(0, 0);
	nodeName = "BlockStatementNode";
}

BlockStatementNode::BlockStatementNode(Seq *s) {
	statements = s;
	nodeName = "BlockStatementNode";
}

BlockStatementNode::~BlockStatementNode() {
	if (statements != 0)
		delete statements;
}

void BlockStatementNode::dump() {
	statements->dump();
}

Node * BlockStatementNode::analyze(void *param) {
	localVariableStack->enterBlock();

	statements = statements->analyze(param);

	localVariableStack->leaveBlock();

	return (Node *)this;
}

Seq * BlockStatementNode::getStatements() {
	return statements;
}

void BlockStatementNode::add(Node *n) {
	if (statements != 0)
		statements->add(n);
}

ExpressionStatementNode::ExpressionStatementNode(ExpressionNode *e) {
	expression = e;
	nodeName = "ExpressionStatementNode";
}

ExpressionStatementNode::~ExpressionStatementNode() {
	if (expression != 0)
		delete expression;
}

void ExpressionStatementNode::dump() {
	StatementNode::dump();
	expression->dump();
}

Node * ExpressionStatementNode::analyze(void *param) {
	expression = (ExpressionNode *)expression->analyze(param);

	return (Node *)this;
}

IfThenStatementNode::IfThenStatementNode(ExpressionNode *c, StatementNode *t) {
	condition = c;
	thenStatement = t;
	endLabel = 0;
	nodeName = "IfThenStatementNode";
}

IfThenStatementNode::~IfThenStatementNode() {
	if (condition != 0)
		delete condition;
	if (thenStatement != 0)
		delete thenStatement;
}

void IfThenStatementNode::dump() {
	StatementNode::dump();
	condition->dump();
	thenStatement->dump();
}

Node * IfThenStatementNode::analyze(void *param) {
	labelCounter++;
	endLabel = labelCounter-1;

	condition = (ExpressionNode *)condition->analyze(param);

	if (condition->getType() != types->classTable()->find("Integer")) {
		cerr << line << ": expression of an if statement must be of"
		     << " Integer type!\n";
		err = true;
	}

	localVariableStack->enterBlock();
	thenStatement = (StatementNode *)thenStatement->analyze(param);
	localVariableStack->leaveBlock();

	return (Node *)this;
}

IfThenElseStatementNode::IfThenElseStatementNode(ExpressionNode *c, StatementNode *t,
						 StatementNode *e) {
	condition = c;
	thenStatement = t;
	elseStatement = e;
	elseLabel = 0;
	endLabel = 0;
	nodeName = "IfThenElseStatementNode";
}

IfThenElseStatementNode::~IfThenElseStatementNode() {
	if (condition != 0)
		delete condition;
	if (thenStatement != 0)
		delete thenStatement;
	if (elseStatement != 0)
		delete elseStatement;
}

void IfThenElseStatementNode::dump() {
	StatementNode::dump();
	condition->dump();
	thenStatement->dump();
	elseStatement->dump();
}

Node * IfThenElseStatementNode::analyze(void *param) {
	labelCounter += 2;
	elseLabel = labelCounter-2;
	endLabel = labelCounter-1;

	condition = (ExpressionNode *)condition->analyze(param);

	if (condition->getType() != types->classTable()->find("Integer")) {
		cerr << line << ": expression of an if statement must be of"
		     << " Integer type!\n";
		err = true;
	}

	localVariableStack->enterBlock();
	thenStatement = (StatementNode *)thenStatement->analyze(param);
	localVariableStack->leaveBlock();

	localVariableStack->enterBlock();
	elseStatement = (StatementNode *)elseStatement->analyze(param);
	localVariableStack->leaveBlock();

	return (Node *)this;
}

WhileStatementNode::WhileStatementNode(ExpressionNode *c, StatementNode *b) {
	condition = c;
	body = b;
	entryLabel = 0;
	exitLabel = 0;
	nodeName = "WhileStatementNode";
}

WhileStatementNode::~WhileStatementNode() {
	if (condition != 0)
		delete condition;
	if (body != 0)
		delete body;
}

void WhileStatementNode::dump() {
	StatementNode::dump();
	condition->dump();
	body->dump();
}

Node * WhileStatementNode::analyze(void *param) {
	LabelStackRecord *lsr;

	lsr = labelStack->push();
	entryLabel = lsr->getEntryLabel();
	exitLabel = lsr->getExitLabel();

	condition = (ExpressionNode *)condition->analyze(param);

	if (condition->getType() != types->classTable()->find("Integer")) {
		cerr << line << ": expression of a while statement must be of"
		     << " Integer type!\n";
		err = true;
	}

	localVariableStack->enterBlock();
	body = (StatementNode *)body->analyze(param);
	localVariableStack->leaveBlock();

	labelStack->pop();

	return (Node *)this;
}

ReturnStatementNode::ReturnStatementNode() {
	expression = 0;
	label = 0;
	monitors = 0;
	nodeName = "ReturnStatementNode";
}

ReturnStatementNode::ReturnStatementNode(ExpressionNode *e) {
	expression = e;
	label = 0;
	monitors = 0;
	nodeName = "ReturnStatementNode";
}

ReturnStatementNode::~ReturnStatementNode() {
	if (expression != 0)
		delete expression;
	if (monitors != 0)
		delete[] monitors;
}

void ReturnStatementNode::dump() {
	StatementNode::dump();
	if (expression != 0)
		expression->dump();
}

Node * ReturnStatementNode::analyze(void *param) {
	Type *methodType;
	string str;

	if (expression != 0)
		expression = (ExpressionNode *)expression->analyze(param);

	if (analyzingClassConst == true) {
		str = string(*(constructorAnalyzing->getMungedName()) + "$.exit");
		label = stringPool->newString(str);

		if (expression != 0) {
			cerr << line << ": return statement in class"
			     << " constructor body must not have an"
			     << " expression!\n";
			err = true;
		}
	}

	if (analyzingClassMeth == true) {
		str = string(*(methodAnalyzing->getMungedName()) + "$.exit");
		label = stringPool->newString(str);

		methodType = methodAnalyzing->getType();

		if (expression == 0) {
			cerr << line << ": return statement in method body"
			     << " must have an expression!\n";
			err = true;
		} else if (!expression->getType()->canAssignTo(methodType)) {
			cerr << line << ": return expression type "
			     << *(expression->getType()->toString())
			     << " is not convertable by assignment conversion"
			     << " to return type " << *(methodType->toString())
			     << " of method " << *(methodAnalyzing->getName())
			     << " in class "
			     << *(methodAnalyzing->getClassType()->getName())
			     << "!\n";
			err = true;
		}
	}

	if (analyzingMainBlock == true) {
		label = &mainExitLabel;

		if (expression == 0) {
			cerr << line << ": return statement in main block"
			     << " must have an expression!\n";
			err = true;
		} else if (!expression->getType()->canAssignTo(types->classTable()->find("Integer"))) {
			cerr << line << ": return expression type "
			     << *(expression->getType()->toString())
			     << " is not convertable by assignment conversion"
			     << " to return type Integer"
			     << " of main block!\n";
			err = true;
		}
	}

	monitors = monitorStack->toArray();

	return (Node *)this;
}

OutStatementNode::OutStatementNode(ExpressionNode *e) {
	expression = e;
	callToString = false;
	nodeName = "OutStatementNode";
}

OutStatementNode::~OutStatementNode() {
	if (callToString == false && expression != 0)
		delete expression;
}

void OutStatementNode::dump() {
	StatementNode::dump();
	expression->dump();
}

Node * OutStatementNode::analyze(void *param) {
	NullType *nt;
	ReferenceType *rt;
	MethodInvocationExpressionNode *mien;

	expression = (ExpressionNode *)expression->analyze(param);

	rt = (ReferenceType *)expression->getType();

	if ((nt = dynamic_cast<NullType *>(rt)) == 0 &&
	    rt->isDependant(types->classTable()->find("String")) == false) {
		callToString = true;
		mien = new MethodInvocationExpressionNode(expression,
							  new string("toString"),
							  new Seq(0, 0));
		mien->setExpressionAnalyzed(true);
		mien = (MethodInvocationExpressionNode *)mien->analyze(0);
		toStringInvocation = mien;
	}

	return (Node *)this;
}

BreakStatementNode::BreakStatementNode() {
	exitLabel = 0;
	monitors = 0;
	nodeName = "BreakStatementNode";
}

BreakStatementNode::~BreakStatementNode() {

}

Node * BreakStatementNode::analyze(void *param) {
	LabelStackRecord *lsr;

	if ((lsr = labelStack->peek()) == 0) {
		cerr << line << ": break statement is not enclosed in a while"
		     << " statement!\n";
		err = true;
		exitLabel = &mainExitLabel;
		return (Node *)this;
	}

	exitLabel = lsr->getExitLabel();
	monitors = labelStack->getMonitors();

	return (Node *)this;
}

ContinueStatementNode::ContinueStatementNode() {
	monitors = 0;
	nodeName = "ContinueStatementNode";
}

ContinueStatementNode::~ContinueStatementNode() {
	if (monitors != 0)
		delete[] monitors;
}

Node * ContinueStatementNode::analyze(void *param) {
	LabelStackRecord *lsr;

	if ((lsr = labelStack->peek()) == 0) {
		cerr << line << ": continue statement is not enclosed in a"
		     << " while statement!\n";
		err = true;
		entryLabel = &mainExitLabel;
		return (Node *)this;
	}


	entryLabel = lsr->getEntryLabel();
	monitors = labelStack->getMonitors();	
	
	return (Node *)this;
}

DerefNode::DerefNode(ExpressionNode *e) {
	expression = e;
	nodeName = "DerefNode";
}

ExpressionNode * DerefNode::getExpression() {
	return expression;
}

void DerefNode::dump() {
	ExpressionNode::dump();
	expression->dump();
}

Node * DerefNode::analyze(void *param) {
	expression->setParentIsDeref(true);
	expression = (ExpressionNode *)expression->analyze(param);
	expression->setParentIsDeref(true);

	type = expression->getType();

	return (Node *)this;
}

ConstructorInvocationStatementNode::ConstructorInvocationStatementNode(ThisExpressionNode *k, Seq *a) {
	keyword = k;
	arguments = a;
	mungedName = 0;
	callDefault = false;
	callNative = false;
	nativeIndex = 0;
	nodeName = "ConstructorInvocationStatementNode";
}

ConstructorInvocationStatementNode::~ConstructorInvocationStatementNode() {
	if (keyword != 0)
		delete keyword;
	if (arguments != 0)
		delete arguments;
}

void ConstructorInvocationStatementNode::dump() {
	if (keyword != 0)
		cerr << *(Node::toString()) << " [" << *(keyword->toString()) << "]\n";
	else
		cerr << *(Node::toString()) << '\n';

	if (arguments != 0)
		arguments->dump();
}

Node * ConstructorInvocationStatementNode::analyze(void *param) {
	string str;
	SuperExpressionNode *sen;
	ClassType *ct, *classType;

	ct = (ClassType *)param;

	arguments = arguments->analyze(param);

	if ((sen = dynamic_cast<SuperExpressionNode *>(keyword)) == 0)
		classType = ct;
	else
		classType = (ClassType *)ct->getExtendType();

	if (classType->getUseDefaultConstructor() == true) {
		callDefault = true;
		str = string(*(classType->getName()) + "_constructor");
		mungedName = stringPool->newString(str);
	}

	if (callDefault == false) {
		if ((constructor = classType->findConstructor(arguments)) == 0) {
			cerr << line << ": no matching constructor "
			     << *(classType->getName())
			     << "(" << *(arguments->toStringParams()) << ")"
			     << " in class " << *(classType->getName()) << "!\n";
			err = true;
			// DELETE
			delete this;
			return new StatementNode();
		}

		if (constructor == constructorAnalyzing) {
			cerr << line << ": constructor " << *(classType->getName())
			     << "(" << *(arguments->toStringParams())
			     << ") cannot invoke itself!\n";
			err = true;
			// DELETE
			delete this;
			return new StatementNode();
		}

		if (constructor->getIsNative() == true) {
			callNative = true;
			nativeIndex = constructor->getNativeIndex();
		}

		mungedName = constructor->getMungedName();
	}
	
	return (Node *)this;
}

LocalVarDecStatementNode::LocalVarDecStatementNode(LocalVarDecNode *d) {
	declarations = d;
	nodeName = "LocalVarDecStatementNode";
}

LocalVarDecStatementNode::~LocalVarDecStatementNode() {
	if (declarations != 0)
		delete declarations;
}

void LocalVarDecStatementNode::dump() {
	Node::dump();

	if (declarations != 0)
		declarations->dump();
}

Node * LocalVarDecStatementNode::analyze(void *param) {
	declarations = (LocalVarDecNode *)declarations->analyze(param);

	localVariableStack->push(declarations);

	return (Node *)this;
}
