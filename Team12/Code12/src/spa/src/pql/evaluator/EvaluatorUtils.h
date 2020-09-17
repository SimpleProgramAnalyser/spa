/**
 * Common types and methods used by the Query Evaluator.
 */
#ifndef SPA_EVALUATOR_UTILS_H
#define SPA_EVALUATOR_UTILS_H

#include "Types.h"
#include "pql/preprocessor/AqTypes.h"

typedef Vector<String> ClauseResult;

/*
 * An utility method to convert an integer vector to a string vector.
 * String vector is also what is returned from evaluating a clause.
 *
 * @param intList An integer vector to convert.
 *
 * @return The converted vector of strings, or ClauseResult.
 */
ClauseResult convertToClauseResult(const Vector<Integer>& intList);

/*
 * Given a DesignEntityType, this function maps it to
 * a StatementType, which is a type compatible for calling
 * the PKB API
 *
 * @param entType The DesignEntityType to map to.
 *
 * @return StatementType This represents the corresponding,
 * mapped StatementType object.
 */
StatementType mapToStatementType(DesignEntityType entType);

#endif // SPA_EVALUATOR_UTILS_H
