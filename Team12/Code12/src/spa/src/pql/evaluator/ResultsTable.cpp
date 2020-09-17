/**
 * Implementation of a results table, to hold
 * the results of evaluating each clause in
 * a SIMPLE program query.
 */

#include "ResultsTable.h"

#include <utility>

#include "pkb/PKB.h"

ResultsTable::ResultsTable(const DeclarationTable& decls): declarations(decls) {}

/**
 * Checks if a synonym exists in the results table.
 *
 * @param syn The synonym to be checked.
 * @return True, if the synonym exists in the table.
 *         Otherwise, false.
 */
inline Boolean ResultsTable::checkIfSynonymInTable(const Synonym& syn)
{
    return resultsMap.find(syn) != resultsMap.end();
}

void ResultsTable::filterTable(const Synonym& syn, const ClauseResult& results)
{
    if (checkIfSynonymInTable(syn)) {
        resultsMap[syn] = findCommonElements(results, resultsMap[syn]);
    } else {
        // synonym is not found in table, associate results with synonym
        resultsMap.insert(std::make_pair(syn, results));
    }
}

ClauseResult ResultsTable::get(const Synonym& syn)
{
    if (checkIfSynonymInTable(syn)) {
        return resultsMap[syn];
    } else {
        return retrieveAllMatching(getTypeOfSynonym(syn));
    }
}

DesignEntityType ResultsTable::getTypeOfSynonym(const Synonym& syn)
{
    return declarations.getDesignEntityOfSynonym(syn).getType();
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

ClauseResult retrieveAllMatching(DesignEntityType entTypeOfSynonym)
{
    ClauseResult results;
    if (isStatementDesignEntity(entTypeOfSynonym)) {
        results = convertToClauseResult(getAllStatements(mapToStatementType(entTypeOfSynonym)));
    } else if (entTypeOfSynonym == VariableType) {
        results = getAllVariables();
    } else if (entTypeOfSynonym == ProcedureType) {
        results = getAllProcedures();
    } else if (entTypeOfSynonym == ConstantType) {
        results = convertToClauseResult(getAllConstants());
    } else {
        throw std::runtime_error("Unknown DesignEntityType in retrieveAllMatching");
    }
    return results;
}
