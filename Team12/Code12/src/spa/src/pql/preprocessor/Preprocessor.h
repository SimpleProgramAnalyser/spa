#ifndef SPA_PQL_PREPROCESSOR_H
#define SPA_PQL_PREPROCESSOR_H

#include "AqTypes.h"

namespace Preprocessor {
/**
 * Processes a given PQL query into an AbstractQuery.
 * The returned AbstractQuery will contain the selected
 * synonym, all the declarations in a DeclarationTable
 * and all the clauses contained separately in a
 * ClauseVector.
 *
 * If the PQL query is syntactically or semantically
 * incorrect, an invalid AbstractQuery will be returned.
 *
 * @param query PQL query string to be processed
 * @return      AbstractQuery that breaks the PQL query into
 *              abstract objects that can be evaluated.
 */
AbstractQuery processQuery(const String& query);
} // namespace Preprocessor

/**
 * Processes the declaration component of the PQL query.
 * A DeclarationTable will be created with the Synonym as
 * the key and the DesignEntity of the Synonym as the
 * value.
 *
 * If any of the declarations is invalid, or a syntax
 * error is encountered, an invalid DeclarationTable will
 * be returned.
 *
 * @param declarationsString    The substring of the PQL query with all
 *                              the declarations.
 * @return                      A DeclarationTable that stores all stated
 *                              declarations in the declarationsString.
 */
DeclarationTable processDeclarations(const String& declarationString);

/**
 * Processes the clauses component of the PQL query, and
 * split them up accordingly into separate clauses stored
 * in a clause vector. The different clauses include
 * SuchThatClause and PatternClause.
 *
 * @param clausesString String of clauses to be processed. String
 *                      starts from after the selected synonym.
 * @return              ClauseVector of all clauses in the
 *                      clausesString.
 */
ClauseVector processClauses(const String& clausesString, DeclarationTable& declarationTable);

/**
 * Splits up the given string by the first
 * non-alphanumeric character (or whitespace
 * character). First character should be an
 * alphanumeric character.
 *
 * @param str String to be split
 * @return Vector of 2 strings
 */
StringVector splitByFirstNonAlphanum(const String& str);

/**
 * Splits up the given string by the first
 * consecutive whitespaces, into two substrings.
 * Given string should be trimmed.
 *
 * @param str String to be split
 * @return vector of 2 strings
 */
StringVector splitByFirstConsecutiveWhitespace(const String& str);

/**
 * Checks if a given string contains any open
 * parentheses.
 *
 * @param str   String to be checked/
 * @return      True if string contains open
 *              parentheses.
 */
Boolean containsOpenParentheses(const String& str);

/**
 * Returns a pair of Strings, split by the
 * last appearance of the semicolon. If the
 * semicolon does not exist, a pair of empty
 * strings will be returned.
 *
 * @param query String to be split.
 * @return Pair of Strings split by delimiter.
 */
StringPair splitDeclarationAndSelectClause(const String& query);

/**
 * Counts the number of opened parentheses in the
 * newly appended clause constraint. Opened
 * parentheses here means open parentheses that
 * are not yet closed by a close parentheses.
 *
 * It uses the number of opened parentheses in the
 * token, and add it to the count of number of
 * opened parentheses before the current token was
 * appended.
 *
 * If a close parentheses in this token that closes
 * the first open parentheses in the newly appended
 * clause constraint exist, any further parentheses
 * encountered in this token would deem the clause
 * invalid.
 *
 * If there ever a point where the number of opened
 * parentheses is negative, which means there are
 * more close than open parentheses, the clause
 * would be deemed invalid.
 *
 * @param token                         Current token being processed
 * @param previousNumOfOpenParentheses  Previous number of opened parentheses
 *                                      from the clause constraint before it
 *                                      was appended with token.
 * @return                              Current number of opened parentheses
 */
std::pair<Boolean, Integer> countNumOfOpenParentheses(const String& token, Integer previousNumOfOpenParentheses);

/**
 * Processes the selected result(s) string of the
 * of the PQL query. It check if the result(s) is
 * a BOOLEAN, single Synonym or a tuple of Synonyms,
 * and returns a ResultSynonymVector accordingly.
 *
 * @param selectResultString    Selected result(s) string.
 * @param declarationTable      DeclarationTable to check validity
 *                              of Synonyms if any.
 * @return                      ResultSynonymVector containing the
 *                              selected result Synonym(s).
 */
ResultSynonymVector processSelectResultString(String selectResultString, DeclarationTable& declarationTable);

/**
 * Process the individual result string of the
 * PQL Query. It checks if the Synonym has any
 * attribute, and validate them accordingly.
 *
 * @param resultSynonymString   Selected result string.
 * @param declarationTable      DeclarationTable to check validity
 *                              of Synonym.
 * @return                      ResultSynonym form of the result
 *                              string.
 */
ResultSynonym processResultSynonym(const String& resultSynonymString, DeclarationTable& declarationTable);

/**
 * Splits the selected synonym, tuple or BOOLEAN
 * from the rest of the clauses.
 * @param s     Select clause without `Select` keyword.
 * @return      StringVector that contains either one or two strings.
 *              If StringVector only contains one string, that means
 *              that are no constraint clauses.
 */
StringVector splitResultAndClauses(String& s);

#endif // SPA_PQL_PREPROCESSOR_H
