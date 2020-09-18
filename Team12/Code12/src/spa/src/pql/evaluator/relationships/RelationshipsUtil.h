/**
 * Utility functions for Query Evaluator, specifically
 * for "such that" clauses describing certain design
 * entity relationships. These functions make it easier
 * to retrieve relationship information from the
 * Program Knowledge Base.
 */
#ifndef SPA_QUERY_EVALUATOR_RELATIONSHIPS_UTIL_H
#define SPA_QUERY_EVALUATOR_RELATIONSHIPS_UTIL_H

#include "pkb/PKB.h"

//================ HELPER METHODS FOR PKB ================

inline Vector<Integer> verifyStatementType(const StatementType& stmtType, Vector<StatementNumWithType>& result)
{
    if (!result.empty() && result.at(0).second == stmtType) {
        return std::vector<Integer>(result.at(0).first);
    } else {
        return std::vector<Integer>();
    }
}

inline Vector<Integer> getAllBeforeStatements(Integer after, StatementType stmtType)
{
    Vector<StatementNumWithType> result = getBeforeStatement(after);
    return verifyStatementType(stmtType, result);
}

inline Vector<Integer> getAllAfterStatements(Integer before, StatementType stmtType)
{
    Vector<StatementNumWithType> result = getAfterStatement(before);
    return verifyStatementType(stmtType, result);
}

inline Vector<Integer> getAllParentStatements(Integer child, StatementType stmtType)
{
    Vector<StatementNumWithType> result = getParentStatement(child);
    return verifyStatementType(stmtType, result);
}

inline Vector<Integer> getAllChildStatements(Integer parent, StatementType stmtType)
{
    Vector<StatementNumWithType> result = getChildStatement(parent);
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
Boolean canMatchMultiple(const ReferenceType& refType)
{
    return (refType == SynonymRefType || refType == WildcardRefType);
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
Boolean canMatchOnlyOne(const ReferenceType& refType)
{
    return (refType == IntegerRefType || refType == LiteralRefType);
}

/**
 * Given a Reference to some Design Entity, check if
 * this reference is constrained by the results table.
 * If the Reference is a wildcard, it cannot
 * be constrained.
 *
 * @param ref The reference to be checked.
 * @param resultsTable The results table to use.
 *
 * @return True, if ref is a synonym constrained by previously
 *         identified clause results stored in resultsTable.
 *         This returns false for refs that can match all
 *         possible values of its type, like wildcards or
 *         synonyms that have no other results.
 */
inline Boolean refHasConstraints(const Reference& ref, ResultsTable* resultsTable)
{
    return ref.getReferenceType() == SynonymRefType && resultsTable->checkIfSynonymHasConstraints(ref.getValue())
}

#endif // SPA_QUERY_EVALUATOR_RELATIONSHIPS_UTIL_H
