/**
 * Implementation of a Next Evaluator Facade
 * for the main Next operation.
 */

#include "NextEvaluatorFacade.h"

#include "pkb/PKB.h"

Vector<Integer> NextEvaluatorFacade::getNext(Integer prev, StatementType nextType)
{
    return getAllNextStatements(prev, nextType);
}

Vector<Integer> NextEvaluatorFacade::getPrevious(Integer next, StatementType prevType)
{
    return getAllPreviousStatements(next, prevType);
}

Vector<Integer> NextEvaluatorFacade::getNextMatching(StatementType prevType, StatementType nextType)
{
    return getAllNextStatementsTyped(prevType, nextType);
}

Vector<Integer> NextEvaluatorFacade::getPreviousMatching(StatementType prevType, StatementType nextType)
{
    return getAllPreviousStatementsTyped(prevType, nextType);
}

Vector<std::pair<Integer, Integer>> NextEvaluatorFacade::getNextPairs(StatementType prevType, StatementType nextType)
{
    return getAllNextTuples(prevType, nextType);
}

Vector<Integer> NextEvaluatorFacade::getStatements(StatementType type)
{
    return getAllStatements(type);
}

Boolean NextEvaluatorFacade::isNext(Integer prev, Integer next)
{
    return checkIfNextHolds(prev, next);
}
