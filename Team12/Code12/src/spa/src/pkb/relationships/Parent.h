#ifndef SPA_PARENT_H
#define SPA_PARENT_H

#include <pkb/PkbTypes.h>

typedef ArrayArrayTupleList<Integer, Integer> TupleTable;

/**
 * Stores Parent, Parent* relationships.
 */
class ParentTable {
public:
    // writing
    void addParentRelationships(Integer parent, StatementType parentStmtType, Integer child,
                                StatementType childStmtType);
    void addParentRelationshipsStar(Integer parent, StatementType parentStmtType,
                                    const Vector<StatementNumWithType>& childStmttypePairs);

    // reading
    Boolean checkIfParentHolds(Integer parent, Integer child);
    Boolean checkIfParentHoldsStar(Integer parent, Integer child);

    Vector<StatementNumWithType> getChildStatement(Integer parent);
    Vector<StatementNumWithType> getParentStatement(Integer child);
    Vector<Integer> getAllChildStatementsStar(Integer parent, StatementType stmtType);
    Vector<Integer> getAllParentStatementsStar(Integer child, StatementType stmtType);

    Vector<Integer> getAllParentStatementsTyped(StatementType stmtTypeOfParent, StatementType stmtTypeOfChild);
    Vector<Integer> getAllParentStatementsTypedStar(StatementType stmtTypeOfParent, StatementType stmtTypeOfChild);
    Vector<Integer> getAllChildStatementsTyped(StatementType stmtTypeOfParent, StatementType stmtTypeOfChild);
    Vector<Integer> getAllChildStatementsTypedStar(StatementType stmtTypeOfParent, StatementType stmtTypeOfChild);
    Vector<Pair<Integer, Integer>> getAllParentTuple(StatementType stmtTypeOfParent, StatementType stmtTypeOfChild);
    Vector<Pair<Integer, Integer>> getAllParentTupleStar(StatementType stmtTypeOfParent, StatementType stmtTypeOfChild);

private:
    // to check if Parent(*)(x, y) holds
    HashMap<Integer, StatementNumWithType> stmtParentMap;
    HashMap<Integer, StatementNumWithType> stmtChildMap;
    HashMap<Integer, HashSet<Integer>> stmtParentstarsetMap;
    HashMap<Integer, HashSet<Integer>> stmtChildstarsetMap;

    // statements parent/child given statement, sorted by type
    HashMap<Integer, StatementNumVectorsByType> stmtParentstarlistMap;
    HashMap<Integer, StatementNumVectorsByType> stmtChildstarlistMap;

    // WARNING: potential confusion
    // stmtParentType:
    /**
     * | StmtType | Statements parent this type |
     * | Assign   | Assign: 1, 2; While: 3      |
     */
    Array<StatementNumVectorsByType, STATEMENT_TYPE_COUNT> stmtParentType;
    Array<StatementNumVectorsByType, STATEMENT_TYPE_COUNT> stmtParentStarType;
    Array<StatementNumVectorsByType, STATEMENT_TYPE_COUNT> stmtChildType;
    Array<StatementNumVectorsByType, STATEMENT_TYPE_COUNT> stmtChildStarType;
    TupleTable parentTuples;
    TupleTable parentStarTuples;

    // hashsets to prevent duplication in lists above
    Array<StatementNumSetsByType, STATEMENT_TYPE_COUNT> stmtParentTypeSet;
    Array<StatementNumSetsByType, STATEMENT_TYPE_COUNT> stmtParentStarTypeSet;
    Array<StatementNumSetsByType, STATEMENT_TYPE_COUNT> stmtChildTypeSet;
    Array<StatementNumSetsByType, STATEMENT_TYPE_COUNT> stmtChildStarTypeSet;

    // since the above is confusing, we have some helper functions.
    void typedShenanigans(Integer parent, StatementType parentType, Integer child, StatementType childType);
    void typedShenanigansStar(Integer parent, StatementType parentType, Integer child, StatementType childType);
    static void tryAddParent(Integer parent, StatementType parentType, StatementType childType, ArrayArrayList& aal,
                             ArrayArraySet& aas);
    static void tryAddChild(Integer child, StatementType parentType, StatementType childType, ArrayArrayList& aal,
                            ArrayArraySet& aas);
};

#endif // SPA_PARENT_H
