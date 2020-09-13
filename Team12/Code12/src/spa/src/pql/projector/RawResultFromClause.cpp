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
 * the raw query results of a single clauses in the query
 * (this is the also same raw type returned by the PKB
 * API).
 * @param isClauseRelatedToSynonym representing
 * whether the clause/clause result, is related
 * to the synonym.
 *
 * @return A new instance of this class.
 *
 */
RawResultFromClause::RawResultFromClause(Vector<String> results, Boolean isClauseRelatedToSynonym):
    results{results}, isClauseRelatedToSynonym{isClauseRelatedToSynonym}
{}

/*
 * Constructs a RawResultFromClause instance. This constructor
 * is a private constructor for creating a new instance of
 * the object, to be used internally (e.g, creating
 * empty RawResultFromClause objects).
 *
 * @return A new instance of this class.
 */
RawResultFromClause::RawResultFromClause() {}

/*
 * Returns an instance of an empty RawResultFromClause object,
 * this method makes use of the private constructor.
 *
 * @return An empty RawResultFromClause object.
 */
RawResultFromClause RawResultFromClause::emptyRawResultFromClause()
{
    RawResultFromClause* rawResultFromClause = new RawResultFromClause();

    return *rawResultFromClause;
}

/*
 * A (public) Utility method to convert an integer vector to a string
 * vector, because the constructor (of this class, only accepts
 * Vector<String>)
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
 * Returns true if clause results is indeed related to synonym,
 * false otherwise.
 *
 * Notionally, a clause results is related to the synonym if
 * and only if at least one of its operands
 * has the synonym, for e.g,
 *     statement s1; statement s2;
 *     Select s1 such that Follows(s1, s2);
 *
 * where here it could be observed that synonym
 * s1 is used in Follows relationship, appearing
 * there as one of its operands.
 *
 * @return True if clause results is related to synonym,
 * false otherwise.
 */
Boolean RawResultFromClause::checkIsClauseRelatedToSynonym()
{
    return isClauseRelatedToSynonym;
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
