/**
 * Common types and methods used by the Query Evaluator.
 */
#ifndef SPA_EVALUATOR_UTILS_H
#define SPA_EVALUATOR_UTILS_H

#include <utility>

#include "Types.h"
#include "pql/preprocessor/AqTypes.h"

typedef Vector<String> ClauseResult;

template <typename T, typename U>
using Pair = std::pair<T, U>;

typedef std::string Value;
/**
 * A class implementing a tuple of synonym and string,
 * to associate a synonym with some potential value.
 */
class PotentialValue {
public:
    Synonym synonym;
    String value;

    PotentialValue(Synonym s, String val): synonym(std::move(s)), value(std::move(val)) {}

    bool operator==(const PotentialValue& pv) const
    {
        return this->synonym == pv.synonym && this->value == pv.value;
    }
};

// A hash function for PotentialValue
struct PotentialValueHasher {
    std::size_t operator()(const PotentialValue& pv) const
    {
        std::hash<std::string> stringHasher;
        std::size_t hashedSynonym = stringHasher(pv.synonym);
        return (hashedSynonym
                ^ (stringHasher(pv.value) + uint32_t(2654435769) + (hashedSynonym * 64) + (hashedSynonym / 4)));
    }
};

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

/**
 * Given a clause result, return another clause
 * result with all unique elements from the first
 * list. Duplicate elements will be removed.
 *
 * @param list The list to remove duplicates.
 * @return A clause result with duplicates removed.
 */
ClauseResult removeDuplicates(const ClauseResult& list);

#endif // SPA_EVALUATOR_UTILS_H
