// Niels Widger
// Time-stamp: <30 Nov 2009 at 19:20:21 by nwidger on macros.local>

#ifndef _GLOBALS_H
#define _GLOBALS_H

#include <string>

class ClassConstructorNode;
class ClassMethodNode;
class LabelStack;
class LocalVariableStack;
class MainBlockNode;
class Seq;
class StringPool;
class TypeModule;

extern StringPool *stringPool;
extern TypeModule *types;
extern LocalVariableStack *localVariableStack;
extern LabelStack *labelStack;
extern Seq *ast;

extern int labelCounter;
extern const char *whileLabel;
extern const char *label;

extern string mainEntryLabel;
extern string mainExitLabel;

extern string objectName;
extern string integerName;
extern string stringName;
extern string tableName;

extern bool analyzingMainBlock;
extern bool analyzingClassDec;
extern bool analyzingClassConst;
extern bool analyzingClassMeth;
extern bool analyzingLocalVarDec;

extern MainBlockNode * mainBlockAnalyzing;
extern ClassMethodNode * methodAnalyzing;
extern ClassConstructorNode * constructorAnalyzing;

extern bool err;

extern int getCurrentSourceLineNumber();

#endif
