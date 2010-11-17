// Niels Widger
// Time-stamp: <30 Nov 2009 at 19:23:28 by nwidger on macros.local>

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <iostream>
using namespace std;

#include "ClassTable.h"
#include "Type.h"
#include "TypeModule.h"

TypeModule::TypeModule() {
	cTable = new ClassTable();
	nullType = new NullType();
}

TypeModule::~TypeModule() {
	delete cTable;
	delete nullType;
}

ClassTable * TypeModule::classTable() {
	return cTable;
}

NullType * TypeModule::getNullType() {
	return nullType;
}
