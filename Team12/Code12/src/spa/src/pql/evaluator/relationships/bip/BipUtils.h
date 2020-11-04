/**
 * Utility classes and methods for evaluation
 * of BIP clauses with branching into procedures.
 */

#ifndef SPA_PQL_BIP_UTILS_H
#define SPA_PQL_BIP_UTILS_H

#include "BipFacade.h"
#include "cfg/CfgTypes.h"

// Helper class to represent the exact manifestation of
// a statement number within the CFG BIP. There can be
// multiple statement positions for one statement, if
// the CFG BIP is duplicated by multiple calls.
class StatementPositionInCfg {
private:
    CfgNode* nodePosition;
    std::size_t statementIndex;

    friend class StatementPositionHasher;

public:
    /**
     * Constructor for a StatementPosition representing
     * the exact position of a statement instance in the
     * Control Flow Graph with branching into procedures.
     *
     * @param node Pointer to the CFG BIP node. If this
     *             is nullptr, it may indicate that the
     *             StatementPosition is invalid.
     * @param index Index of the statement within node.
     *              If this is negative, it may indicate
     *              that the StatementPosition is invalid.
     */
    StatementPositionInCfg(CfgNode* node, Integer index);

    /**
     * Constructor for a StatementPosition with a pair of
     * the node location and the statement index.
     */
    explicit StatementPositionInCfg(Pair<CfgNode*, Integer> positionPair);

    bool operator==(const StatementPositionInCfg& sp) const;

    CfgNode* getNodePosition() const;
    Integer getStatementIndex() const;
    Integer getStatementNumber() const;
};

// Hash function for a StatementPosition.
class StatementPositionHasher {
public:
    std::size_t operator()(const StatementPositionInCfg& sp) const;
};

/**
 * Tries to find the statement with number specified
 * in the CFG BIP, given a pointer to the starting
 * node where the traversal is to begin from, and
 * the index of the statement in the node to begin.
 *
 * Ignores the statement specified at startingIndex,
 * so the method works for self-influencing statements.
 * To search all statements in the CfgNode, use a
 * value of -1 for startingIndex.
 *
 * @param startingPosition Starting node of the CFG BIP, with
 *                         the index to begin searching the AST
 *                         statement nodes of the CFG starting node.
 * @param statement The statement to be found.
 * @param visitedCfgPositions The set of visitedCfgPositions, to prevent
 *                            infinite recursion over a while loop.
 *
 * @return Pair of pointer to the CFG node with the index
 *         of the statement in the CFG, if found.
 *         Otherwise, if not found, nullptr and -1.
 *         The pair is represented as a StatementPosition.
 */
StatementPositionInCfg
findCorrespondingNode(StatementPositionInCfg startingPosition, Integer statementToFind,
                      std::unordered_set<StatementPositionInCfg, StatementPositionHasher>& visitedCfgPositions);

/**
 * Given a statement number, find all positions of that
 * statement in the CFG BIP that exist.
 *
 * @param statementToFind The statement to find.
 * @param bipFacade The BIP facade to access the PKB with.
 * @return Every position of that statement in the CFG BIP.
 */
Vector<StatementPositionInCfg> findAllCorrespondingPositions(Integer statementToFind, BipFacade& bipFacade);

#endif // SPA_PQL_BIP_UTILS_H
