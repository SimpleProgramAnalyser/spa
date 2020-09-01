/**
 * Class structure of Abstract Syntax Tree,
 * types, fields and methods.
 */

#ifndef SPA_AST_TYPES_H
#define SPA_AST_TYPES_H

#include <Types.h>

typedef String Name;

enum StatementType {
    AnyStatement = 0,
    AssignmentStatement = 1,
    CallStatement = 2,
    IfStatement = 4,
    PrintStatement = 8,
    ReadStatement = 16,
    WhileStatement = 32
};

class StatementNode {
public:
    virtual ~StatementNode() = 0;
    virtual StatementType getStatementType() = 0;
    virtual Integer getStatementNumber() = 0;
};

class StatementListNode {
public:
    List<StatementNode> statementList;
};

class ProcedureNode {
public:
    Name procName;
    StatementListNode statementList;
};

class ProgramNode {
public:
    Name progName;
    List<ProcedureNode> procedureList;
};

class BasicDataType {
public:
    virtual ~BasicDataType() = 0;
    virtual String toString() = 0;
};

class Constant: public BasicDataType {
public:
    Integer value;
    String toString() override;
};

class Variable: public BasicDataType {
public:
    Name varName;
    String toString() override;
};

#endif // SPA_AST_TYPES_H
