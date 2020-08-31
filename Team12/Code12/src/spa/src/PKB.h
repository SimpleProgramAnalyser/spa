#pragma once

#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>

using namespace std;
typedef short PROC;

class TNode;

class VarTable; // no need to #include "VarTable.h" as all I need is pointer

class PKB {
    public:
    static VarTable* varTable;
    static int setProcToAST(PROC p, TNode* r);
    static TNode* getRootAST(PROC p);
};