/**
 * Implementation of Next BIP Table Facade, methods for
 * NextExtractor to interact with PKB for NextBip relationships.
 */

#include "NextBipTableFacade.h"

#include "pkb/PKB.h"

Void NextBipTableFacade::addNextRelationshipBetweenNodes(StatementNode* currentNode, StatementNode* nextNode,
                                                         std::vector<Pair<Integer, Integer>>& nextRelationships)
{
    addNextBipRelationships(currentNode->getStatementNumber(), currentNode->getStatementType(),
                            nextNode->getStatementNumber(), nextNode->getStatementType());

    // For testing
    Pair<Integer, Integer> nextRelationship(currentNode->getStatementNumber(), nextNode->getStatementNumber());
    nextRelationships.push_back(nextRelationship);
}
