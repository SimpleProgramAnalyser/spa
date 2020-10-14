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

typedef Vector<Pair<String, String>> PairedResult;

// Foreward declaration of SynonymWithValue
class SynonymWithValue;

/**
 * A class implementing a tuple of synonym and string,
 * to associate a synonym with some potential value.
 */
class PotentialValue {
public:
    Synonym synonym;
    String value;

    PotentialValue(Synonym synonym, String value);
    /**
     * Constructor for PotentialValue using a SynonymWithValue.
     */
    explicit PotentialValue(const SynonymWithValue& swv);
    explicit operator SynonymWithValue() const;
    bool operator==(const PotentialValue& pv) const;
    bool operator<(const PotentialValue& pv) const;

    // Convert to SynonymWithValue.
    SynonymWithValue asSwv() const;
};

// A hash function for PotentialValue
struct PotentialValueHasher {
    std::size_t operator()(const PotentialValue& pv) const;
};

/**
 * A synonym with an attached value, that behaves like the synonym
 * by itself in its hash function and equality operators.
 *
 * Very similar to PotentialValue, but two SynonymWithValue with the
 * same synonym and possibly different values will always appear
 * to be the same object when the value is not checked directly.
 */
class SynonymWithValue {
public:
    Synonym synonym;
    String value;

    SynonymWithValue(Synonym synonym, String value);
    /**
     * Constructor with a PotentialValue.
     */
    explicit SynonymWithValue(const PotentialValue& pv): synonym(pv.synonym), value(pv.value) {}
    explicit operator PotentialValue() const;
    bool operator==(const SynonymWithValue& swv) const;

    // Convert to PotentialValue.
    PotentialValue asPv() const;
};

// A hash function for SynonymWithValue
struct SynonymWithValueHasher {
    std::size_t operator()(const SynonymWithValue& swv) const;
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
 * An utility method to convert pairs of integers to pairs of strings.
 * Pairs of strings are used to represent relationships between result
 * values of two different synonyms.
 *
 * @param intPairsList An integer pair vector to convert.
 *
 * @return The converted vector of pairs of strings, or PairedResult.
 */
PairedResult convertToPairedResult(const Vector<Pair<Integer, Integer>>& intPairsList);

/*
 * An utility method to convert pairs of string and integer to pairs
 * of two strings. Pairs of strings are used to represent relationships
 * between result values of two different synonyms.
 *
 * @param intPairsList An string/integer pair vector to convert.
 *
 * @return The converted vector of pairs of strings, or PairedResult.
 */
PairedResult convertToPairedResult(const Vector<Pair<String, Integer>>& intPairsList);

/*
 * An utility method to convert pairs of integer and string to pairs
 * of two strings. Pairs of strings are used to represent relationships
 * between result values of two different synonyms.
 *
 * @param intPairsList An integer/string pair vector to convert.
 *
 * @return The converted vector of pairs of strings, or PairedResult.
 */
PairedResult convertToPairedResult(const Vector<Pair<Integer, String>>& intPairsList);

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
