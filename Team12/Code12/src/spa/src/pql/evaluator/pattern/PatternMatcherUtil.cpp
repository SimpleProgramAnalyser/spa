/**
 * Implementation of utility functions for pattern matching.
 */

#include "PatternMatcherUtil.h"

#include <iterator>

Void PatternMatcherTuple::addTargetStatement(Integer targetStatementNumber) {
    targetStatementResults.push_back(std::to_string(targetStatementNumber));
}

Void PatternMatcherTuple::addTargetStatement(Integer targetStatementNumber, const String& variable)
{
    targetStatementResults.push_back(std::to_string(targetStatementNumber));
    variableResults.insert(variable);
    relationshipsResults.emplace_back(targetStatementNumber, variable);
}

Void PatternMatcherTuple::addTargetStatement(Integer targetStatementNumber, const std::unordered_set<String>& variables)
{
    targetStatementResults.push_back(std::to_string(targetStatementNumber));

    for (auto variable : variables) {
        variableResults.insert(variable);
        relationshipsResults.emplace_back(targetStatementNumber, variable);
    }
}

Void PatternMatcherTuple::concatTuple(const PatternMatcherTuple& pmt)
{
    this->targetStatementResults.insert(this->targetStatementResults.cend(), pmt.targetStatementResults.cbegin(),
                                        pmt.targetStatementResults.cend());
    std::copy(pmt.variableResults.begin(), pmt.variableResults.end(),
              std::inserter(this->variableResults, this->variableResults.end()));
    this->relationshipsResults.insert(this->relationshipsResults.cend(), pmt.relationshipsResults.cbegin(),
                                      pmt.relationshipsResults.cend());
}

std::vector<String> PatternMatcherTuple::getTargetStatements() const
{
    return targetStatementResults;
}

std::vector<String> PatternMatcherTuple::getVariables() const
{
    return std::vector<String>(variableResults.begin(), variableResults.end());
}

std::vector<std::pair<Integer, String>> PatternMatcherTuple::getRelationships() const
{
    return relationshipsResults;
}

Void getVariableInExpr(const Expression* expr, std::unordered_set<String>& variables)
{
    if (expr->isArithmetic()) {
        auto arithmeticExpr = static_cast<const ArithmeticExpression*>(expr);
        const Expression* leftExpr = arithmeticExpr->leftFactor;
        const Expression* rightExpr = arithmeticExpr->rightFactor;

        getVariableInExpr(leftExpr, variables);
        getVariableInExpr(rightExpr, variables);
    } else {
        // Reference Expression
        auto refExpr = static_cast<const ReferenceExpression*>(expr);
        const BasicDataType* basicDataType = refExpr->basicData;
        if (basicDataType->isConstant()) {
            return;
        }

        const auto* variable = static_cast<const Variable*>(basicDataType);
        variables.insert(variable->varName);
    }
}

Void getLiteralVariablesInCondExpr(const ConditionalExpression* condExpr, std::unordered_set<String>& variables)
{
    auto conditionType = condExpr->getConditionalType();
    switch (conditionType) {
        case NotConditionalExpression: {
            auto notCondExpr = static_cast<const NotExpression*>(condExpr);
            const ConditionalExpression* negatedCondExpression = notCondExpr->expression;

            getLiteralVariablesInCondExpr(negatedCondExpression, variables);
            break;
        }
        case OrConditionalExpression: {
            auto orCondExpr = static_cast<const OrExpression*>(condExpr);
            const ConditionalExpression* leftCondExpr = orCondExpr->leftExpression;
            const ConditionalExpression* rightCondExpr = orCondExpr->rightExpression;

            getLiteralVariablesInCondExpr(leftCondExpr, variables);
            getLiteralVariablesInCondExpr(rightCondExpr, variables);
            break;
        }
        case AndConditionalExpression: {
            auto andCondExpr = static_cast<const AndExpression*>(condExpr);
            const ConditionalExpression* leftCondExpr = andCondExpr->leftExpression;
            const ConditionalExpression* rightCondExpr = andCondExpr->rightExpression;

            getLiteralVariablesInCondExpr(leftCondExpr, variables);
            getLiteralVariablesInCondExpr(rightCondExpr, variables);
            break;
        }
        case RelationalConditionalExpression: {
            auto relCondExpr = static_cast<const RelationalExpression*>(condExpr);
            const Expression* leftExpr = relCondExpr->leftFactor;
            const Expression* rightExpr = relCondExpr->rightFactor;

            getVariableInExpr(leftExpr, variables);
            getVariableInExpr(rightExpr, variables);
            break;
        }
        default:
            throw std::runtime_error("Unknown or invalid expression type in getLiteralVariablesInCondExpr");
    }
}