/**
 * Implementation of an Affects Evaluator Facade
 * for the main Affects operation.
 */

#include "AffectsEvaluatorFacade.h"

#include "pkb/PKB.h"

Vector<String> AffectsEvaluatorFacade::getUsed(Integer stmtNum)
{
    return getUsesVariablesFromStatement(stmtNum);
}

Vector<String> AffectsEvaluatorFacade::getModified(Integer stmtNum)
{
    return getModifiesVariablesFromStatement(stmtNum);
}

Vector<Integer> AffectsEvaluatorFacade::getNext(Integer stmtNum)
{
    return getAllNextStatements(stmtNum, AnyStatement);
}

Vector<Integer> AffectsEvaluatorFacade::getPrevious(Integer stmtNum)
{
    return getAllPreviousStatements(stmtNum, AnyStatement);
}

StatementType AffectsEvaluatorFacade::getType(Integer stmtNum)
{
    return getStatementType(stmtNum);
}

Vector<Integer> AffectsEvaluatorFacade::getAssigns()
{
    return getAllStatements(AssignmentStatement);
}

Vector<String> AffectsEvaluatorFacade::getRelevantProcedures()
{
    return getProceduresWithCFG();
}

CfgNode* AffectsEvaluatorFacade::getCfg(const String& procedureName)
{
    return getCFG(procedureName);
}

Boolean AffectsEvaluatorFacade::doesStatementUse(Integer stmtNum, const String& variable)
{
    return checkIfStatementUses(stmtNum, variable);
}

Boolean AffectsEvaluatorFacade::doesStatementModify(Integer stmtNum, const String& variable)
{
    StatementType type = getType(stmtNum);
    if (!(type == AssignmentStatement || type == CallStatement || type == ReadStatement)) {
        // ignore container statements, according to definition of Affects
        return false;
    } else {
        return checkIfStatementModifies(stmtNum, variable);
    }
}
