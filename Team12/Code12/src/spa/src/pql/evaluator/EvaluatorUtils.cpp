/**
 * Implementation of common methods used by the Query Evaluator.
 */
#include "EvaluatorUtils.h"

#include <unordered_set>

ClauseResult convertToClauseResult(const Vector<Integer>& intList)
{
    ClauseResult strList;
    for (Integer i : intList) {
        strList.push_back(std::to_string(i));
    }
    return strList;
}

StatementType mapToStatementType(DesignEntityType entType)
{
    StatementType mappedStmtType;
    // TODO: Use a hash table instead
    if (entType == StmtType) {
        mappedStmtType = AnyStatement;
    } else if (entType == ReadType) {
        mappedStmtType = ReadStatement;
    } else if (entType == PrintType) {
        mappedStmtType = PrintStatement;
    } else if (entType == CallType) {
        mappedStmtType = CallStatement;
    } else if (entType == WhileType) {
        mappedStmtType = WhileStatement;
    } else if (entType == IfType) {
        mappedStmtType = IfStatement;
    } else if (entType == AssignType) {
        mappedStmtType = AssignmentStatement;
    } else {
        throw std::runtime_error("Wrong entity type in mapToStatementType");
    }

    return mappedStmtType;
}

ClauseResult removeDuplicates(const ClauseResult& list)
{
    std::unordered_set<String> resultsFromList;
    std::copy(list.begin(), list.end(), std::inserter(resultsFromList, resultsFromList.end()));
    return std::vector<String>(resultsFromList.begin(), resultsFromList.end());
}
