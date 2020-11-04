/**
 * Implementation of a facade used by BIP evaluators.
 */

#include "BipFacade.h"

#include "pkb/PKB.h"

CfgNode* BipFacade::getCfg(const String& procedureName)
{
    return getCFGBip(procedureName);
}

Vector<String> BipFacade::getProcedure(Integer stmtNum)
{
    return getContainingProcedure(stmtNum);
}

Vector<String> BipFacade::getCallersStar(const String& procedureName)
{
    return getAllCallersStar(procedureName);
}
