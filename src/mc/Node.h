// Niels Widger
// Time-stamp: <17 Nov 2010 at 12:46:14 by nwidger on macros.local>

#ifndef _NODE_H
#define _NODE_H

#include <string>

class BlockStatementNode;
class ClassMethodNode;
class ClassConstructorNode;
class ClassTable;
class ClassType;
class LabelStack;
class LocalVariableStack;
class LocalVariableStackRecord;
class MainBlockNode;
class ReferenceType;
class Seq;
class Type;
class TypeModule;
class VarNode;

// -----------------------------------------------------------------------------

class Node {
public:
	Node();
	Node(const char *n);
	virtual ~Node();
	virtual void dump();
	virtual string * toString();
	virtual void setLineNumber(int l);
	virtual int getLineNumber();
	virtual Node * analyze(void *param);
	virtual void encode();
	virtual void setMarked(bool m);
	virtual bool getMarked();
	virtual void setLocals(int l);
	virtual int getLocals();
protected:
	int line;
	string nodeName;
	bool marked;
	int locals;
};

// -----------------------------------------------------------------------------

class MainBlockNode : public Node {
public:
	MainBlockNode(BlockStatementNode *b);
	~MainBlockNode();
	void dump();
	void encode();
	Node * analyze(void *param);
protected:
	BlockStatementNode *body;
};

// -----------------------------------------------------------------------------
// VarDecNode
// -----------------------------------------------------------------------------

class VarDecNode : public Node {
public:
	VarDecNode(Type *t, Seq *v);
	~VarDecNode();
	virtual Type * getType();
	virtual Seq * getVariables();
	virtual VarNode * findVariable(string *n);
	virtual void dump();
	virtual string * toString();
        virtual void toStringAux(Seq *v, string *str);
	virtual bool equalType(VarDecNode *vdn);
	Node * analyze(void *param);
protected:
	Type *type;
	Seq *variables;
	bool doNotDelete;
};

class LocalVarDecNode : public VarDecNode {
public:
	LocalVarDecNode(Type *t, Seq *v);
	void encode();
	Node * analyze(void *param);
};

// -----------------------------------------------------------------------------

class ClassDecNode : public Node {
public:
	ClassDecNode(string *n, Seq *b);
	ClassDecNode(string *n, string *e, Seq *b);
	~ClassDecNode();
	void dump();
	string * getName();
	Seq * getBody();
	Node * analyze(void *param);
protected:
	string *name;
	string *extends;
	Seq *body;
};

class ClassConstructorNode : public Node {
public:
	ClassConstructorNode(string *n, Seq *p);
	~ClassConstructorNode();
	void setBody(BlockStatementNode *b);
	BlockStatementNode * getBody();	
	ClassType * getClassType();	
	void dump();
	void encode();
	string * toString();
	void toStringAux(Seq *p, string *str);
	VarNode * findParameter(string *n);
	Node * analyze(void *param);
	bool equal(ClassConstructorNode *ccn);
	Seq * getParameters();
	string * getMungedName();
	void generateMungedName();
	string * getName();
	void setNativeIndex(int i);	
	bool getIsNative();
	int getNativeIndex();
protected:
	ClassType *classType;
	string *name;
	string *mungedName;
	Seq *parameters;
	BlockStatementNode *body;
	bool isNative;
	int nativeIndex;	
};

class ClassMethodNode : public Node {
public:
	ClassMethodNode(string *n, Seq *p);
	~ClassMethodNode();
	BlockStatementNode * getBody();	
	void setClassType(ClassType *c);
	ClassType * getClassType();
	void setType(Type *t);
	Type * getType();
	void setBody(BlockStatementNode *b);
	VarNode * findParameter(string *n);
	Seq * getParameters();
	void setOffset(int o);
	int getOffset();
	string * getName();
	void dump();
	string * toString();
	void toStringAux(Seq *p, string *str);
	Node * analyze(void *param);
	bool equal(ClassMethodNode *cmn);
	string * getMungedName();
	void setMungedName(string *m);
	void generateMungedName();
	void setNativeIndex(int i);
	bool getIsNative();
	int getNativeIndex();
	void encode();
protected:
	ClassType *classType;
	Type *type;
	string *name;
	string *mungedName;
	Seq *parameters;
	int offset;
	BlockStatementNode *body;
	bool isNative;
	int nativeIndex;
};

class ClassFieldDecNode : public VarDecNode {
public:
	ClassFieldDecNode(Type *t, Seq *v);
	Node * analyze(void *param);
};

// -----------------------------------------------------------------------------
// ExpressionNode
// -----------------------------------------------------------------------------

class ExpressionNode : public Node {
public:
	ExpressionNode();
	virtual void dump();
	virtual void setParentIsNegate(bool p);
	virtual void setParentIsDeref(bool p);
	virtual void setIsParenthesized(bool i);		
	virtual bool equalType(ExpressionNode *e);
	void setType(Type *t);
	Type * getType();
	void setIsStatement(bool i);
	bool getIsStatement();
protected:
	bool parentIsNegate;
	bool parentIsDeref;
	bool isParenthesized;
	bool isStatement;
	Type *type;
};

// -----------------------------------------------------------------------------
// InExpressionNode
// -----------------------------------------------------------------------------

class InExpressionNode : public ExpressionNode {
public:
	InExpressionNode();
	void encode();
	Node * analyze(void *param);
};

// -----------------------------------------------------------------------------
// InstanceOfExpressionNode
// -----------------------------------------------------------------------------

class InstanceOfExpressionNode : public ExpressionNode {
public:
	InstanceOfExpressionNode(ExpressionNode *e, Type *t);
	~InstanceOfExpressionNode();
	void dump();
	Node * analyze(void *param);
	void encode();
protected:
	ExpressionNode *expression;
	ClassType *toType;
	string *toVmt;
};

// -----------------------------------------------------------------------------
// CastExpressionNode
// -----------------------------------------------------------------------------

class CastExpressionNode : public ExpressionNode {
public:
	CastExpressionNode(ExpressionNode *c, ExpressionNode *e);
	~CastExpressionNode();
	void dump();
	Node * analyze(void *param);
	void encode();
protected:
	ReferenceType *castType;
	string *toVmt;
	int rank;
	Type *expressionType;
	ExpressionNode *className;
	ExpressionNode *expression;
};

// -----------------------------------------------------------------------------
// ClassInstanceExpressionNode
// -----------------------------------------------------------------------------

class ClassInstanceCreationExpressionNode : public ExpressionNode {
public:
	ClassInstanceCreationExpressionNode(string *n, Seq *a);
	~ClassInstanceCreationExpressionNode();
	void dump();
	void encode();
	Node * analyze(void *param);
protected:
	string *name;
	ClassType *classType;
	Seq *arguments;
	int goodLabel;
	string *mungedName;
	bool callNative;
	bool callDefault;
	int nativeIndex;
	ClassConstructorNode *constructor;
};

// -----------------------------------------------------------------------------
// MethodInvocationExpressionNode
// -----------------------------------------------------------------------------

class MethodInvocationExpressionNode : public ExpressionNode {
public:
	MethodInvocationExpressionNode(ExpressionNode *e, string *m, Seq *a);
	~MethodInvocationExpressionNode();
	void dump();
	void encode();
	void setExpressionAnalyzed(bool e);
	bool getExpressionAnalyzed();
	Node * analyze(void *param);
protected:
	ExpressionNode *expression;
	string *methodName;
	Seq *arguments;
	ClassMethodNode *method;
	string *mungedName;
	int offset;
	bool callDirectly;
	bool callNative;
	bool expressionAnalyzed;
	int nativeIndex;
};

// -----------------------------------------------------------------------------
// DerefNode
// -----------------------------------------------------------------------------

class DerefNode : public ExpressionNode {
public:
	DerefNode(ExpressionNode *e);
	ExpressionNode * getExpression();
	void dump();
	void encode();
	Node * analyze(void *param);
protected:
	ExpressionNode *expression;
};

// -----------------------------------------------------------------------------
// VarNode
// -----------------------------------------------------------------------------

class VarNode : public ExpressionNode {
public:
	VarNode(string *n);
	void encode();
	virtual void setPosition(int p);
	virtual int getPosition();
	virtual string * getName();
	virtual void dump();
	virtual string * toString();
	virtual bool equal(VarNode *vn);
	Node * analyze(void *param);
protected:
	string *name;
	int position;
	bool isLocal;
};

class ClassFieldNode : public VarNode {
public:
	ClassFieldNode(string *n);
	ClassFieldNode(VarNode *vn);
	void encode();
	string * toString();
	void setOffset(int o);
	int getOffset();
	Node * analyze(void *param, void *param2);
protected:
	int offset;
};

class MainVarNode : public VarNode {
public:
	MainVarNode(string *n);
	Node * analyze(void *param);
protected:
};

class ClassVarNode : public VarNode {
public:
	ClassVarNode(string *n);
	Node * analyze(void *param);
protected:
	ClassType *classType;
};

// -----------------------------------------------------------------------------
// ThisExpressionNode
// -----------------------------------------------------------------------------

class ThisExpressionNode : public ExpressionNode {
public:
	ThisExpressionNode();
	virtual void dump();
	virtual string * toString();
	virtual void encode();
	virtual Node * analyze(void *param);
};

// -----------------------------------------------------------------------------
// SuperExpressionNode
// -----------------------------------------------------------------------------

class SuperExpressionNode : public ThisExpressionNode {
public:
	SuperExpressionNode();
	Node * analyze(void *param);
	string * toString();	
};

// -----------------------------------------------------------------------------
// IntegerLiteralNode
// -----------------------------------------------------------------------------

class IntegerLiteralNode : public ExpressionNode {
public:
	IntegerLiteralNode(unsigned long v);
	void dump();
	void encode();
	Node * analyze(void *param);
protected:
	unsigned long value;
};

// -----------------------------------------------------------------------------
// NullLiteralNode
// -----------------------------------------------------------------------------

class NullLiteralNode : public ExpressionNode {
public:
	NullLiteralNode();
	void dump();
	void encode();	
protected:
};

// -----------------------------------------------------------------------------
// StringLiteralNode
// -----------------------------------------------------------------------------

class StringLiteralNode : public ExpressionNode {
public:
	StringLiteralNode(string *s);
	void dump();
	void encode();
	Node * analyze(void *param);
protected:
	string *str;
};

// -----------------------------------------------------------------------------
// UnaryOperatorNode
// -----------------------------------------------------------------------------

class UnaryOperatorNode : public ExpressionNode {
public:
	UnaryOperatorNode(ExpressionNode *e);
	~UnaryOperatorNode();
	virtual void dump();
	virtual void encode();
	virtual Node * analyze(void *param);
protected:
	ExpressionNode *expression;
};

// -----------------------------------------------------------------------------
// FieldAccessOperatorNode
// -----------------------------------------------------------------------------

class FieldAccessOperatorNode : public UnaryOperatorNode {
public:
	FieldAccessOperatorNode(ExpressionNode *e, string *i);
	void dump();
	void encode();
	Node * analyze(void *param);
	int getOffset();
protected:
	string *identifier;
	int offset;
};

// -----------------------------------------------------------------------------
// BinaryOperatorNode
// -----------------------------------------------------------------------------

class BinaryOperatorNode : public ExpressionNode {
public:
	BinaryOperatorNode(ExpressionNode *l, ExpressionNode *r);
	~BinaryOperatorNode();
	virtual void dump();
	virtual void encode();
	virtual Node * analyze(void *param);
protected:
	ExpressionNode *lhs;
	ExpressionNode *rhs;
};

// -----------------------------------------------------------------------------
// EqualityOperatorNode
// -----------------------------------------------------------------------------

class EqualityOperatorNode : public BinaryOperatorNode {
public:
	EqualityOperatorNode(ExpressionNode *l, ExpressionNode *r);
	void encode();
	Node * analyze(void *param);
protected:
};

// -----------------------------------------------------------------------------
// AssignmentOperatorNode
// -----------------------------------------------------------------------------

class AssignmentOperatorNode : public BinaryOperatorNode {
public:
	AssignmentOperatorNode(ExpressionNode *l, ExpressionNode *r);
	void encode();
	Node * analyze(void *param);
protected:
	string *storeInstruction;
	bool usingPutfield;
};

// -----------------------------------------------------------------------------
// StatementNode
// -----------------------------------------------------------------------------

class StatementNode : public Node {
public:
	StatementNode();
	void dump();
protected:
};

class SynchronizedStatementNode : public StatementNode {
public:
	SynchronizedStatementNode(ExpressionNode *e, BlockStatementNode *b);
	~SynchronizedStatementNode();
	void dump();
	void encode();
	Node * analyze(void *param);
	ExpressionNode * getExpression();
	BlockStatementNode * getBody();
protected:
	ExpressionNode *expression;
	BlockStatementNode *body;
	VarNode *varNode;
};

class BlockStatementNode : public StatementNode {
public:
	BlockStatementNode();
	BlockStatementNode(Seq *s);
	~BlockStatementNode();
	void add(Node *n);
	void dump();
	void encode();
	Node * analyze(void *param);
	Seq * getStatements();
protected:
	Seq *statements;
};

class ExpressionStatementNode : public StatementNode {
public:
	ExpressionStatementNode(ExpressionNode *e);
	~ExpressionStatementNode();
	void dump();
	void encode();
	Node * analyze(void *param);
protected:
	ExpressionNode *expression;
};

class IfThenStatementNode : public StatementNode {
public:
	IfThenStatementNode(ExpressionNode *c, StatementNode *t);
	~IfThenStatementNode();
	void dump();
	void encode();
	Node * analyze(void *param);
protected:
	ExpressionNode *condition;
	StatementNode *thenStatement;
	int endLabel;
};

class IfThenElseStatementNode : public StatementNode {
public:
	IfThenElseStatementNode(ExpressionNode *c, StatementNode *t, 
				StatementNode *e);
	~IfThenElseStatementNode();
	void dump();
	void encode();
	Node * analyze(void *param);
protected:
	ExpressionNode *condition;
	StatementNode *thenStatement;
	StatementNode *elseStatement;
	int elseLabel;
	int endLabel;
};

class WhileStatementNode : public StatementNode {
public:
	WhileStatementNode(ExpressionNode *c, StatementNode *b);
	~WhileStatementNode();
	void dump();
	void encode();
	Node * analyze(void *param);
protected:
	ExpressionNode *condition;
	StatementNode *body;
	string *entryLabel;
	string *exitLabel;
};

class ReturnStatementNode : public StatementNode {
public:
	ReturnStatementNode();
	ReturnStatementNode(ExpressionNode *e);
	~ReturnStatementNode();
	void dump();
	void encode();
	Node * analyze(void *param);	
protected:
	ExpressionNode *expression;
	string *label;
};

class OutStatementNode : public StatementNode {
public:
	OutStatementNode(ExpressionNode *e);
	~OutStatementNode();
	void dump();
	void encode();
	Node * analyze(void *param);	
protected:
	ExpressionNode *expression;
	bool callToString;
	MethodInvocationExpressionNode *toStringInvocation;	
};

class BreakStatementNode : public StatementNode {
public:
	BreakStatementNode();
	void encode();
	Node * analyze(void *param);	
protected:
	string *exitLabel;
};

class ContinueStatementNode : public StatementNode {
public:
	ContinueStatementNode();
	void encode();
	Node * analyze(void *param);	
protected:
	string *entryLabel;
};

class ConstructorInvocationStatementNode : public StatementNode {
public:
	ConstructorInvocationStatementNode(ThisExpressionNode *k, Seq *a);
	~ConstructorInvocationStatementNode();
	void dump();
	void encode();
	Node * analyze(void *param);
protected:
	ThisExpressionNode *keyword;
	Seq *arguments;
	string *mungedName;
	bool callDefault;
	bool callNative;
	int nativeIndex;
	ClassConstructorNode *constructor;
};

class LocalVarDecStatementNode : public StatementNode {
public:
	LocalVarDecStatementNode(LocalVarDecNode *d);
	~LocalVarDecStatementNode();
	void dump();
	void encode();
	Node * analyze(void *param);
protected:
	LocalVarDecNode *declarations;
};

#endif
