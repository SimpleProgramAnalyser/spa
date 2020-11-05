/**
 * Implementation of an Affects BIP Facade
 * for the Affects operation that involves
 * branching into other procedures.
 */

#include "AffectsBipFacade.h"

#include "pkb/PKB.h"

Vector<String> AffectsBipFacade::getModified(Integer stmtNum)
{
    if (this->getType(stmtNum) == CallStatement) {
        return Vector<String>();
    } else {
        return getModifiesVariablesFromStatement(stmtNum);
    }
}

Vector<Integer> AffectsBipFacade::getNext(Integer stmtNum)
{
    return getAllNextBipStatements(stmtNum, AnyStatement);
}

Vector<Integer> AffectsBipFacade::getPrevious(Integer stmtNum)
{
    return getAllPreviousBipStatements(stmtNum, AnyStatement);
}

Vector<String> AffectsBipFacade::getRelevantProcedures()
{
    return getProceduresWithCFGBip();
}

CfgNode* AffectsBipFacade::getCfg(const String& procedureName)
{
    return BipFacade::getCfg(procedureName);
}

Boolean AffectsBipFacade::doesStatementModify(Integer stmtNum, const String& variable)
{
    StatementType type = getType(stmtNum);
    if (!(type == AssignmentStatement || type == ReadStatement)) {
        // ignore container statements and Call statement for BIP
        return false;
    } else {
        return checkIfStatementModifies(stmtNum, variable);
    }
}
