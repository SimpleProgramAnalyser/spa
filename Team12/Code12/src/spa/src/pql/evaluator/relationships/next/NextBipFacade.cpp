/**
 * Implementation of a Next BIP Facade for the NextBip
 * operation that involves branching into other procedures.
 */

#include "NextBipFacade.h"

#include "pkb/PKB.h"

Vector<Integer> NextBipFacade::getNext(Integer prev, StatementType nextType)
{
    return getAllNextBipStatements(prev, nextType);
}

Vector<Integer> NextBipFacade::getPrevious(Integer next, StatementType prevType)
{
    return getAllPreviousBipStatements(next, prevType);
}

Vector<Integer> NextBipFacade::getNextMatching(StatementType prevType, StatementType nextType)
{
    return getAllNextBipStatementsTyped(prevType, nextType);
}

Vector<Integer> NextBipFacade::getPreviousMatching(StatementType prevType, StatementType nextType)
{
    return getAllPreviousBipStatementsTyped(prevType, nextType);
}

Vector<std::pair<Integer, Integer>> NextBipFacade::getNextPairs(StatementType prevType, StatementType nextType)
{
    return getAllNextBipTuples(prevType, nextType);
}

Boolean NextBipFacade::isNext(Integer prev, Integer next)
{
    return checkIfNextBipHolds(prev, next);
}
