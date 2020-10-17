/**
 * Utility functions for Query Evaluator, specifically
 * for "such that" clauses describing certain design
 * entity relationships. These functions make it easier
 * to retrieve relationship information from the
 * Program Knowledge Base.
 */
#ifndef SPA_PQL_EVALUATOR_RELATIONSHIPS_UTIL_H
#define SPA_PQL_EVALUATOR_RELATIONSHIPS_UTIL_H

#include "pkb/PKB.h"
#include "pql/evaluator/ResultsTable.h"
#include "pql/preprocessor/AqTypes.h"

//================ HELPER METHODS FOR PKB (Follows and Parent) ================

inline Vector<Integer> verifyStatementType(const StatementType& stmtType, const Vector<StatementNumWithType>& result)
{
    Vector<Integer> filteredStatements;
    for (const StatementNumWithType& pairedStmt : result) {
        if (pairedStmt.second == stmtType || stmtType == AnyStatement) {
            filteredStatements.push_back(pairedStmt.first);
        }
    }
    return filteredStatements;
}

inline Vector<Integer> getAllBeforeStatements(Integer after, StatementType stmtType)
{
    Vector<StatementNumWithType> result = getBeforeStatement(after);
    return verifyStatementType(stmtType, result);
}

inline Vector<Integer> getAllAfterStatements(Integer before, StatementType stmtType)
{
    Vector<StatementNumWithType> result = getAfterStatement(before);
    Vector<Integer> check = verifyStatementType(stmtType, result);
    return check;
}

inline Vector<Integer> getAllParentStatements(Integer child, StatementType stmtType)
{
    Vector<StatementNumWithType> result = getParentStatement(child);
    return verifyStatementType(stmtType, result);
}

//================ HELPER METHODS FOR DESIGN ENTITY TYPE ================

/**
 * Checks if a ReferenceType corresponds to multiple
 * query results, namely if the ReferenceType is
 * the Synonym type or Wildcard type.
 *
 * @param refType The ReferenceType to be checked.
 * @return True, if the Reference holding this
 *         type can match more than one result.
 *         Otherwise, false.
 */
inline Boolean canMatchMultiple(const ReferenceType& refType)
{
    return (refType == SynonymRefType || refType == WildcardRefType || refType == AttributeRefType);
}

/**
 * The opposite of canMatchMultiple, checks if a
 * ReferenceType can only correspond to one
 * design entity in the program.
 *
 * @param refType The ReferenceType to be checked.
 * @return True, if the Reference holding this
 *         type can only match one entity.
 *         Otherwise, false.
 */
inline Boolean canMatchOnlyOne(const ReferenceType& refType)
{
    return (refType == IntegerRefType || refType == LiteralRefType);
}

#endif // SPA_PQL_EVALUATOR_RELATIONSHIPS_UTIL_H
