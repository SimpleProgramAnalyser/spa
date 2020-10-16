/**
 * Implementation of Next extractor.
 */
#include <stdexcept>
#include <unordered_set>

#include "CallsExtractor.h"
#include "pkb/PKB.h"

/**
* Gets the procedure name with the index provided from the list of procedure nodes.
* 
* @param procList The list of procedurenodes
* @param index The index of the procedure
* @return The name of the procedure of the provided index in the procList
*/
ProcedureName getProcedureNameWithIndex(const ProcedureNodeList* procList, Integer index)
{
    return procList->at(index).get()->procedureName;
}
    
/**
* Helper method to extract Calls* relationship.
* 
* @param procList The list of procedure of the whole program
* @param adjacencyMatrix The adjacenctMatrix of Calls SEV
* @param isMemo The vector of boolean to check if the procedures that a procedure calls is memoized
* @param memo The adjacency list for memoization of the procedures and the procedures they call
* @param rowIndex The index of the current procedure, or rowIndex of the adjacencyMatrix
* @return The unordered set of the procedures that the current procedure calls
*/
std::unordered_set<size_t> extractCallsStarUtil(const ProcedureNodeList* procList, Matrix* adjacencyMatrix,
                                                Vector<Boolean>* isMemo,
                                                std::vector<std::pair<size_t, std::unordered_set<size_t>>>* memo,
                                                size_t rowIndex)
{
    size_t sizeOfProcList = procList->size();
    std::unordered_set<size_t> currentSet;
    
    // If the current procedure of index rowIndex, we return an empty set
    // Else, we go loop though the columns of this current row for this 
    // procedure (adjacencyMatrix)
    if (!isMemo->at(rowIndex)) {
        for (size_t j = 0; j< sizeOfProcList; j++) {
            // If a column is true, the procedure with the index j of the 
            // column is called by the current procedure 
            bool isCalled = adjacencyMatrix->at(rowIndex).at(j);

            if (isCalled) {
                // If the called procedure is memoized, we get the already computed results and insert 
                // them into the current set of possible Calls* Relationships. This is possible because 
                // the current procedure will also have Calls* Relationships with the procedures that 
                // the called procedure have Calls* relationships with. 
                if (isMemo->at(j)) {
                    std::unordered_set<size_t> memoResult = memo->at(j).second;
                    currentSet.insert(memoResult.begin(), memoResult.end());
                } else {
                    // Else, we recurse with the called procedure
                    // The returned set will be the procedures that have Calls* relatiomships with 
                    // the current procedure
                    std::unordered_set<size_t> mySet = extractCallsStarUtil(procList, adjacencyMatrix, isMemo, memo, j);
                    currentSet.insert(mySet.begin(), mySet.end());
                }
            }
        }
        // We mark the current procedure as memoized
        isMemo->at(rowIndex) = true;
        // Then we insert the current set into the memoization adjacency list
        memo->at(rowIndex).second.insert(currentSet.begin(), currentSet.end());
        // We add the current index into current set so that the returned set 
        // will include the current procedure
        currentSet.insert({rowIndex});
    }
    return currentSet;
}

/**
 * Extracts the Calls* relationship.
 *
 * @param procList The list of procedure of the whole program
 * @param adjacencyMatrix The adjacenctMatrix of Calls from SEV
 * @return The memoized adjacency list that stores the Calls* relationships. Solely for testing purposes.
 */ 
std::vector<std::pair<size_t, std::unordered_set<size_t>>> extractCallsStar(const ProcedureNodeList* procList,
                                                                        Matrix adjacencyMatrix)
{
    size_t sizeOfProcList = procList->size();
    Vector<Boolean> isMemo;
    std::vector<std::pair<size_t, std::unordered_set<size_t>>> memo;

    // Initialise isMemo and memo
    for (size_t i = 0; i < sizeOfProcList; i++) {
        isMemo.push_back(false);

        std::unordered_set<size_t> newSet;
        memo.push_back(std::make_pair(i, newSet));

    }

    // Extract Calls* relationship
    for (size_t i = 0; i < sizeOfProcList; i++) {
        extractCallsStarUtil(procList, &adjacencyMatrix, &isMemo, &memo, i);
    }

    // Iterate through memo
    for (size_t j = 0; j < sizeOfProcList; j++) {
        // Procedure name of current procedure
        ProcedureName currentProcName = getProcedureNameWithIndex(procList, j);
        // Get the set of procedures mapped to the current procedure
         std::unordered_set<size_t> callStarProcedureSet = memo.at(j).second;
        // Add caller relationship star for the set of procedures
        for (auto k = callStarProcedureSet.begin(); k != callStarProcedureSet.end(); k++) {
            addCallerRelationshipsStar(currentProcName, getProcedureNameWithIndex(procList, *k));
        }
    }
    return memo;
}


/**
 * Extract Calls relationship from the AST and the Call graph from the SemanticErrorsValidator.
 * 
 * @param rootNode The root node of the AST of the current program
 * @param adjacencyMatrix The matrix of procedure calls in the program
 * @return A vector of pairs of strings which represents calls to a procedure from another procedure. 
 *         Solely for testing purposes.
 */                                                                     
std::vector<std::pair<String, String>> extractCalls(const ProgramNode& rootNode,
                                                                              Matrix adjacencyMatrix)
{
    // For testing
    std::vector<std::pair<String, String>> callsRelationships;

    const List<ProcedureNode>* procList = &(rootNode.procedureList);
    size_t sizeOfProcList = procList->size();

    for (size_t i = 0; i < sizeOfProcList; i++) {
        ProcedureName currentProcName = getProcedureNameWithIndex(procList, i);
        for (size_t j = 0; j < sizeOfProcList; j++) {
            bool isCalled = adjacencyMatrix.at(i).at(j);
            if (isCalled) {
                ProcedureName calleeProcName = getProcedureNameWithIndex(procList, j);
                // Add Calls relationship into the PKB
                addCallerRelationships(currentProcName, calleeProcName);

                // For testing
                Pair<String, String> callsRelationship(currentProcName, calleeProcName);
                callsRelationships.push_back(callsRelationship);

            }
        }
    }

    // Extract Call Star relationships
    extractCallsStar(procList, adjacencyMatrix);
    
    // For testing
    return callsRelationships;
}