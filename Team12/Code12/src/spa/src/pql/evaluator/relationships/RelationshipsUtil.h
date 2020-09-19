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
inline Boolean canMatchMultiple(const ReferenceType& refType)
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
inline Boolean canMatchOnlyOne(const ReferenceType& refType)
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
    return ref.getReferenceType() == SynonymRefType && resultsTable->checkIfSynonymHasConstraints(ref.getValue());
}

/**
 * Given two References to Design Entities, check if
 * there is any relationship between them.
 * If one of the the References is a wildcard,
 * it cannot have any relationships.
 *
 * @param leftRef First reference to be checked.
 * @param rightRef Second reference to be checked.
 * @param resultsTable The results table to use.
 *
 * @return True, if leftRef and rightRef have some relationships
 *         between them. False, if they are unrelated.
 */
inline Boolean refsHaveRelationship(const Reference& leftRef, const Reference& rightRef, ResultsTable* resultsTable)
{
    return leftRef.getReferenceType() == SynonymRefType && rightRef.getReferenceType() == SynonymRefType
           && resultsTable->checkIfHaveRelationships(leftRef.getValue(), rightRef.getValue());
}

#endif // SPA_PQL_EVALUATOR_RELATIONSHIPS_UTIL_H
