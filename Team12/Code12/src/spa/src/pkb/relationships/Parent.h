#ifndef SPA_PARENT_H
#define SPA_PARENT_H

#include <pkb/PkbTypes.h>

/**
 * Stores Parent, Parent* relationships.
 */
class ParentTable {
public:
    // writing
    void addParentRelationships(Integer parent, Integer child);
    void addParentRelationshipsStar(Integer parent, Vector<Integer> children);

    // reading
    Boolean checkIfParentHolds(Integer parent, Integer child);
    Boolean checkIfParentHoldsStar(Integer parent, Integer child);

    Vector<Integer> getAllChildStatements(Integer parent, StatementType stmtType);
    Vector<Integer> getAllParentStatements(Integer parent, StatementType stmtType);
    Vector<Integer> getAllChildStatementsStar(Integer parent, StatementType stmtType);
    Vector<Integer> getAllParentStatementsStar(Integer parent, StatementType stmtType);

    Vector<Integer> getAllParentStatementsTyped(StatementType stmtTypeOfParent, StatementType stmtTypeOfChild);
    Vector<Integer> getAllParentStatementsTypedStar(StatementType stmtTypeOfParent, StatementType stmtTypeOfChild);
    Vector<Integer> getAllChildStatementsTyped(StatementType stmtTypeOfParent, StatementType stmtTypeOfChild);
    Vector<Integer> getAllChildStatementsTypedStar(StatementType stmtTypeOfParent, StatementType stmtTypeOfChild);

private:
    HashMap<Integer, Integer> stmtParentMap;
    HashMap<Integer, Integer> stmtChildMap;

    HashMap<Integer, StatementNumVectorsByType> stmtParentstarlistMap;
    HashMap<Integer, StatementNumVectorsByType> stmtChildstarlistMap;

    HashMap<Integer, HashSet<Integer>> stmtParentstarMap;
    HashMap<Integer, HashSet<Integer>> stmtChildstarMap;

    Vector<Integer> parentByType[STATEMENT_TYPE_COUNT];
    Vector<Integer> parentStarByType[STATEMENT_TYPE_COUNT];
    Vector<Integer> childByType[STATEMENT_TYPE_COUNT];
    Vector<Integer> childStarByType[STATEMENT_TYPE_COUNT];
};

#endif // SPA_PARENT_H
