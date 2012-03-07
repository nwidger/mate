// Niels Widger
// Time-stamp: <06 Mar 2012 at 18:50:38 by nwidger on macros.local>

#ifndef _GLOBALS_H
#define _GLOBALS_H

#include <string>

class ClassConstructorNode;
class ClassMethodNode;
class LabelStack;
class LocalVariableStack;
class MainBlockNode;
class MonitorStack;
class Seq;
class StringPool;
class TypeModule;

extern StringPool *stringPool;
extern TypeModule *types;
extern LocalVariableStack *localVariableStack;
extern LabelStack *labelStack;
extern MonitorStack *monitorStack;
extern Seq *ast;

extern int labelCounter;
extern int predefinedLines;
extern int synchronizedCounter;
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
