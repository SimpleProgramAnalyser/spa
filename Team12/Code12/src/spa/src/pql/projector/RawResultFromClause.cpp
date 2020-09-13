/**
 * Implementation of the RawResultFromClause class,
 * and some handy (public) methods.
 */

#include "RawResultFromClause.h"

/*
 * Constructs a RawResultFromClause instance,
 * from a Vector<String> as returned directly from the
 * PKB API.
 *
 * @param results A Vector<String> representing
 * the raw query results of a single clauses in the query,
 * evaluated with respect to a particular synonym.
 *
 * @return A new instance of this class.
 *
 */
RawResultFromClause::RawResultFromClause(Vector<String> results, Boolean isClauseRelatedToSynonym):
    results{results}, isClauseRelatedToSynonym{isClauseRelatedToSynonym}
{}

/*
 * A (private) Utility method to convert an integer vector to a string
 * vector.
 *
 * @param intList An integer vector to convert.
 *
 * @return Vector<String> the converted vector.
 */
Vector<String> RawResultFromClause::convertToStringVect(Vector<Integer> intList)
{
    Vector<String> strList;

    Integer len = intList.size();

    for (int i = 0; i < len; ++i) {
        int elem = intList.at(i);

        String str = std::to_string(elem);

        strList.push_back(str);
    }

    return strList;
}

/*
 * Constructs a RawResultFromClause instance,
 * from a Vector<Integer> as returned directly from the
 * PKB API.
 *
 * @param results A Vector<Integer> representing
 * the raw query results of a single clauses in the query,
 * evaluated with respect to a particular synonym.
 *
 * The constructor, will convert the Vector<Integer>
 * tot Vector<String> for internal representation/
 * storage consistency.
 *
 * @return A new instance of this class.
 *
 */
RawResultFromClause::RawResultFromClause(Vector<Integer> results, Boolean isClauseRelatedToSynonym)
{
    this->isClauseRelatedToSynonym = isClauseRelatedToSynonym;
    this->results = convertToStringVect(results);
}

/*
 * Checks if the results list is empty.
 *
 * @return True if results list indeed empty,
 * false otherwise.
 */
Boolean RawResultFromClause::isEmpty()
{
    return results.empty();
}

/*
 * Retrieves a particular String in the
 * Vector, given an index.
 *
 * @param index The index of the vector, to access.
 *
 * @return The sought RawResultFromClauses.
 */
String RawResultFromClause::get(Integer index)
{
    return results.at(index);
}

/*
 * Retrieves the size of the vector (i.e, number of
 * String  element in the Vector).
 *
 * @return The size of the String vector.
 */
Integer RawResultFromClause::count()
{
    return results.size();
}

/*
 * Determines when 2 RawResultFromClause objects are equal.
 * This method overloads equality operator and defines
 * what it means for 2 such objects to be equal.
 *
 * In our definition, this equality holds if and only
 * if the Vector<String> are the same and so on
 * (recursively).
 *
 * @param rawResultFromClause Another RawResultFromClause object
 * to compare with current instance.
 *
 * @return Boolean, true if equals, false otherwise.
 */
Boolean RawResultFromClause::operator==(const RawResultFromClause& rawResultFromClause) const
{
   return this->results == rawResultFromClause.results;
}
