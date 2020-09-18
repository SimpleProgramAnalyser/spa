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

ClauseResult findCommonElements(const ClauseResult& firstList, const ClauseResult& secondList)
{
    // initiate set of elements from first list
    std::unordered_set<String> resultsFromFirst;
    std::copy(firstList.begin(), firstList.end(), std::inserter(resultsFromFirst, resultsFromFirst.end()));
    // initiate set to contain elements found in both
    std::unordered_set<String> resultsFoundInBoth;
    // loop through elements in second
    for (const String& str : secondList) {
        if (resultsFromFirst.find(str) == resultsFromFirst.end()) {
            // element from secondList is not in firstList
        } else {
            // element is found in firstList!
            resultsFoundInBoth.insert(str);
        }
    }
    return std::vector<String>(resultsFoundInBoth.begin(), resultsFoundInBoth.end());
}

ClauseResult removeDuplicates(const ClauseResult& list)
{
    std::unordered_set<String> resultsFromList;
    std::copy(list.begin(), list.end(), std::inserter(resultsFromList, resultsFromList.end()));
    return std::vector<String>(resultsFromList.begin(), resultsFromList.end());
}
