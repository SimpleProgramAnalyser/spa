/**
 * Implementation of common methods used by the Query Evaluator.
 */
#include "EvaluatorUtils.h"

#include <iterator>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>

ClauseResult convertToClauseResult(const Vector<Integer>& intList)
{
    ClauseResult strList;
    for (Integer i : intList) {
        strList.push_back(std::to_string(i));
    }
    return strList;
}

template<> struct std::hash<DesignEntityType> {
    std::size_t operator()(const DesignEntityType& pv) const
    {
        // NOLINTNEXTLINE
        return std::hash<char>()(static_cast<const char&>(pv));
    }
};

std::unordered_map<DesignEntityType, StatementType> getStatementTypesMap()
{
    std::unordered_map<DesignEntityType, StatementType> queryPkbTypesMap
        = {{StmtType, AnyStatement},         {ReadType, ReadStatement},   {PrintType, PrintStatement},
           {CallType, CallStatement},        {WhileType, WhileStatement}, {IfType, IfStatement},
           {AssignType, AssignmentStatement}};
    return queryPkbTypesMap;
}

StatementType mapToStatementType(DesignEntityType entType)
{
    std::unordered_map<DesignEntityType, StatementType> queryPkbTypesMap = getStatementTypesMap();
    if (queryPkbTypesMap.find(entType) != queryPkbTypesMap.end()) {
        return queryPkbTypesMap[entType];
    } else {
        throw std::runtime_error("Wrong entity type in mapToStatementType");
    }
}

ClauseResult removeDuplicates(const ClauseResult& list)
{
    std::unordered_set<String> resultsFromList;
    std::copy(list.begin(), list.end(), std::inserter(resultsFromList, resultsFromList.end()));
    return std::vector<String>(resultsFromList.begin(), resultsFromList.end());
}
