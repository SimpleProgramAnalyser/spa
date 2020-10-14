/**
 * Methods to retrieve a synonym's attributes from the
 * Program Knowledge Base, given the exact values that
 * matches that synonym in the query.
 */

#ifndef SPA_PQL_ATTRIBUTE_MAP_H
#define SPA_PQL_ATTRIBUTE_MAP_H

#include "pql/evaluator/EvaluatorUtils.h"

/**
 * Given the values for a single synonym, retrieve the
 * corresponding attributes for those values based on
 * the attribute specified within ResultsSynonym.
 *
 * The returned list is a unique set of values matching
 * both the synonym and its attribute.
 *
 * @param values The original values for the synonym.
 * @param synonym The synonym and its attribute.
 * @return Values corresponding to the correct attribute.
 */
ClauseResult mapAttributesOne(const ClauseResult& values, const ResultSynonym& synonym);

/**
 * Given paired values for two synonyms, retrieve the
 * corresponding attributes for those values based on
 * the attributes specified within both ResultsSynonyms.
 *
 * The returned list is a unique set of pairs with
 * values matching both synonyms and their attributes.
 *
 * @param pairs The original values for (syn1, syn2).
 * @param syn1 The first synonym and its attribute.
 * @param syn2 The second synonym and its attribute.
 * @return Pairs of values corresponding to the correct attributes.
 */
PairedResult mapAttributesTwo(const PairedResult& pairs, const ResultSynonym& syn1, const ResultSynonym& syn2);

/**
 * Given n-tupled values for n synonyms, retrieve the
 * corresponding attributes for those values based on
 * the attributes specified within all ResultsSynonyms.
 *
 * The returned list is a unique set of n-tuples with
 * values matching all the synonyms and their attributes.
 *
 * @param tuples The original values for (syns[1], syns[2], ..., syns[n]).
 * @param syns The synonyms and their attributes.
 * @return N-tuples of values corresponding to the correct attributes.
 */
NtupledResult mapAttributesN(const NtupledResult& tuples, const Vector<ResultSynonym>& syns);

#endif // SPA_PQL_ATTRIBUTE_MAP_H
