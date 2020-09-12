/**
 * Implementation of the PQL results projector/formatter.
 */

#include "FormattedQueryResult.h"
#include "Projector.h"
#include "RawQueryResult.h"
#include "Types.h"
#include "frontend/parser/StringMatcher.h"

/*
 * Formats the raw query results into a format suitable and conforming to
 * the autotester.
 *
 * @param rawQueryResult The raw query result as returned by the
 * evaluator.
 *
 * @return FormattedQueryResult, which respresents the result
 * in a foramt ready for the autotester/UI.
 */
FormattedQueryResult Projector::formatAutotester(RawQueryResult rawQueryResult)
{
    String formattedResults;
    Vector<Vector<Vector<String>>> rawResults = rawQueryResult.getResults();

    Vector<Vector<String>> reducedRawResults = reduceQueryRawResults(rawResults);

    FormattedQueryResult formattedQueryResult(formattedResults);

    return formattedQueryResult;
}

/*
 * Given a Vector<Vector<Vector<String>>>, which represents the
 * entire raw query result of all clauses in the PQL query
 * pertaining to each particular synonym, for each synonym,
 * combine similar results from all the clauses.
 *
 * It is not hard to see that after this merging, the
 * Vector<Vector<String>> which consists of the results of
 * all clauses partaining to a particular synonym, would be
 * hence reduced to a single Vector<String>.
 *
 * Resultantly, we would do the same for each synonym,
 * and hence reduce the Vector<Vector<Vector<String>>>
 * to just Vector<Vector<String>>.
 *
 * In layman terms, this final result consists of
 * unique results of all clauses pertaining to a particular
 * synonym, for all synonyms.
 *
 * @param rawResults Represents the raw query result
 * per synonym (with respect to all clauses in the query), for
 * all synonyms.
 *
 * @return Vector<Vector<String>> A merged vector, after combining several
 * vectors.
 */
Vector<Vector<String>> Projector::reduceQueryRawResults(Vector<Vector<Vector<String>>> rawResults)
{
    Vector<Vector<String>> mergedResults;

    Integer len = rawResults.size();

    for (int i = 0; i < len; ++i) {
        Vector<Vector<String>> temp = rawResults.at(i);

        Vector<String> merged = reduceQueryRawResultsPerSynonym(temp);

        mergedResults.push_back(merged);
    }

    return mergedResults;
}

/*
 * Given a Vector<Vector<String>>, which represents the raw query result
 * of all clauses in the PQL query pertaining to a particular synonym,
 * combine this vector of vectors into a single vector (if there
 * are similar results from vectors, merge them).
 *
 * @param rawResults Represents the raw query result
 * per synonym (with respect to all clauses in the query).
 *
 * @return Vector<String> A merged vector, after combining several
 * vectors.
 */
Vector<String> Projector::reduceQueryRawResultsPerSynonym(Vector<Vector<String>> rawResults)
{
    Vector<String> mergedResults;

    Integer len = rawResults.size();

    /*
     * We utilise the trie header library provided by Frontend,
     * where we can check if a given string is in a set
     * (of strings).
     */
    auto* trie = new str_match::Trie<Boolean>();

    for (int i = 0; i < len; ++i) {
        Vector<String> temp = rawResults.at(i);

        Integer len2 = temp.size();

        for (int j = 0; j < len2; ++j) {
            String str = temp.at(j);

            // If str not in trie, add it, else ignore it.
            if (!trie->matchString(str, false)) {
                trie->addEntryToTrie(str, false);

                mergedResults.push_back(str);
            }
        }

    }
    return mergedResults;
}

FormattedQueryResult Projector::formatUI(RawQueryResult rawQueryResult)
{
    String formattedResults = "";
    FormattedQueryResult formattedQueryResult(formattedResults);

    return formattedQueryResult;
}
