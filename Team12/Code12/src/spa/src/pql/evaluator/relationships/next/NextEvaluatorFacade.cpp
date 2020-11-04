/**
 * Implementation of a Next Evaluator Facade
 * for the main Next operation.
 */

#include "NextEvaluatorFacade.h"

#include <algorithm>

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

StatementType NextEvaluatorFacade::getType(Integer stmtNum)
{
    return getStatementType(stmtNum);
}

Integer NextEvaluatorFacade::getLastStatementNumberInWhileLoop(Integer currentStmtNum, Integer whileStmtNum)
{
    Vector<Integer> prevStatementList = this->getPrevious(whileStmtNum, AnyStatement);
    Integer maxNextStmtNum = *std::max_element(prevStatementList.begin(), prevStatementList.end());

    if (this->getType(maxNextStmtNum) == WhileStatement) {
        return this->getLastStatementNumberInWhileLoop(currentStmtNum, maxNextStmtNum);
    } else {
        return maxNextStmtNum;
    }
}

Boolean NextEvaluatorFacade::isNext(Integer prev, Integer next)
{
    return checkIfNextHolds(prev, next);
}

Boolean NextEvaluatorFacade::checksIfCallsStarHolds(ProcedureName p1, ProcedureName p2)
{
    return checkIfCallsHoldsStar(p1, p2);
}

ProcedureName NextEvaluatorFacade::getProcedureOfStmt(StatementNumber stmtNum)
{
    Vector<ProcedureName> optional = getContainingProcedure(stmtNum);
    if (optional.empty()) {
        return "";
    }

    return optional.at(0);
}
