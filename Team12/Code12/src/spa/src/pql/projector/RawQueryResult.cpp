/**
 * Implementation of the RawQueryResult class,
 * and some handy (public) methods.
 */

#include "RawQueryResult.h"

#include <algorithm>

/*
 * Constructs a RawQueryResult instance, from a Vector
 * of RawResultFromClauses. This constructor is the
 * only public constructor available for creating a
 * new instance of RawQueryResult representing a
 * query that was successfully evaluated.S
 *
 * @param results A Vector<RawResultFromClauses> representing
 * the raw query results of all clauses in the query,
 * where each element in the Vector represents a
 * synonym (in the PQL query) through which the clauses
 * are evaluated with respect to.
 *
 * @return A new instance of this class.
 *
 */
RawQueryResult::RawQueryResult(Vector<String> results): hasError(false), results{std::move(results)} {}

/*
 * Constructs a RawQueryResult instance that represents a
 * syntax error in the Query Preprocessor. This constructor
 * is a private constructor for creating a new instance of
 * the object, to be used internally (e.g, creating
 * RawQueryResult for syntax error).
 *
 * @return A new instance of this class.
 */
RawQueryResult::RawQueryResult(String errorMessage, Boolean isSyntaxError):
    hasError(true), errorMessage(InputError(std::move(errorMessage), 0, 0, ErrorSource::Query,
                                            isSyntaxError ? ErrorType::Syntax : ErrorType::Semantic)),
    results()
{}

/*
 * Constructs a RawQueryResult instance that represents a
 * semantic error with Select BOOLEAN in the Query Preprocessor.
 *
 * @return A new instance of this class.
 */
RawQueryResult::RawQueryResult(String errorMessage, Vector<String> results):
    hasError(true), errorMessage(InputError(std::move(errorMessage), 0, 0, ErrorSource::Query, ErrorType::Semantic)),
    results(std::move(results))
{}

/*
 * Returns a RawQueryResult that represents a syntax error
 * in the Query Preprocessor.
 *
 * @return RawQueryResult representing syntax error
 */
RawQueryResult RawQueryResult::getSyntaxError(String errorMessage)
{
    return RawQueryResult(std::move(errorMessage), true);
}

/*
 * Returns a RawQueryResult that represents a semantic error
 * in the Query Preprocessor.
 *
 * @return RawQueryResult representing semantic error
 */
RawQueryResult RawQueryResult::getSemanticError(String errorMessage)
{
    return RawQueryResult(std::move(errorMessage), false);
}

/*
 * Returns a RawQueryResult that represents a semantic error
 * in the Query Preprocessor. This query should return FALSE.
 *
 * @return RawQueryResult representing syntax error
 */
RawQueryResult RawQueryResult::getFalseResultWithSemanticError(String errorMessage)
{
    return RawQueryResult(std::move(errorMessage), Vector<String>({"FALSE"}));
}

/*
 * Checks if the results list is empty.
 *
 * @return True if results list indeed empty,
 * false otherwise.
 */
Boolean RawQueryResult::isEmpty()
{
    return results.empty();
}

/*
 * Retrieves a particular String in the
 * Vector, given an index.
 *
 * @param index The index of the vector, to access.
 *
 * @return The String.
 */
String RawQueryResult::get(Integer index)
{
    return results.at(index);
}

/*
 * Retrieves the size of the vector (i.e, number of
 * RawResultFromClauses element in the Vector).
 *
 * @return The size of the RawResultFromClauses vector.
 */
size_t RawQueryResult::count()
{
    return results.size();
}

/*
 * Determines when 2 RawQueryResult objects are equal.
 * This method overloads equality operator and defines
 * what it means for 2 such objects to be equal.
 *
 * In our definition, this equality holds if and only
 * if the ClauseResult contains the same elements at
 * every position when they are sorted.
 *
 * @param rawQueryResult Another RawQueryResult object
 * to compare with current instance.
 *
 * @return Boolean, true if equals, false otherwise.
 */
Boolean RawQueryResult::operator==(const RawQueryResult& rawQueryResult) const
{
    std::vector<std::string> sortedThis = this->results;
    std::vector<std::string> sortedOther = rawQueryResult.results;
    std::sort(sortedThis.begin(), sortedThis.end());
    std::sort(sortedOther.begin(), sortedOther.end());
    return sortedThis == sortedOther && this->hasError == rawQueryResult.hasError
           && this->errorMessage == rawQueryResult.errorMessage;
}

/**
 * Sorts the RawQueryResult by lexicographical order
 */
void RawQueryResult::sort()
{
    std::sort(results.begin(), results.end(), [](const std::string& a, const std::string& b) {
        if (!a.empty() && std::all_of(a.begin(), a.end(), ::isdigit) && !b.empty()
            && std::all_of(b.begin(), b.end(), ::isdigit)) {

            return std::stoi(a) < std::stoi(b);
        } else {
            return a < b;
        }
    });
}
