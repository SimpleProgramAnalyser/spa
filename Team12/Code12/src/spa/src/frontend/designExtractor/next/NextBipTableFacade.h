/**
 * This class contains methods for Next Extractor to
 * store relationships into the Next BIP table for
 * NextBip relationships with branching into procedures
 * from the Control Flow Graph with Branching Into
 * Procedures (CFG BIP).
 */

#ifndef SPA_FRONTEND_NEXT_BIP_TABLE_FACADE_H
#define SPA_FRONTEND_NEXT_BIP_TABLE_FACADE_H

#include "NextTableFacade.h"

class NextBipTableFacade: public NextTableFacade {
public:
    NextBipTableFacade() = default;
    NextBipTableFacade(const NextBipTableFacade&) = default;
    NextBipTableFacade(NextBipTableFacade&&) = default;
    ~NextBipTableFacade() override = default;
    NextBipTableFacade& operator=(const NextBipTableFacade&) = default;
    NextBipTableFacade& operator=(NextBipTableFacade&&) = default;

    /**
     * Calls the Next BIP API to add a NextBip relationship between two statement
     * nodes. Also updates the nextRelationships vector to represent
     * a NextBip relationship for testing.
     *
     * @param currentNode The previous node in the Next relationship
     * @param previousNode The next node in the Next relationship
     * @param nextRelationships Vector of pairs of integers to represent NextBip
     *        Relationships. Solely for testing purposes
     */
    Void addNextRelationshipBetweenNodes(StatementNode* currentNode, StatementNode* nextNode,
                                         std::vector<Pair<Integer, Integer>>& nextRelationships) override;
};

#endif // SPA_FRONTEND_NEXT_BIP_TABLE_FACADE_H
