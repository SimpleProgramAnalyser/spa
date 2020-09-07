/**
 * Implementation of Abstract Syntax Tree
 * classes and methods.
 */

#include "SemanticErrorsValidator.h"

#include "ast/AstTypes.h"

SemanticErrorsValidator::SemanticErrorsValidator(ProgramNode& progNode): programNode(progNode) {}

Boolean SemanticErrorsValidator::checkProgramValidity()
{
    List<ProcedureNode>* procedureList = &(programNode.procedureList);
    Boolean isProgramValid = true;

    for (size_t i = 0; i < procedureList->size(); i++) {
        const StmtlstNode& stmtListNode = *(procedureList->at(i))->statementListNode;

        for (size_t j = 0; j < stmtListNode.statementList.size(); j++) {
            const List<StatementNode>& stmtList = stmtListNode.statementList;
            // Terminate early
            if (!isProgramValid) {
                return false;
            }

            StatementNode* stmtNode = stmtList.at(j).get();
            isProgramValid = checkStatementValidity(stmtNode);
        }
    }
    return isProgramValid;
}

Boolean SemanticErrorsValidator::checkWhileStatementValidity(WhileStatementNode* stmtNode)
{
    const ConditionalExpression* predicate = stmtNode->predicate;
    const StmtlstNode& stmtListNode = *(stmtNode->statementList);
    Boolean isValid = true;

    if (predicate == nullptr) {
        // TODO: throw error
        return false;
    } else if (stmtListNode.statementList.size() == NULL) {
        // TODO: throw error
        return false;
    } else {
        const List<StatementNode>& stmtList = stmtListNode.statementList;
        for (size_t i = 0; i < stmtList.size(); i++) {
            StatementNode* statementNode = stmtList.at(i).get();
            isValid = checkStatementValidity(statementNode);
        }
    }
    return isValid;
}

Boolean SemanticErrorsValidator::checkAssignmentStatementValidity(AssignmentStatementNode* stmtNode)
{
    Variable variable = stmtNode->variable;
    const Expression* expression = stmtNode->expression;
    Boolean expressionIsValid = true;
    if (variable.isConstant()) {
        // Throw error
        return false;
    } else if (variable.varName == "") {
        // throw error
        return false;
    } else if (expression == nullptr) {
        // throw error
        return false;
    } else {

        expressionIsValid = checkExpressionValidity(expression);
    }
    return expressionIsValid;
}

Boolean SemanticErrorsValidator::checkIfStatementValidity(IfStatementNode* stmtNode)
{
    const ConditionalExpression* predicate = stmtNode->predicate;
    const StmtlstNode& ifStmtListNode = *(stmtNode->ifStatementList);
    const StmtlstNode& elseStmtListNode = *(stmtNode->elseStatementList);
    const List<StatementNode>& ifStmtList = ifStmtListNode.statementList;
    const List<StatementNode>& elseStmtList = elseStmtListNode.statementList;
    Boolean ifStmtlstIsValid = true;
    Boolean elseStmtlstIsValid = true;

    if (predicate == nullptr) {
        // throw error
        return false;
    }

    if (ifStmtList.empty()) {
        // throw error
        return false;
    }
    if (elseStmtList.empty()) {
        // throw error
        return false;
    }

    for (size_t i = 0; i < ifStmtList.size(); i++) {
        // Terminate early
        if (!ifStmtlstIsValid) {
            return false;
        }

        StatementNode* statementNode = ifStmtList.at(i).get();
        ifStmtlstIsValid = checkStatementValidity(statementNode);
    }

    for (size_t i = 0; i < elseStmtList.size(); i++) {
        // Terminate early
        if (!elseStmtlstIsValid) {
            return false;
        }
        StatementNode* statementNode = elseStmtList.at(i).get();
        elseStmtlstIsValid = checkStatementValidity(statementNode);
    }
    return ifStmtlstIsValid && elseStmtlstIsValid;
}

Boolean SemanticErrorsValidator::checkCallStatementValidity(CallStatementNode* stmtNode)
{
    Name procedureName = stmtNode->procedureName;

    if (procedureName == "") {
        // Throw error
        return false;
    }
    return true;
}

Boolean SemanticErrorsValidator::checkReadStatementValidity(ReadStatementNode* stmtNode)
{
    Name variable = stmtNode->var.varName;
    if (variable == "") {
        // Throw error
        return false;
    }
    return true;
}

Boolean SemanticErrorsValidator::checkPrintStatementValidity(PrintStatementNode* stmtNode)
{
    Name variable = stmtNode->var.varName;
    if (variable == "") {
        // Throw error
        return false;
    }
    return true;
}

Boolean SemanticErrorsValidator::checkStatementValidity(StatementNode* stmtNode)
{
    Boolean statementIsValid = true;
    StatementType stmtType = stmtNode->getStatementType();
    switch (stmtType) {
    case WhileStatement:
        statementIsValid = checkWhileStatementValidity(dynamic_cast<WhileStatementNode*>(stmtNode));
        break;
    case AssignmentStatement:
        statementIsValid = checkAssignmentStatementValidity(dynamic_cast<AssignmentStatementNode*>(stmtNode));
        break;
    case CallStatement:
        statementIsValid = checkCallStatementValidity(dynamic_cast<CallStatementNode*>(stmtNode));
        break;
    case IfStatement:
        statementIsValid = checkIfStatementValidity(dynamic_cast<IfStatementNode*>(stmtNode));
        break;
    case ReadStatement:
        statementIsValid = checkReadStatementValidity(dynamic_cast<ReadStatementNode*>(stmtNode));
        break;
    case PrintStatement:
        statementIsValid = checkPrintStatementValidity(dynamic_cast<PrintStatementNode*>(stmtNode));
        break;
    // Should not reach this case
    default:
        statementIsValid = false;
        break;
    }
    return statementIsValid;
}

Boolean SemanticErrorsValidator::checkExpressionValidity(const Expression* expression)
{
    Boolean operandIsValid = true;
    Boolean leftChildIsValid = true;
    Boolean rightChildIsValid = true;

    // Expression is a ReferenceExpression & has either a Constant or a Variable
    if (!(expression->isArithmetic())) {
        const BasicDataType* data = dynamic_cast<const ReferenceExpression*>(expression)->basicData;
        if (data == nullptr) {
            // TODO: throw error
            return false;
        }
        if (data->isConstant()) {
            const Constant* constantObj = dynamic_cast<const Constant*>(data);
            if (constantObj->value == NULL) {
                // TODO: throw error
                return false;
            }
        } else {
            const Variable* variableObj = dynamic_cast<const Variable*>(data);
            if (variableObj->varName == "") {
                // TODO: throw error
                return false;
            }
        }
        return true;
    }

    // Expression is Arithmetic
    const ArithmeticExpression* arithmeticExp = dynamic_cast<const ArithmeticExpression*>(expression);
    // Terminate early
    if (arithmeticExp->opr == NULL) {
        // TODO: throw error for no operand
        operandIsValid = false;
        return false;
    }

    // If left child is Arithmetic, check validity
    const Expression* leftExp = arithmeticExp->leftFactor;
    leftChildIsValid = checkExpressionValidity(leftExp);
    // Terminate early
    if (!leftChildIsValid) {
        return false;
    }

    // If right child is Arithmetic, check validity
    const Expression* rightExp = arithmeticExp->rightFactor;
    rightChildIsValid = checkExpressionValidity(rightExp);

    return operandIsValid && leftChildIsValid && rightChildIsValid;
}
