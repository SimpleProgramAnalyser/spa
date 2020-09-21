#ifndef SPA_BEFORE_H
#define SPA_BEFORE_H

#include <pkb/PkbTypes.h>

typedef ArrayArrayTupleList<Integer, Integer> TupleTable;

/**
 * Stores Follows, Follows* relationships.
 */
class FollowsTable {
public:
    // writing
    void addFollowsRelationships(Integer before, StatementType beforeStmtType, Integer after,
                                 StatementType afterStmtType);
    void addFollowsRelationshipsStar(Integer before, StatementType beforeStmtType,
                                     const Vector<StatementNumWithType>& afterStmttypePairs);

    // reading
    Boolean checkIfFollowsHolds(Integer before, Integer after);
    Boolean checkIfFollowsHoldsStar(Integer before, Integer after);

    Vector<StatementNumWithType> getAfterStatement(Integer before);
    Vector<StatementNumWithType> getBeforeStatement(Integer after);
    Vector<Integer> getAllAfterStatementsStar(Integer before, StatementType stmtType);
    Vector<Integer> getAllBeforeStatementsStar(Integer after, StatementType stmtType);

    Vector<Integer> getAllBeforeStatementsTyped(StatementType stmtTypeOfBefore, StatementType stmtTypeOfAfter);
    Vector<Integer> getAllBeforeStatementsTypedStar(StatementType stmtTypeOfBefore, StatementType stmtTypeOfAfter);
    Vector<Integer> getAllAfterStatementsTyped(StatementType stmtTypeOfBefore, StatementType stmtTypeOfAfter);
    Vector<Integer> getAllAfterStatementsTypedStar(StatementType stmtTypeOfBefore, StatementType stmtTypeOfAfter);
    Vector<Pair<Integer, Integer>> getAllFollowsTuple(StatementType stmtTypeOfBefore, StatementType stmtTypeOfAfter);
    Vector<Pair<Integer, Integer>> getAllFollowsTupleStar(StatementType stmtTypeOfBefore,
                                                          StatementType stmtTypeOfAfter);

private:
    // to check if Follows(*)(x, y) holds
    HashMap<Integer, StatementNumWithType> stmtBeforeMap;
    HashMap<Integer, StatementNumWithType> stmtAfterMap;
    HashMap<Integer, HashSet<Integer>> stmtBeforestarsetMap;
    HashMap<Integer, HashSet<Integer>> stmtAfterstarsetMap;

    // statements before/after given statement, sorted by type
    HashMap<Integer, StatementNumVectorsByType> stmtBeforestarlistMap;
    HashMap<Integer, StatementNumVectorsByType> stmtAfterstarlistMap;

    // WARNING: potential confusion
    // stmtBeforeType:
    /**
     * | StmtType | Statements before this type |
     * | Assign   | Assign: 1, 2; While: 3      |
     */
    Array<StatementNumVectorsByType, STATEMENT_TYPE_COUNT> stmtBeforeType;
    Array<StatementNumVectorsByType, STATEMENT_TYPE_COUNT> stmtBeforeStarType;
    Array<StatementNumVectorsByType, STATEMENT_TYPE_COUNT> stmtAfterType;
    Array<StatementNumVectorsByType, STATEMENT_TYPE_COUNT> stmtAfterStarType;
    TupleTable followsTuples;
    TupleTable followsStarTuples;

    // hashsets to prevent duplication in lists above
    Array<StatementNumSetsByType, STATEMENT_TYPE_COUNT> stmtBeforeTypeSet;
    Array<StatementNumSetsByType, STATEMENT_TYPE_COUNT> stmtBeforeStarTypeSet;
    Array<StatementNumSetsByType, STATEMENT_TYPE_COUNT> stmtAfterTypeSet;
    Array<StatementNumSetsByType, STATEMENT_TYPE_COUNT> stmtAfterStarTypeSet;

    // since the above is confusing, we have some helper functions.
    void typedShenanigans(Integer before, StatementType beforeType, Integer after, StatementType afterType);
    void typedShenanigansStar(Integer before, StatementType beforeType, Integer after, StatementType afterType);
    static void tryAddBefore(Integer before, StatementType beforeType, StatementType afterType, ArrayArrayList& aal,
                             ArrayArraySet& aas);
    static void tryAddAfter(Integer after, StatementType beforeType, StatementType afterType, ArrayArrayList& aal,
                            ArrayArraySet& aas);
};
#endif // SPA_BEFORE_H
