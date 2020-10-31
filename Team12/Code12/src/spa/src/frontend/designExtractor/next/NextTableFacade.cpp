/**
 * Implementation of Next Table Facade, methods for
 * NextExtractor to interact with PKB for Next relationships.
 */

#include "NextTableFacade.h"

#include "pkb/PKB.h"

Void NextTableFacade::addNextRelationshipBetweenNodes(StatementNode* currentNode, StatementNode* nextNode,
                                                      std::vector<Pair<Integer, Integer>>& nextRelationships)
{
    addNextRelationships(currentNode->getStatementNumber(), currentNode->getStatementType(),
                         nextNode->getStatementNumber(), nextNode->getStatementType());

    // For testing
    Pair<Integer, Integer> nextRelationship(currentNode->getStatementNumber(), nextNode->getStatementNumber());
    nextRelationships.push_back(nextRelationship);
}
