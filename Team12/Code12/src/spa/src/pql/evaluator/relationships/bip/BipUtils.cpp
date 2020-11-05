/**
 * Implementation of utility methods for BIP evaluation.
 */

#include "BipUtils.h"

#include <iterator>

StatementPositionInCfg::StatementPositionInCfg(CfgNode* node, Integer index): nodePosition(node), statementIndex(index)
{}

StatementPositionInCfg::StatementPositionInCfg(Pair<CfgNode*, Integer> positionPair):
    nodePosition(positionPair.first), statementIndex(positionPair.second)
{}

CfgNode* StatementPositionInCfg::getNodePosition() const
{
    return nodePosition;
}

Integer StatementPositionInCfg::getStatementIndex() const
{
    return statementIndex;
}

Integer StatementPositionInCfg::getStatementNumber() const
{
    return nodePosition->statementNodes->at(statementIndex)->getStatementNumber();
}

bool StatementPositionInCfg::operator==(const StatementPositionInCfg& sp) const
{
    return this->nodePosition == sp.nodePosition && this->statementIndex == sp.statementIndex;
}

std::size_t StatementPositionHasher::operator()(const StatementPositionInCfg& sp) const
{
    std::size_t hashedPointer = std::hash<CfgNode*>()(sp.nodePosition);
    return (hashedPointer
            ^ (std::hash<Integer>()(sp.statementIndex) + uint32_t(2654435769) + (hashedPointer * 64)
               + (hashedPointer / 4)));
}

Vector<StatementPositionInCfg>
findCorrespondingNodes(StatementPositionInCfg startingPosition, Integer statementToFind,
                       std::unordered_set<StatementPositionInCfg, StatementPositionHasher>& visitedCfgPositions)
{
    CfgNode* startingNode = startingPosition.getNodePosition();
    Integer startingIndex = startingPosition.getStatementIndex();
    std::unordered_set<StatementPositionInCfg, StatementPositionHasher> matchingPositions;

    // if visited before, we will not find the statementToFind here
    if (visitedCfgPositions.find(startingPosition) != visitedCfgPositions.end()) {
        return Vector<StatementPositionInCfg>();
    }
    visitedCfgPositions.insert(startingPosition);

    // first, try to search the untraversed statements
    Integer maxLength = startingNode->statementNodes->size();
    for (Integer i = startingIndex + 1; i < maxLength; i++) {
        if (startingNode->statementNodes->at(i)->getStatementNumber() == statementToFind) {
            // for this branch, we found the first occurrence so we terminate
            return Vector<StatementPositionInCfg>({{startingNode, i}});
        } else {
            visitedCfgPositions.insert({startingNode, i});
        }
    }

    // else, search the children recursively
    for (CfgNode* child : *startingNode->childrenNodes) {
        Vector<StatementPositionInCfg> resultsFromChild
            = findCorrespondingNodes({child, -1}, statementToFind, visitedCfgPositions);
        if (!resultsFromChild.empty()) {
            // we use a set to handle branching paths that both find the same
            // first occurrence after the paths have joined back with each other
            std::copy(resultsFromChild.begin(), resultsFromChild.end(),
                      std::inserter(matchingPositions, matchingPositions.begin()));
        }
    }

    // we searched all paths and now we can return the positions
    return Vector<StatementPositionInCfg>(matchingPositions.begin(), matchingPositions.end());
}

Vector<StatementPositionInCfg> findAllCorrespondingPositions(Integer statementToFind, BipFacade& bipFacade)
{
    // first, get all procedures that possibly have CFG BIPs with statement
    Vector<String> directContainerProcedures = bipFacade.getProcedure(statementToFind);
    std::unordered_set<String> indirectContainerProcedures;
    for (const String& procedure : directContainerProcedures) {
        for (const String& indirectProcedures : bipFacade.getCallersStar(procedure)) {
            indirectContainerProcedures.insert(indirectProcedures);
        }
        indirectContainerProcedures.insert(procedure);
    }

    // next, find the matching positions of those CFG BIPs
    std::unordered_set<StatementPositionInCfg, StatementPositionHasher> matchingPositions;
    for (const String& procedure : indirectContainerProcedures) {
        CfgNode* startingNode = bipFacade.getCfg(procedure);
        if (startingNode == nullptr) {
            continue;
        }
        std::unordered_set<StatementPositionInCfg, StatementPositionHasher> uniqueStatementsVisited;
        Vector<StatementPositionInCfg> positionsInProcedure
            = findCorrespondingNodes({startingNode, -1}, statementToFind, uniqueStatementsVisited);
        if (!positionsInProcedure.empty()) {
            std::copy(positionsInProcedure.begin(), positionsInProcedure.end(),
                      std::inserter(matchingPositions, matchingPositions.begin()));
        }
    }
    return Vector<StatementPositionInCfg>(matchingPositions.begin(), matchingPositions.end());
}
