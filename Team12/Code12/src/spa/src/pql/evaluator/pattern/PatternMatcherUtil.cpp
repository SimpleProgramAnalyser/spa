/**
 * Implementation of utility functions for pattern matching.
 */

#include "PatternMatcherUtil.h"

Void PatternMatcherTuple::addAssignStatement(Integer assignStatementNumber, const String& variable)
{
    assignStatementResults.push_back(std::to_string(assignStatementNumber));
    variableResults.insert(variable);
    relationshipsResults.emplace_back(assignStatementNumber, variable);
}

Void PatternMatcherTuple::concatTuple(const PatternMatcherTuple& pmt)
{
    this->assignStatementResults.insert(this->assignStatementResults.cend(), pmt.assignStatementResults.cbegin(),
                                        pmt.assignStatementResults.cend());
    std::copy(pmt.variableResults.begin(), pmt.variableResults.end(),
              std::inserter(this->variableResults, this->variableResults.end()));
    this->relationshipsResults.insert(this->relationshipsResults.cend(), pmt.relationshipsResults.cbegin(),
                                      pmt.relationshipsResults.cend());
}

std::vector<String> PatternMatcherTuple::getAssignStatements() const
{
    return assignStatementResults;
}

std::vector<String> PatternMatcherTuple::getVariables() const
{
    return std::vector<String>(variableResults.begin(), variableResults.end());
}

std::vector<std::pair<Integer, String>> PatternMatcherTuple::getRelationships() const
{
    return relationshipsResults;
}
