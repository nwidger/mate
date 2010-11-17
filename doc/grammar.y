%{
%}

%token <str> IDENTIFIER
%token <str> STRING_LITERAL
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
%token IF ELSE WHILE NEW RETURN
%token OUT
%token IN
%token BREAK CONTINUE
%token BAD

%start CompilationUnit

%%

CompilationUnit
	: MainFunctionDeclaration
	| MainFunctionDeclaration ClassDeclarations
	| ClassDeclarations MainFunctionDeclaration
	| ClassDeclarations MainFunctionDeclaration ClassDeclarations
	;

MainFunctionDeclaration
	: Type MAIN '(' ')' Block
	;

ClassDeclarations
	: ClassDeclarations ClassDeclaration
	| ClassDeclaration
	;

ClassDeclaration
	: CLASS Identifier ClassBody
	| CLASS Identifier EXTENDS ClassType ClassBody
	;

ClassBody
	: '{' ClassBodyDeclarations '}'
	| '{' '}'
	;

ClassBodyDeclarations
	: ClassBodyDeclarations ClassBodyDeclaration
	| ClassBodyDeclaration
	;

ClassBodyDeclaration
	: ClassMemberDeclaration
	| ConstructorDeclaration
	;

ClassMemberDeclaration
	: FieldDeclaration
	| MethodDeclaration
	| OperatorDeclaration
	;

FieldDeclaration
	: Type VariableDeclarators ';'
	;

MethodDeclaration
	: Type MethodDeclarator MethodBody
	;

MethodDeclarator
	: Identifier FormalParameters
	;

MethodBody
	: Block
	;

OperatorDeclaration
	: Type OperatorDeclarator OperatorBody
	;

OperatorDeclarator
	: OPERATOR BinaryClassOperator '(' FormalParameter ')'
	| OPERATOR MinusClassOperator '(' FormalParameter ')'
	| OPERATOR UnaryClassOperator '(' ')'
	| OPERATOR MinusClassOperator '(' ')'
	;

OperatorBody
	: Block
	;

ConstructorDeclaration
	: ConstructorDeclarator ConstructorBody
	;

ConstructorDeclarator
	: Identifier FormalParameters
	;

ConstructorBody
	: '{' ConstructorInvocation BlockStatements '}'
	| '{' ConstructorInvocation '}'
	| Block
	;

ConstructorInvocation
	: THIS Arguments ';'
	| SUPER Arguments ';'
	;

FormalParameters
	: '(' FormalParameterList ')'
	| '(' ')'
	;

FormalParameterList
	: FormalParameterList ',' FormalParameter
	| FormalParameter
	;

FormalParameter
	: Type VariableDeclaratorID
	;

VariableDeclaratorID
	: Identifier
	;

Block
	: '{' BlockStatements '}'
	| '{' '}'
	;

BlockStatement
	: Statement
	;

BlockStatements
	: BlockStatements BlockStatement
	| BlockStatement
	;

Statement
	: Block
	| EmptyStatement
	| VariableDeclarationStatement
	| ExpressionStatement
	| IfThenStatement
	| IfThenElseStatement
	| WhileStatement
	| ReturnStatement
	| OutputStatement
	| BreakStatement
	| ContinueStatement
	;

VariableDeclarationStatement
	: VariableDeclaration ';'
	;

VariableDeclaration
	: Type VariableDeclarators
	;

VariableDeclarators
	: VariableDeclarators ',' VariableDeclarator
	| VariableDeclarator
	;

VariableDeclarator
	: Identifier
	;

IfThenStatement
	: IF ParenExpression Statement
	;

IfThenElseStatement
	: IF ParenExpression Statement ELSE Statement
	;

WhileStatement
	: WHILE ParenExpression Statement
	;

ReturnStatement
	: RETURN ';'
	| RETURN Expression ';'
	;

OutputStatement
	: OUT Expression ';'
	;

BreakStatement
	: BREAK ';'
	;

ContinueStatement
	: CONTINUE ';'
	;

EmptyStatement
	: ';'
	;

ParenExpression
	: '(' Expression ')'
	;

ExpressionStatement
	: StatementExpression ';'
	;

StatementExpression
	: Assignment
	| MethodInvocation
	;

Expression
	: AssignmentExpression
	;

AssignmentExpression
	: Assignment
	| EqualityExpression
	;

Assignment
	: LeftHandSide '=' AssignmentExpression
	;

LeftHandSide
	: Identifier
	| FieldAccess
	;

EqualityExpression
	: EqualityExpression EQ_OP InstanceOfExpression
	| InstanceOfExpression
	;

InstanceOfExpression
	: InstanceOfExpression INSTANCEOF_OP ClassType
	| RelationalExpression
	;

RelationalExpression
	: RelationalExpression '<' AdditiveExpression
	| RelationalExpression '>' AdditiveExpression
	| AdditiveExpression
	;

AdditiveExpression
	: AdditiveExpression '+' MultiplicativeExpression
	| AdditiveExpression '-' MultiplicativeExpression
	| MultiplicativeExpression
	;

MultiplicativeExpression
	: MultiplicativeExpression '*' UnaryExpression
	| MultiplicativeExpression '/' UnaryExpression
	| UnaryExpression
	;

UnaryExpression
	: '-' UnaryExpression
	| '!' UnaryExpression
	| CastExpression
	;

CastExpression
	: ParenExpression CastExpression
	| Primary
	;

Primary
	: Identifier
	| ParenExpression
	| THIS
	| FieldAccess
	| MethodInvocation
	| ClassInstanceCreationExpression
	| Literal
	| InputExpression
	;

InputExpression
	: IN
	;

ClassInstanceCreationExpression
	: NEW ClassType Arguments
	;

Type
	: ReferenceType
	;

ReferenceType
	: ClassType
	;

ClassType
	: Identifier
	;

FieldAccess
	: Primary '.' Identifier
	| SUPER '.' Identifier
	;

MethodInvocation
	: Identifier Arguments
	| Primary '.' Identifier Arguments
	| SUPER '.' Identifier Arguments
	;

Arguments
	: '(' ArgumentList ')'
	| '(' ')'
	;

ArgumentList
	: ArgumentList ',' Expression
	| Expression
	;

Identifier
	: IDENTIFIER
	;

Literal
	: INTEGER_LITERAL
	| NULL_LITERAL
	| STRING_LITERAL
	;

UnaryClassOperator
	: '!'
	;

BinaryClassOperator
	: '+'
	| '*'
	| '/'
	| '>'
	| '<'
	;

MinusClassOperator
	: '-'
	;

%%
