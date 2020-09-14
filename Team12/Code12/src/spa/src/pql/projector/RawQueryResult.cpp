/**
 * Implementation of the RawQueryResult class,
 * and some handy (public) methods.
 */

#include "RawQueryResult.h"

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
RawQueryResult::RawQueryResult(Vector<RawResultFromClauses> results):
    isSyntaxError(false), results{std::move(results)}
{}

/*
 * Constructs a RawQueryResult instance that represents a
 * syntax error in the Query Preprocessor. This constructor
 * is a private constructor for creating a new instance of
 * the object, to be used internally (e.g, creating
 * RawQueryResult for syntax errot).
 *
 * @return A new instance of this class.
 */
RawQueryResult::RawQueryResult(String errorMessage):
    isSyntaxError(true), errorMessage(std::move(errorMessage)), results()
{}

/*
 * Returns a RawQueryResult that represents a syntax error
 * in the Query Preprocessor.
 *
 * @return RawQueryResult representing syntax error
 */
RawQueryResult RawQueryResult::getSyntaxError(String errorMessage)
{
    return RawQueryResult(std::move(errorMessage));
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
 * Retrieves a particular RawResultFromClauses in the
 * Vector, given an index.
 *
 * @param index The index of the vector, to access.
 *
 * @return The sought RawResultFromClauses.
 */
RawResultFromClauses RawQueryResult::get(Integer index)
{
    return results.at(index);
}

/*
 * Retrieves the size of the vector (i.e, number of
 * RawResultFromClauses element in the Vector).
 *
 * @return The size of the RawResultFromClauses vector.
 */
Integer RawQueryResult::count()
{
    return results.size();
}

/*
 * Determines when 2 RawQueryResult objects are equal.
 * This method overloads equality operator and defines
 * what it means for 2 such objects to be equal.
 *
 * In our definition, this equality holds if and only
 * if the Vector<RawResultFromClauses> are the same
 * and so on (recursively).
 *
 * @param rawQueryResult Another RawQueryResult object
 * to compare with current instance.
 *
 * @return Boolean, true if equals, false otherwise.
 */
Boolean RawQueryResult::operator==(const RawQueryResult& rawQueryResult) const
{
    return this->results == rawQueryResult.results;
}
