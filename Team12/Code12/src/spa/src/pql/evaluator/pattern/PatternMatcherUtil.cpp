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
