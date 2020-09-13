/**
 * Implementation of the RawResultFromClauses class,
 * and some handy (public) methods.
 */

#include "RawResultFromClauses.h"

/*
 * Constructs a RawResultFromClauses instance,
 * from a Vector<String> as returned directly from the
 * PKB API.
 *
 * @param results A Vector<RawResultFromClause> representing
 * the raw query results of a all clauses in the query,
 * evaluated with respect to a particular synonym.
 *
 * @return A new instance of this class.
 *
 */
RawResultFromClauses::RawResultFromClauses(Vector<RawResultFromClause> results):
    results{results}
{}

/*
 * Checks if the results list is empty.
 *
 * @return True if results list indeed empty,
 * false otherwise.
 */
Boolean RawResultFromClauses::isEmpty()
{
    return results.empty();
}

/*
 * Retrieves a particular RawResultFromClause in the
 * Vector, given an index.
 *
 * @param index The index of the vector, to access.
 *
 * @return The sought RawResultFromClauses.
 */
RawResultFromClause RawResultFromClauses::get(Integer index)
{
    return results.at(index);
}

/*
 * Retrieves the size of the vector (i.e, number of
 * RawResultFromClause element in the Vector).
 *
 * @return The size of the RawResultFromClause vector.
 */
Integer RawResultFromClauses::count()
{
    return results.size();
}

/*
 * Determines when 2 RawResultFromClauses objects are equal.
 * This method overloads equality operator and defines
 * what it means for 2 such objects to be equal.
 *
 * In our definition, this equality holds if and only
 * if the Vector<String> are the same and so on
 * (recursively).
 *
 * @param rawResultFromClauses Another RawResultFromClauses object
 * to compare with current instance.
 *
 * @return Boolean, true if equals, false otherwise.
 */
Boolean RawResultFromClauses::operator==(const RawResultFromClauses& rawResultFromClauses) const
{
   return this->results == rawResultFromClauses.results;
}
