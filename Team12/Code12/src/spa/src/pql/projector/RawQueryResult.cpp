/**
 * Implementation of the RawQueryResult class,
 * and some handy (public) methods.
 */

#include "pkb/PkbTypes.h"
#include "RawQueryResult.h"

/*
 * Constructs a RawQueryResult instance. This constructor
 * is the only public constructor available for creating a
 * new instance of the object.
 *
 * @param results A list of list of strings, each Vector<String>
 * element in the outer list represents the result of a single
 * synonym (in the PQL 'select').
 *
 * @return A new instance of this class.
 *
 */
RawQueryResult::RawQueryResult(Vector<Vector<String>> results):
    results(results)
{}

/*
 * Constructs a RawQueryResult instance. This constructor
 * is a private constructor for creating a new instance of
 * the object, to be used internally (e.g, creating
 * empty RawQueryResult objects).
 *
 * @return A new instance of this class.
 */
RawQueryResult::RawQueryResult()
{}

/*
 * Returns an instance of an empty RawQueryResult object,
 * this method makes use of the private constructor.
 *
 * @return An empty RawQueryResult object.
 */
RawQueryResult RawQueryResult::emptyRawQueryResult()
{
    RawQueryResult* rawQueryResult = new RawQueryResult();

    return *rawQueryResult;
}

/*
 * Returns the results stored in the form of Vector<Vector<String>>.
 *
 * @return A Vector<Vector<String>> representing the results.
 */
Vector<Vector<String>> RawQueryResult::getResults()
{
    return results;
}

/*
 * Checks if the results list is empty, there are 2
 * scenarios this could happen. Either, the
 * query is invalid or there are indeed no results
 * from query.
 *
 * @return True if results list indeed empty,
 * false otherwise.
 */
Boolean RawQueryResult::isEmpty()
{
    return results.empty();
}

/*bool operator== (RawQueryResult& a, RawQueryResult & b)
{
    return (a.results == b.results);
}*/
