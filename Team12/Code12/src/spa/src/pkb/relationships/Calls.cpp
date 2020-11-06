#include "Calls.h"

#include <pkb/PKBUtils.h>

/**
 * Adds relationship as given by parameters into the relevant basic tables (table and its inverse).
 *
 * @param caller
 * @param callee
 */
void CallsTable::addIntoBasicTables(const ProcedureName& caller, const ProcedureName& callee)
{
    deduplicatedAdd(callee, procCalleeMap[caller], procCalleeSet[caller]);
    deduplicatedAdd(caller, procCallerMap[callee], procCallerSet[callee]);
}

/**
 * Adds relationship as given by parameters into the relevant tuple tables (table and its inverse).
 *
 * @param caller
 * @param callee
 */
void CallsTable::addIntoTupleTables(const ProcedureName& caller, const ProcedureName& callee)
{
    callsTuples.push_back(std::make_pair(caller, callee));
}

/**
 * Adds relationship (star) as given by parameters into the relevant basic tables (table and its inverse).
 *
 * @param caller
 * @param callee
 */
void CallsTable::addIntoBasicTablesStar(const ProcedureName& caller, const ProcedureName& callee)
{
    deduplicatedAdd(callee, procCalleeMapStar[caller], procCalleeSetStar[caller]);
    deduplicatedAdd(caller, procCallerMapStar[callee], procCallerSetStar[callee]);
}

/**
 * Adds relationship (star) as given by parameters into the relevant tuple tables (table and its inverse).
 *
 * @param caller
 * @param callee
 */
void CallsTable::addIntoTupleTablesStar(const ProcedureName& caller, const ProcedureName& callee)
{
    callsTuplesStar.push_back(std::make_pair(caller, callee));
}

/**
 * Given a relationship Calls(a, b), insert them into the basic tables and tuple tables. Idempotent.
 *
 * @param caller
 * @param callee
 */
void CallsTable::addCallerRelationships(const ProcedureName& caller, const ProcedureName& callee)
{
    addIntoBasicTables(caller, callee);
    addIntoCollectionTables(caller, callee);
    addIntoTupleTables(caller, callee);
}

/**
 * Given a relationship Calls*(a, b), insert them into the basic tables and tuple tables. Idempotent.
 *
 * @param caller
 * @param callee
 */
void CallsTable::addCallerRelationshipsStar(const ProcedureName& caller, const ProcedureName& callee)
{
    addIntoBasicTablesStar(caller, callee);
    addIntoCollectionTablesStar(caller, callee);
    addIntoTupleTablesStar(caller, callee);
}

/**
 * Returns TRUE if there is a Calls relationship between caller and callee, else return FALSE.
 *
 * @param caller
 * @param callee
 * @return
 */
Boolean CallsTable::checkIfCallsHolds(const ProcedureName& caller, const ProcedureName& callee)
{
    return procCalleeSet.find(caller) != procCalleeSet.end()
           && procCallerSet[callee].find(caller) != procCallerSet[callee].end();
}

/**
 * Returns TRUE if there is a Calls* relationship between caller and callee, else return FALSE.
 *
 * @param caller
 * @param callee
 * @return
 */
Boolean CallsTable::checkIfCallsHoldsStar(const ProcedureName& caller, const ProcedureName& callee)
{
    return procCalleeSetStar.find(caller) != procCalleeSetStar.end()
           && procCallerSetStar[callee].find(caller) != procCallerSetStar[callee].end();
}

/**
 * Returns a Vector<ProcedureName> containing all procedure names that calls callee directly. Vector<ProcedureName>
 * returned is empty if there is no procedure that calls callee directly.
 *
 * @param callee
 * @return
 */
Vector<ProcedureName> CallsTable::getAllCallers(const ProcedureName& callee)
{
    return procCallerMap[callee];
}

/**
 * Returns a Vector<ProcedureName> containing all procedure names that calls callee directly or indirectly.
 * Vector<ProcedureName> returned is empty if there is no procedure that calls callee.
 *
 * @param callee
 * @return
 */
Vector<ProcedureName> CallsTable::getAllCallersStar(const ProcedureName& callee)
{
    return procCallerMapStar[callee];
}

/**
 * Returns a Vector<ProcedureName> containing all ProcedureNames that are called by caller directly.
 * Vector<ProcedureName> returned is empty if there is no procedure that are called by caller directly.
 *
 * @param caller
 * @return
 */
Vector<ProcedureName> CallsTable::getAllCallees(const ProcedureName& caller)
{
    return procCalleeMap[caller];
}

/**
 * Returns a Vector<ProcedureName> containing all ProcedureNames that are called by caller directly or indirectly.
 * Vector<ProcedureName> returned is empty if there is no procedure that are called by caller.
 *
 * @param caller
 * @return
 */
Vector<ProcedureName> CallsTable::getAllCalleesStar(const ProcedureName& caller)
{
    return procCalleeMapStar[caller];
}

/**
 * Returns a list of pairs of ProcedureName, where for each pair, the Calls relationship holds between them.
 *
 * @return
 */
Vector<Pair<ProcedureName, ProcedureName>> CallsTable::getAllCallsTuple()
{
    return callsTuples;
}

/**
 * Returns a list of pairs of ProcedureName, where for each pair, the Calls* relationship holds between them.
 *
 * @return
 */
Vector<Pair<ProcedureName, ProcedureName>> CallsTable::getAllCallsTupleStar()
{
    return callsTuplesStar;
}

/**
 * Returns a list of procedure names which procedure called some other procedure.
 *
 * @return
 */
Vector<ProcedureName> CallsTable::getAllCallers()
{
    return callers;
}

/**
 * Returns a list of procedure names which procedure was called by some other procedure.
 *
 * @return
 */
Vector<ProcedureName> CallsTable::getAllCallees()
{
    return callees;
}

/**
 * Returns a list of procedure names which procedure called some other procedure directly or indirectly.
 *
 * @return
 */
Vector<ProcedureName> CallsTable::getAllCallersStar()
{
    return callersStar;
}

/**
 * Returns a list of procedure names which procedure was called by some other procedure directly or indirectly.
 *
 * @return
 */
Vector<ProcedureName> CallsTable::getAllCalleesStar()
{
    return calleesStar;
}

/**
 * Given caller and callee, add caller into a collection of all callers, same with callee.
 *
 * @param caller
 * @param callee
 */
void CallsTable::addIntoCollectionTables(const ProcedureName& caller, const ProcedureName& callee)
{
    deduplicatedAdd(caller, callers, callersSet);
    deduplicatedAdd(callee, callees, calleesSet);
}

/**
 * Given caller and callee, add caller into a collection of all callers, same with callee.
 *
 * @param caller
 * @param callee
 */
void CallsTable::addIntoCollectionTablesStar(const ProcedureName& caller, const ProcedureName& callee)
{
    deduplicatedAdd(caller, callersStar, callersStarSet);
    deduplicatedAdd(callee, calleesStar, calleesStarSet);
}
