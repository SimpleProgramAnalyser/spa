#ifndef SPA_BEFORE_H
#define SPA_BEFORE_H

#include <pkb/PkbTypes.h>

void addFollowsRelationships(Integer before, Integer after);
void addFollowsRelationshipsStar(Integer before, Vector<Integer> after);

class BeforeTable {
public:
    // writing
    void addBeforeRelationships(Integer parent, Integer child);
    void addBeforeRelationshipsStar(Integer parent, Vector<Integer> children);

    // reading
    Boolean checkIfFollowsHolds(Integer parent, Integer child);
    Boolean checkIfFollowsHoldsStar(Integer parent, Integer child);

    Vector<Integer> getAllAfterStatements(Integer parent, StatementType stmtType);
    Vector<Integer> getAllBeforeStatements(Integer parent, StatementType stmtType);
    Vector<Integer> getAllAfterStatementsStar(Integer parent, StatementType stmtType);
    Vector<Integer> getAllBeforeStatementsStar(Integer parent, StatementType stmtType);

    Vector<Integer> getAllBeforeStatementsTyped(StatementType stmtTypeOfBefore, StatementType stmtTypeOfAfter);
    Vector<Integer> getAllBeforeStatementsTypedStar(StatementType stmtTypeOfBefore, StatementType stmtTypeOfAfter);
    Vector<Integer> getAllAfterStatementsTyped(StatementType stmtTypeOfBefore, StatementType stmtTypeOfAfter);
    Vector<Integer> getAllAfterStatementsTypedStar(StatementType stmtTypeOfBefore, StatementType stmtTypeOfAfter);

private:
    HashMap<Integer, Integer> stmtBeforeMap;
    HashMap<Integer, Integer> stmtAfterMap;

    HashMap<Integer, StatementNumVectorsByType> stmtBeforestarlistMap;
    HashMap<Integer, StatementNumVectorsByType> stmtAfterstarlistMap;

    HashMap<Integer, HashSet<Integer>> stmtBeforestarMap;
    HashMap<Integer, HashSet<Integer>> stmtAfterstarMap;

    Vector<Integer> beforeByType[STATEMENT_TYPE_COUNT];
    Vector<Integer> beforeStarByType[STATEMENT_TYPE_COUNT];
    Vector<Integer> afterByType[STATEMENT_TYPE_COUNT];
    Vector<Integer> afterStarByType[STATEMENT_TYPE_COUNT];
};

#endif // SPA_BEFORE_H
