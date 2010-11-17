// Niels Widger
// Time-stamp: <16 Nov 2010 at 20:54:04 by nwidger on macros.local>

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <iostream>
using namespace std;

#include "ClassTable.h"
#include "globals.h"
#include "Node.h"
#include "Seq.h"
#include "Type.h"
#include "TypeModule.h"

void Node::encode() {
	cout << "# " << nodeName << '\n';
}

void SynchronizedStatementNode::encode() {
	Node::encode();

	expression->encode();

	cout << "  dup\n"
	     << "  monitorenter\n";

	body->encode();

	cout << "  monitorexit\n";	
}

void BlockStatementNode::encode() {
	Node::encode();
	if (statements != 0)
		statements->encode();
}

void ExpressionStatementNode::encode() {
	Node::encode();
	expression->encode();
}

void MainBlockNode::encode() {
	Node::encode();

	cout << "$" << mainEntryLabel << " " << locals << " " << '\n';	

	types->classTable()->encodeVMTs();

	cout << mainEntryLabel << ":\n";
	
	body->encode();

	cout << "  newint 0\n"
	     << mainExitLabel << ":\n"
	     << "  areturn\n";

	types->classTable()->encodeBodies();
}

void VarNode::encode() {
	Node::encode();

	if (parentIsDeref == true)
		cout << "  aload " << position << '\n';
}

void ClassFieldNode::encode() {
	Node::encode();

	cout << "  aload 0\n";

	if (parentIsDeref == true)
		cout << "  getfield " << offset << '\n';
}

void InstanceOfExpressionNode::encode() {
	Node::encode();

	expression->encode();

	cout << "  newint 0\n"
	     << "  checkcast $" << *toVmt << '\n';
}

void CastExpressionNode::encode() {
	Node::encode();

	expression->encode();

	cout << "  newint 1\n"
	     << "  checkcast $" << *toVmt << '\n';
}

void ClassInstanceCreationExpressionNode::encode() {
	Node::encode();

	cout << "  new $" << *(classType->getName()) << '\n'
	     << "  dup\n";
	
	if (arguments != 0)
		arguments->encode();

	if (callNative == true)
		cout << "  invokenative " << nativeIndex << '\n';
	else if (callDefault == true)
		cout << "  invokespecial $" << *(type->getName()) << "_constructor"
		     << " 1 1\n";
	else
		cout << "  invokespecial $" << *mungedName
		     << " " << arguments->size()+1
		     << " " << constructor->getLocals() << '\n';
}

void MethodInvocationExpressionNode::encode() {
	Node::encode();

	if (expression != 0)
		expression->encode();

	if (arguments != 0)
		arguments->encode();

	if (callNative == true)
		cout << "  invokenative " << nativeIndex << '\n';
	else if (callDirectly == true)
		cout << "  invokespecial $" << *mungedName << " " << arguments->size()+1
		     << " " << method->getLocals() << '\n';
	else
		cout << "  invokevirtual " << offset << " " << arguments->size()+1 << '\n';

	if (isStatement == true)
		cout << "  pop\n";
}

void ThisExpressionNode::encode() {
	Node::encode();

	cout << "  aload 0\n";
}

void IntegerLiteralNode::encode() {
	Node::encode();

	cout << "  newint " << value << '\n';
}

void NullLiteralNode::encode() {
	Node::encode();
	
	cout << "  aconst_null\n";
}

void StringLiteralNode::encode() {
	Node::encode();

	cout << "  newstr \"" << *str << "\"\n";
}

void BinaryOperatorNode::encode() {
	Node::encode();
	lhs->encode();
	rhs->encode();
}

void EqualityOperatorNode::encode() {
	BinaryOperatorNode::encode();

	cout << "  refcmp\n";
}

void AssignmentOperatorNode::encode() {
	BinaryOperatorNode::encode();

	if (isStatement == false)
		cout << ((usingPutfield) ? "  dup_x1\n" : "  dup\n");

	cout << "  " << *storeInstruction << '\n';
}

void UnaryOperatorNode::encode() {
	Node::encode();
	expression->encode();
}

void InExpressionNode::encode() {
	Node::encode();

	cout << "  in\n";
}

void FieldAccessOperatorNode::encode() {
	UnaryOperatorNode::encode();
	
	if (parentIsDeref == true)
		cout << "  getfield " << offset << '\n';
}

void OutStatementNode::encode() {
	Node::encode();

	if (callToString == true)
		toStringInvocation->encode();
	else
		expression->encode();

	cout << "  out\n";
}

void IfThenStatementNode::encode() {
	Node::encode();
	
	condition->encode();

	cout << "  ifeq $" << label << endLabel << '\n';
	thenStatement->encode();
	cout << label << endLabel << ":\n";
}

void IfThenElseStatementNode::encode() {
	Node::encode();
	
	condition->encode();
	cout << "  ifeq $" << label << elseLabel << '\n';
	thenStatement->encode();
	cout << "  goto $" << label << endLabel << "\n"
	     << label << elseLabel << ":\n";
	elseStatement->encode();
	cout << label << endLabel << ":\n";
}

void WhileStatementNode::encode() {
	Node::encode();

	cout << *entryLabel << ":\n";
	condition->encode();
	cout << "  ifeq $" << *exitLabel << '\n';
	body->encode();
	cout << "  goto $" << *entryLabel << '\n'
	     << *exitLabel << ":\n";
}

void BreakStatementNode::encode() {
	Node::encode();	
	
	cout << "  goto $" << *exitLabel << "\n";
}

void ContinueStatementNode::encode() {
	Node::encode();	
	
	cout << "  goto $" << *entryLabel << "\n";
}

void ReturnStatementNode::encode() {
	Node::encode();

	if (expression != 0)
		expression->encode();

	cout << "  goto $" << *label << '\n';			
}

void Seq::encode() {
	if (left != 0)
		left->encode();
	if (right != 0)
		right->encode();
}

void DerefNode::encode() {
	Node::encode();

	expression->encode();
}

void ClassType::encodeVMT() {
	cout << *name << ":\n";
	
	if (extendType == 0)
		cout << "  0\n";
	else
		cout << "  $" << *(extendType->name) << '\n';

	cout << "  " << offset+fields->size() << '\n';

	cout << "  " << methodTableSize << '\n';
	for (int i = 0; i < methodTableSize; i++) {
		if (methodTable[i]->getIsNative() == true)
			cout << "  0 " << (methodTable[i]->getNativeIndex())
			     << " \"" << *(methodTable[i]->getMungedName()) << "\"\n";
		else
			cout << "  $" << *(methodTable[i]->getMungedName()) << " "
			     << methodTable[i]->getLocals()
			     << " \"" << *(methodTable[i]->getMungedName()) << "\"\n";
	}
}

void ClassType::encodeBody() {
	ClassConstructorNode *noArgConstructor;
	Seq noArgs = Seq(0,0);
	
	if (useDefaultConstructor == true) {
		cout << *name << "_constructor:\n";
		
		if (extendType != 0) {
			cout << "  aload 0\n";

			noArgConstructor = extendType->findConstructor(&noArgs);

			if (extendType->useDefaultConstructor == true) {
				cout << "  invokespecial $" << *(extendType->name) << "_constructor 1 1\n";
			} else if (noArgConstructor != 0) {
				if (noArgConstructor->getIsNative() == false) {
					cout << "  invokespecial $" << *(extendType->name) << "_constructor 1 "
					     << noArgConstructor->getLocals() << '\n';
				} else {
					cout << "  invokenative " << noArgConstructor->getNativeIndex() << '\n';					
				}
			}
		}

		cout << *name << "_constructor$.exit:\n"
		     << "  return\n";
	}

	if (constructors != 0)
		constructors->encode();

	if (methods != 0)
		methods->encode();
}

void ConstructorInvocationStatementNode::encode() {
	Node::encode();

	if (keyword != 0)
		keyword->encode();

	if (arguments != 0)
		arguments->encode();

	if (callNative == true)
		cout << "  invokenative " << nativeIndex << '\n';
	else if (callDefault == true)
		cout << "  invokespecial $" << *mungedName << " 1 1\n";
	else
		cout << "  invokespecial $" << *mungedName
		     << " " << arguments->size()+1
		     << " " << constructor->getLocals() << '\n';
}

void ClassMethodNode::encode() {
	if (getIsNative() == true)
		return;
	
	Node::encode();

	cout << *mungedName << ":\n";

	if (body != 0)
		body->encode();

	cout << "  aconst_null\n"
	     << *mungedName << "$.exit:\n"
	     << "  areturn\n";
}

void ClassConstructorNode::encode() {
	if (getIsNative() == true)
		return;
	
	Node::encode();

	cout << *mungedName << ":\n";

	if (body != 0)
		body->encode();

	cout << *mungedName << "$.exit:\n"
	     << "  return\n";
}

void LocalVarDecStatementNode::encode() {
	Node::encode();

	if (declarations != 0)
		declarations->encode();
}

void LocalVarDecNode::encode() {
	Seq *v;
	VarNode *vn;

	for (v = variables; v != 0 && v->getRight() != 0; v = v->getLeft()) {
		vn = ((VarNode *)(v->getRight()));
		cout << "  aconst_null\n"
		     << "  astore " << vn->getPosition() << '\n';
	}
}
