#ifndef SPA_CALLS_H
#define SPA_CALLS_H

#include <pkb/PkbTypes.h>

class CallsTable {
public:
    // Section 1: Add methods
    void addCallerRelationships(const ProcedureName& caller, const ProcedureName& callee);
    void addCallerRelationshipsStar(const ProcedureName& caller, const ProcedureName& callee);

    // Section 2: Table and inverse
    Boolean checkIfCallsHolds(const ProcedureName& caller, const ProcedureName& callee);
    Boolean checkIfCallsHoldsStar(const ProcedureName& caller, const ProcedureName& callee);
    Vector<ProcedureName> getAllCallers(const ProcedureName& callee);
    Vector<ProcedureName> getAllCallersStar(const ProcedureName& callee);
    Vector<ProcedureName> getAllCallees(const ProcedureName& caller);
    Vector<ProcedureName> getAllCalleesStar(const ProcedureName& caller);
    Vector<Pair<ProcedureName, ProcedureName>> getAllCallsTuple();
    Vector<Pair<ProcedureName, ProcedureName>> getAllCallsTupleStar();

private:
    // Table and inverse
    /**
     * Primary key: ProcedureName
     * Value: Vector<ProcedureName>
     */
    HashMap<ProcedureName, Vector<ProcedureName>> procCallerMap;
    HashMap<ProcedureName, Vector<ProcedureName>> procCalleeMap;
    HashMap<ProcedureName, Vector<ProcedureName>> procCallerMapStar;
    HashMap<ProcedureName, HashSet<ProcedureName>> procCallerSetStar;
    // de-duplicating sets
    HashMap<ProcedureName, HashSet<ProcedureName>> procCallerSet;
    HashMap<ProcedureName, HashSet<ProcedureName>> procCalleeSet;
    HashMap<ProcedureName, Vector<ProcedureName>> procCalleeMapStar;
    HashMap<ProcedureName, HashSet<ProcedureName>> procCalleeSetStar;

    // Tuples
    /**
     * Just a collection of tuples.
     */
    Vector<Pair<ProcedureName, ProcedureName>> callsTuples;
    // de-duplicating set
    Vector<Pair<ProcedureName, ProcedureName>> callsTuplesStar;

    // we only have basic and tuple here
    void addIntoBasicTables(const ProcedureName& caller, const ProcedureName& callee);
    void addIntoTupleTables(const ProcedureName& caller, const ProcedureName& callee);
    void addIntoBasicTablesStar(const ProcedureName& caller, const ProcedureName& callee);
    void addIntoTupleTablesStar(const ProcedureName& caller, const ProcedureName& callee);
};

#endif // SPA_CALLS_H
