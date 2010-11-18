%{

// Niels Widger
// CS 712
// Time-stamp: <17 Nov 2010 at 20:08:00 by nwidger on macros.local>

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <string>
#include <iostream>
using namespace std;

#define YYERROR_VERBOSE 1

#include "TypeModule.h"
#include "Type.h"
#include "Seq.h"
#include "Node.h"

// function declarations
extern int yyparse();
extern int getCurrentSourceLineNumber();
extern int yylex();
void yyerror(const char *s);

extern Seq *ast;
extern TypeModule *types;

%}

%union {
	Seq *seq;

        unsigned long value;
	string *str;
	char c;

	Type *type;

	Node *node;

	LocalVarDecNode *localVarDec;
	VarDecNode *varDec;
	VarNode *varNode;

	MainBlockNode *mainBlock;

	ClassDecNode *classDec;
	ClassMethodNode *classMeth;
	ClassConstructorNode *classConst;
	ClassFieldDecNode *classFieldDec;
	ClassFieldNode *classField;

	ExpressionNode *expr;
	StatementNode *stmnt;

	BlockStatementNode *blockStmnt;
};

%expect 1 // suppress if/else conflict message

%token <str> IDENTIFIER
%token <str> STRING_LITERAL
%token NEWLINE
%token TAB
%token <value> NATIVE
%token <value> INTEGER_LITERAL
%token NULL_LITERAL
%token OPERATOR

%token <str> EQ_OP
%token <str> INSTANCEOF_OP
%token <str> '+'
%token <str> '>'
%token <str> '-'
%token <str> '!'
%token <str> '/'
%token <str> '<'
%token <str> '*'

%token MAIN CLASS EXTENDS
%token THIS SUPER
%token SYNCHRONIZED
%token IF ELSE WHILE NEW RETURN
%token OUT
%token IN
%token BREAK CONTINUE
%token BAD

%type <seq> CompilationUnit
%type <mainBlock> MainFunctionDeclaration

%type <seq> ClassDeclarations
%type <classDec> ClassDeclaration
%type <seq> ClassBody
%type <seq> ClassBodyDeclarations
%type <node> ClassBodyDeclaration
%type <node> ClassMemberDeclaration

%type <classFieldDec> FieldDeclaration

%type <stmnt> VariableDeclarationStatement
%type <localVarDec> VariableDeclaration
%type <seq> VariableDeclarators
%type <varNode> VariableDeclarator

%type <classMeth> MethodDeclaration
%type <classMeth> MethodDeclarator
%type <blockStmnt> MethodBody

%type <classMeth> OperatorDeclaration
%type <classMeth> OperatorDeclarator
%type <blockStmnt> OperatorBody

%type <classConst> ConstructorDeclaration
%type <classConst> ConstructorDeclarator
%type <blockStmnt> ConstructorBody

%type <stmnt> ConstructorInvocation

%type <seq> FormalParameters
%type <seq> FormalParameterList
%type <varDec> FormalParameter
%type <varNode> VariableDeclaratorID

%type <blockStmnt> Block
%type <seq> BlockStatements
%type <stmnt> BlockStatement

%type <stmnt> Statement
%type <stmnt> SynchronizedStatement
%type <stmnt> IfThenStatement
%type <stmnt> IfThenElseStatement
%type <stmnt> WhileStatement
%type <stmnt> ReturnStatement
%type <stmnt> OutputStatement
%type <stmnt> BreakStatement
%type <stmnt> ContinueStatement
%type <stmnt> EmptyStatement

%type <expr> ParenExpression
%type <expr> ExpressionStatement
%type <expr> StatementExpression
%type <expr> Expression

%type <expr> AssignmentExpression
%type <expr> Assignment
%type <expr> LeftHandSide

%type <expr> EqualityExpression
%type <expr> RelationalExpression

%type <expr> InstanceOfExpression
%type <expr> AdditiveExpression
%type <expr> MultiplicativeExpression
%type <expr> UnaryExpression

%type <expr> InputExpression

%type <expr> CastExpression

%type <expr> Primary

%type <expr> THIS
%type <expr> SUPER

%type <expr> ClassInstanceCreationExpression
%type <type> Type
%type <type> ReferenceType
%type <type> ClassType

%type <expr> FieldAccess

%type <expr> MethodInvocation

%type <seq> Arguments
%type <seq> ArgumentList

%type <str> Identifier

%type <expr> Literal

%type <str> UnaryClassOperator
%type <str> BinaryClassOperator
%type <str> MinusClassOperator

%start CompilationUnit

%%

CompilationUnit
	: MainFunctionDeclaration
	{
	  $$ = new Seq(0, $1);
	  ast = $$;
	}
	| MainFunctionDeclaration ClassDeclarations
	{
	  $$ = $2->add($1);
	  ast = $$;
	}
	| ClassDeclarations MainFunctionDeclaration
	{
	  $$ = new Seq(0, $2);
	  $$->add($1);
	  ast = $$;
	}
	| ClassDeclarations MainFunctionDeclaration ClassDeclarations
	{
	  $$ = $3->add($2)->add($1);
	  ast = $$;
	}
	;

MainFunctionDeclaration
	: Type MAIN '(' ')' Block
	{
	  if (*($1->getName()) != "Integer") {
	     cerr << "mc: " << @$.first_line << ": parse error\n";
	     return 1;
	  }

	  $$ = new MainBlockNode($5);
	}
	;

ClassDeclarations
	: ClassDeclarations ClassDeclaration
	{
	  $$ = new Seq($1, $2);
	}
	| ClassDeclaration
	{
	  $$ = new Seq(0, $1);
	}
	;

ClassDeclaration
	: CLASS Identifier ClassBody
	{
	  $$ = new ClassDecNode($2, $3);
	  $$->setLineNumber(@$.first_line);
	}
	| CLASS Identifier EXTENDS ClassType ClassBody
	{
	  $$ = new ClassDecNode($2, ((ClassType *)$4)->getName(), $5);
	  delete $4;
	  $$->setLineNumber(@$.first_line);
	}
	;

ClassBody
	: '{' ClassBodyDeclarations '}'
	{
	  $$ = $2;
	}
	| '{' '}'
	{
	  $$ = new Seq(0, 0);
	}
	;

ClassBodyDeclarations
	: ClassBodyDeclarations ClassBodyDeclaration
	{
	  $$ = new Seq($1, $2);
	}
	| ClassBodyDeclaration
	{
	  $$ = new Seq(0, $1);
	}
	;

ClassBodyDeclaration
	: ClassMemberDeclaration
	{
	  $$ = $1;
	}
	| ConstructorDeclaration
	{
	  $$ = $1;
	}
	;

ClassMemberDeclaration
	: FieldDeclaration
	{
	  $$ = $1;
	}
	| MethodDeclaration
	{
	  $$ = $1;
	}
	| OperatorDeclaration
	{
	  $$ = $1;
	}
	;

FieldDeclaration
	: Type VariableDeclarators ';'
	{
	  $$ = new ClassFieldDecNode($1, $2);
	  $$->setLineNumber(@$.first_line);
	}
	;

MethodDeclaration
	: NATIVE Type MethodDeclarator ';'
	{
	  $3->setNativeIndex($1);
	  $3->setType($2);
	  $3->setBody(new BlockStatementNode());
	  $$ = $3;
	  $$->setLineNumber(@$.first_line);
	}
	| Type MethodDeclarator MethodBody
	{
	  $2->setType($1);
	  $2->setBody($3);
	  $$ = $2;
	  $$->setLineNumber(@$.first_line);
	}
	;

MethodDeclarator
	: Identifier FormalParameters
	{
	  $$ = new ClassMethodNode($1, $2);
	}
	;

MethodBody
	: Block
	{
	  $$ = $1;
	}
	;

OperatorDeclaration
	: NATIVE Type OperatorDeclarator ';'
	{
	  $3->setNativeIndex($1);
	  $3->setType($2);
	  $3->setBody(new BlockStatementNode());
	  $$ = $3;
	  $$->setLineNumber(@$.first_line);
	}
	| Type OperatorDeclarator OperatorBody
	{
	  $2->setType($1);
	  $2->setBody($3);
	  $$ = $2;
	  $$->setLineNumber(@$.first_line);
	}
	;

OperatorDeclarator
	: OPERATOR BinaryClassOperator '(' FormalParameter ')'
	{
	  $$ = new ClassMethodNode($2, new Seq(0, $4));
	}
	| OPERATOR MinusClassOperator '(' FormalParameter ')'
	{
	  $$ = new ClassMethodNode($2, new Seq(0, $4));
	}
	| OPERATOR UnaryClassOperator '(' ')'
	{
	  $$ = new ClassMethodNode($2, new Seq(0, 0));
	}
	| OPERATOR MinusClassOperator '(' ')'
	{
	  $$ = new ClassMethodNode($2, new Seq(0, 0));
	}
	;

OperatorBody
	: Block
	{
	  $$ = $1;
	}
	;

ConstructorDeclaration
	: NATIVE ConstructorDeclarator ';'
	{
	  $2->setNativeIndex($1);	
	  $2->setBody(new BlockStatementNode());
	  $$ = $2;
	}
	| ConstructorDeclarator ConstructorBody
	{
	  $1->setBody($2);
	  $$ = $1;
	}
	;

ConstructorDeclarator
	: Identifier FormalParameters
	{
	  $$ = new ClassConstructorNode($1, $2);
	  $$->setLineNumber(@$.first_line);
	}
	;

ConstructorBody
	: '{' ConstructorInvocation BlockStatements '}'
	{
	  $$ = new BlockStatementNode($3->add($2));
	}
	| '{' ConstructorInvocation '}'
	{
	  $$ = new BlockStatementNode(new Seq(0, $2));
	}
	| Block
	{
	  $1->add(new ConstructorInvocationStatementNode(new SuperExpressionNode(), new Seq(0, 0)));
	  $$ = $1;
	}
	;

ConstructorInvocation
	: THIS Arguments ';'
	{
	  $$ = new ConstructorInvocationStatementNode(new ThisExpressionNode(), $2);
	}
	| SUPER Arguments ';'
	{
	  $$ = new ConstructorInvocationStatementNode(new SuperExpressionNode(), $2);
	}
	;

FormalParameters
	: '(' FormalParameterList ')'
	{
	  $$ = $2;
	}
	| '(' ')'
	{
	  $$ = new Seq(0, 0);
	}
	;

FormalParameterList
	: FormalParameterList ',' FormalParameter
	{
	  $$ = new Seq($1, $3);
	}
	| FormalParameter
	{
	  $$ = new Seq(0, $1);
	}
	;

FormalParameter
	: Type VariableDeclaratorID
	{
	  $$ = new VarDecNode($1, new Seq(0, $2));
	  $$->setLineNumber(@$.first_line);
	}
	;

VariableDeclaratorID
	: Identifier
	{
	  $$ = new VarNode($1);
	  $$->setLineNumber(@$.first_line);
	}
	;

Block
	: '{' BlockStatements '}'
	{
	  $$ = new BlockStatementNode($2);
	}
	| '{' '}'
	{
	  $$ = new BlockStatementNode(new Seq(0, 0));
	}
	;

BlockStatement
	: Statement
	{
	  $$ = $1;
	}
	;

BlockStatements
	: BlockStatements BlockStatement
	{
	  $$ = new Seq($1, $2);
	}
	| BlockStatement
	{
	  $$ = new Seq(0, $1);
	}
	;

SynchronizedStatement
	: SYNCHRONIZED '(' Expression ')' '{' BlockStatements '}'
	{
	  $$ = new SynchronizedStatementNode($3, $6);
	}
	| SYNCHRONIZED '(' Expression ')' '{' '}'
	{
	  $$ = new SynchronizedStatementNode($3, new Seq(0, 0));
	}
	;
	
Statement
	: Block
	{
	  $$ = $1;
	}
	| SynchronizedStatement
	{
	  $$ = $1;
	}
	| EmptyStatement
	{
	  $$ = $1;
	}
	| VariableDeclarationStatement
	{
	  $$ = $1;
	}
	| ExpressionStatement
	{
	  $1->setIsStatement(true);
	  $$ = new ExpressionStatementNode($1);
	}
	| IfThenStatement
	{
	  $$ = $1;
	}
	| IfThenElseStatement
	{
	  $$ = $1;
	}
	| WhileStatement
	{
	  $$ = $1;
	}
	| ReturnStatement
	{
	  $$ = $1;
	}
	| OutputStatement
	{
	  $$ = $1;
	}
	| BreakStatement
	{
	  $$ = $1;
	}
	| ContinueStatement
	{
	  $$ = $1;
	}
	;

VariableDeclarationStatement
	: VariableDeclaration ';'
	{
	  $$ = new LocalVarDecStatementNode($1);
	}
	;

VariableDeclaration
	: Type VariableDeclarators
	{
	  $$ = new LocalVarDecNode($1, $2);
	  $$->setLineNumber(@$.first_line);
	}
	;

VariableDeclarators
	: VariableDeclarators ',' VariableDeclarator
	{
	  $$ = new Seq($1, $3);
	}
	| VariableDeclarator
	{
	  $$ = new Seq(0, $1);
	}
	;

VariableDeclarator
	: Identifier
	{
	  $$ = new VarNode($1);
	  $$->setLineNumber(@$.first_line);
	}
	;

IfThenStatement
	: IF ParenExpression Statement
	{
	  $$ = new IfThenStatementNode($2, $3);
	  $$->setLineNumber(@$.first_line);
	}
	;

IfThenElseStatement
	: IF ParenExpression Statement ELSE Statement
	{
	  $$ = new IfThenElseStatementNode($2, $3, $5);
	  $$->setLineNumber(@$.first_line);
	}
	;

WhileStatement
	: WHILE ParenExpression Statement
	{
	  $$ = new WhileStatementNode($2, $3);
	  $$->setLineNumber(@$.first_line);
	}
	;

ReturnStatement
	: RETURN ';'
	{
	  $$ = new ReturnStatementNode();
	}
	| RETURN Expression ';'
	{
	  $$ = new ReturnStatementNode($2);
	}
	;

OutputStatement
	: OUT Expression ';'
	{
	  $$ = new OutStatementNode($2);
	}
	;

BreakStatement
	: BREAK ';'
	{
	  $$ = new BreakStatementNode();
	}
	;

ContinueStatement
	: CONTINUE ';'
	{
	  $$ = new ContinueStatementNode();
	}
	;

EmptyStatement
	: ';'
	{
	  $$ = new StatementNode();
	}
	;

ParenExpression
	: '(' Expression ')'
	{
	  $2->setIsParenthesized(true);
	  $$ = $2;
	}
	;

ExpressionStatement
	: StatementExpression ';'
	{
	  $$ = $1;
	}
	;

StatementExpression
	: Assignment
	{
	  $$ = $1;
	}
	| MethodInvocation
	{
	  $$ = $1;
	}
	;

Expression
	: AssignmentExpression
	{
	  $$ = $1;
	}
	;

AssignmentExpression
	: Assignment
	{
	  $$ = $1;
	}
	| EqualityExpression
	{
	  $$ = $1;
	}
	;

Assignment
	: LeftHandSide '=' AssignmentExpression
	{
	  $$ = new AssignmentOperatorNode($1, $3);
	}
	;

LeftHandSide
	: Identifier
	{
	  $$ = new DerefNode(new VarNode($1));
	}
	| FieldAccess
	{
	  $$ = $1;
	}
	;

EqualityExpression
	: EqualityExpression EQ_OP InstanceOfExpression
	{
	  $$ = new EqualityOperatorNode($1, $3);
	}
	| InstanceOfExpression
	{
	  $$ = $1;
	}
	;

InstanceOfExpression
	: InstanceOfExpression INSTANCEOF_OP ClassType
	{
	  $$ = new InstanceOfExpressionNode($1, $3);
	}
	| RelationalExpression
	{
	  $$ = $1;
	}
	;
	
RelationalExpression
	: RelationalExpression '<' AdditiveExpression
	{
	  $$ = new MethodInvocationExpressionNode($1, $2, new Seq(0, $3));
	}
	| RelationalExpression '>' AdditiveExpression
	{
	  $$ = new MethodInvocationExpressionNode($1, $2, new Seq(0, $3));
	}
	| AdditiveExpression
	{
	  $$ = $1;
	}
	;

AdditiveExpression
	: AdditiveExpression '+' MultiplicativeExpression
	{
	  $$ = new MethodInvocationExpressionNode($1, $2, new Seq(0, $3));
	}
	| AdditiveExpression '-' MultiplicativeExpression
	{
	  $$ = new MethodInvocationExpressionNode($1, $2, new Seq(0, $3));
	}
	| MultiplicativeExpression
	{
	  $$ = $1;
	}
	;

MultiplicativeExpression
	: MultiplicativeExpression '*' UnaryExpression
	{
	  $$ = new MethodInvocationExpressionNode($1, $2, new Seq(0, $3));
	}
	| MultiplicativeExpression '/' UnaryExpression
	{
	  $$ = new MethodInvocationExpressionNode($1, $2, new Seq(0, $3));
	}
	| UnaryExpression
	{
	  $$ = $1;
	}
	;

UnaryExpression
	: '-' UnaryExpression
	{
	  $2->setParentIsNegate(true);
	  $$ = new MethodInvocationExpressionNode($2, $1, new Seq(0, 0));
	}
	| '!' UnaryExpression
	{
	  $$ = new MethodInvocationExpressionNode($2, $1, new Seq(0, 0));
	}
	| CastExpression
	{
	  $$ = $1;
	}
	;

CastExpression
	: ParenExpression CastExpression
	{
	  $$ = new CastExpressionNode($1, $2);
	}
	| Primary
	{
	  $$ = $1;
	}
	;

Primary
	: Identifier
	{
	  $$ = new DerefNode(new VarNode($1));
	}
	| ParenExpression
	{
	  $$ = $1;
	}
	| THIS
	{
	  $$ = new ThisExpressionNode();
	}
	| FieldAccess
	{
	  $$ = $1;
	}
	| MethodInvocation
	{
	  $$ = $1;
	}
	| ClassInstanceCreationExpression
	{
	  $$ = $1;
	}
	| Literal
	{
	  $$ = $1;
	}
	| InputExpression
	{
	  $$ = $1;
	}
	;

InputExpression
	: IN
	{
	  $$ = new InExpressionNode();
	}
	;

ClassInstanceCreationExpression
	: NEW ClassType Arguments
	{
	  $$ = new ClassInstanceCreationExpressionNode($2->getName(), $3);
	  delete $2;
	}
	;

Type
	: ReferenceType
	{
	  $$ = $1;
	}
	;

ReferenceType
	: ClassType
	{
	  $$ = $1;
	}
	;

ClassType
	: Identifier
	{
	  $$ = new ClassType($1);
	}
	;

FieldAccess
	: Primary '.' Identifier
	{
	  $$ = new DerefNode(new FieldAccessOperatorNode($1, $3));
	}
	| SUPER '.' Identifier
	{
	  $$ = new DerefNode(new FieldAccessOperatorNode(new SuperExpressionNode(), $3));
	}
	;

MethodInvocation
	: Identifier Arguments
	{
	  $$ = new MethodInvocationExpressionNode(new ThisExpressionNode(), $1, $2);
	}
	| Primary '.' Identifier Arguments
	{
	  $$ = new MethodInvocationExpressionNode($1, $3, $4);
	}
	| SUPER '.' Identifier Arguments
	{
	  $$ = new MethodInvocationExpressionNode(new SuperExpressionNode(), $3, $4);
	}
	;

Arguments
	: '(' ArgumentList ')'
	{
	  $$ = $2;
	}
	| '(' ')'
	{
	  $$ = new Seq(0, 0);
	}
	;

ArgumentList
	: ArgumentList ',' Expression
	{
	  $$ = new Seq($1, $3);
	}
	| Expression
	{
	  $$ = new Seq(0, $1);
	}
	;

Identifier
	: IDENTIFIER
	{
	  $$ = $1;
	}
	;

Literal
	: INTEGER_LITERAL
	{
	  $$ = new IntegerLiteralNode($1);
	}
	| NULL_LITERAL
	{
	  $$ = new NullLiteralNode();
	}
	| STRING_LITERAL
	{
	  $$ = new StringLiteralNode($1);
	}
	;

UnaryClassOperator
	: '!'
	{
	  $$ = $1;
	}
	;

BinaryClassOperator
	: '+'
	{
	  $$ = $1;
	}
	| '*'
	{
	  $$ = $1;	
	}
	| '/'
	{
	  $$ = $1;	
	}
	| '>'
	{
	  $$ = $1;	
	}
	| '<'
	{
	  $$ = $1;	
	}
	;

MinusClassOperator
	: '-'
	{
	  $$ = $1;
	}
	;

%%

void yyerror(const char *s) {
        cerr << "mc: " << getCurrentSourceLineNumber() << ": " << s << '\n';
}
