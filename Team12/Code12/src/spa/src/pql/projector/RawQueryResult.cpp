/**
 * Implementation of the RawQueryResult class,
 * and some handy (public) methods.
 */

#include "RawQueryResult.h"

#include "pkb/PkbTypes.h"

/*
 * Constructs a RawQueryResult instance. This constructor
 * is the only public constructor available for creating a
 * new instance of the object.
 *
 * @param results A Vector<Vector<Vector<String>>>, where each
 * Vector<Vector<String>> element represents the result of a single
 * synonym (in the PQL 'select'), evaluated with respect to (all) the
 * clauses in the query. And within the Vector<Vector<String>>,
 * each Vector<String> represents the result of a single synonym
 * evaluated with respect to (only) 1 particular clause (in the query).
 *
 * @return A new instance of this class.
 *
 */
RawQueryResult::RawQueryResult(Vector<Vector<Vector<String>>> results): results(results) {}

/*
 * Constructs a RawQueryResult instance. This constructor
 * is a private constructor for creating a new instance of
 * the object, to be used internally (e.g, creating
 * empty RawQueryResult objects).
 *
 * @return A new instance of this class.
 */
RawQueryResult::RawQueryResult() {}

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
Vector<Vector<Vector<String>>> RawQueryResult::getResults()
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

/*
 * Determines when 2 RawQueryResult objects are equal.
 * This method overloads equality operator and defines
 * what it means for 2 such objects to be equal.
 *
 * In our definition, this equality holds if and only
 * if the Vector<Vector<Vector<String>>> results, from both
 * objects are equals.
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
