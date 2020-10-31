/**
 * Identifies Next relationships.
 */

#ifndef SPA_FRONTEND_NEXT_EXTRACTOR_H
#define SPA_FRONTEND_NEXT_EXTRACTOR_H

#include <unordered_set>

#include "NextTableFacade.h"
#include "ast/AstTypes.h"
#include "cfg/CfgTypes.h"

class NextExtractor {
private:
    /**
     * Root Control Flow Graph node of the current
     * procedure that NextExtractor is working on.
     */
    const CfgNode* node;
    /**
     * Vector of pairs of integers to represent Next
     * Relationships. Solely for testing purposes.
     */
    Vector<Pair<Integer, Integer>> nextRelationships;
    /**
     * Array of booleans to indicate if a node has been
     * visited by the NextExtractor.
     */
    Vector<Boolean> visitedArray;

    /**
     * The facade which this extractor uses to interact
     * and store Next relationships in the PKB.
     */
    std::unique_ptr<NextTableFacade> facade;

    /**
     * Extracts Next Relationships between statement nodes with in a CFG node and with
     * the statement accessed before accessing the current CFG node.
     *
     * @param cfgNode The current CFG node
     * @param prevStmtNode The most recent statement before accessing this CFG node
     */
    Void extractNextFromNode(const CfgNode* cfgNode, StatementNode* prevStmtNode);

public:
    /**
     * Constructor for a NextExtractor. Memory management
     * of the NextTableFacade pointer will be handled by
     * the constructed NextExtractor when it is deleted.
     *
     * @param procedureNode The starting Control Flow Graph
     *                      node for a procedure.
     * @param numberOfNodes Number of nodes in the Control
     *                      Flow Graph given.
     * @param facade Pointer to a NextTableFacade to be
     *               used by this NextExtractor to interact
     *               with the Program Knowledge Base.
     */
    NextExtractor(const CfgNode* procedureNode, size_t numberOfNodes, NextTableFacade* facade);

    /**
     * Extracts Next relationships and stores the result in the PKB.
     * Returns a vector of pair of integers with all Next relationships,
     * for use in unit testing.
     *
     * @return Vector of Next relationships.
     */
    Vector<Pair<Integer, Integer>> extractNext();
};

/**
 * Extracts the Next relationships from the current program, represented with a CFG.
 * We can access the whole CFG with just its root node
 *
 * @param cfgInfo A pair container the pointer to the CFG root node and the number
 *        of CFG nodes it has
 * @return A vector of pairs of Integers that represents all the next relationships.
 *         Solely for testing purposes.
 */
Vector<Pair<Integer, Integer>> extractNext(std::pair<CfgNode*, size_t> cfgInfo);

#endif // SPA_FRONTEND_NEXT_EXTRACTOR_H
