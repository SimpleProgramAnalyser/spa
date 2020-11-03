/**
 * This class contains methods for Next Extractor to
 * store relationships into the normal Next table.
 */

#ifndef SPA_FRONTEND_NEXT_TABLE_FACADE_H
#define SPA_FRONTEND_NEXT_TABLE_FACADE_H

#include "Types.h"
#include "ast/AstTypes.h"

class NextTableFacade {
public:
    NextTableFacade() = default;
    NextTableFacade(const NextTableFacade&) = default;
    NextTableFacade(NextTableFacade&&) = default;
    virtual ~NextTableFacade() = default;
    NextTableFacade& operator=(const NextTableFacade&) = default;
    NextTableFacade& operator=(NextTableFacade&&) = default;

    /**
     * Calls the PKB API to add a Next relationship between two statement nodes. Also updates
     * the nextRelationships vector to represent a next relationship for testing.
     *
     * @param currentNode The previous node in the Next relationship
     * @param previousNode The next node in the Next relationship
     * @param nextRelationships Vector of pairs of integers to represent Next
     *        Relationships. Solely for testing purposes
     */
    virtual Void addNextRelationshipBetweenNodes(StatementNode* currentNode, StatementNode* nextNode,
                                                 std::vector<Pair<Integer, Integer>>& nextRelationships);
};

#endif // SPA_FRONTEND_NEXT_TABLE_FACADE_H
