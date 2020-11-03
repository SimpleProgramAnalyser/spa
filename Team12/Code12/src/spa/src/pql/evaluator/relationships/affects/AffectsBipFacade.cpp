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
    return getCFGBip(procedureName);
}
