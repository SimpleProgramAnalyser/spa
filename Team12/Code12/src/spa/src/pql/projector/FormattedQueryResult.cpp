/**
 * Implementation of a FormattedQueryResult class, which could
 * be used by higher level classes (e.g, say the UI).
 */

#include "FormattedQueryResult.h"

/*
 * Constructs a FormattedQueryResult instance. This constructor
 * is the only public constructor available for creating a
 * new instance of the object.
 *
 * @param results A String representing the formatted
 * results.
 *
 * @return A new instance of this class.
 *
 */
FormattedQueryResult::FormattedQueryResult(String results):
    results(results)
{}

/*
 * Constructs a RawQueryResult instance. This constructor
 * is a private constructor for creating a new instance of
 * the object, to be used internally (e.g, cneating
 * empty FormattedQueryResult objects).
 *
 * @return A new instance of this class.
 */
FormattedQueryResult::FormattedQueryResult()
{}

/*
 * Returns an instance of an empty FormattedQueryResult object,
 * this method makes use of the private constructor.
 *
 * @return An empty FormattedQueryResult object.
 */
FormattedQueryResult FormattedQueryResult::emptyFormattedQueryResult()
{
    FormattedQueryResult* formattedQueryResult = new FormattedQueryResult();

    return *formattedQueryResult;
}

/*
 * Returns the formatted results stored in the form of a String.
 * This would be the final form of the results, which the UI and
 * other higher-level components would look at. As an example,
 * the results look something like this; 2,3,4 (where these
 * numbers are the statement numbers of the selected statements)
 *
 * @return A String representing the results.
 */
String FormattedQueryResult::getResults()
{
    return results;
}

/*
 * Determines when 2 FormattedueryResult objects are equal.
 * This method overloads equality operator and defines
 * what it means for 2 such objects to be equal.
 *
 * In our definition, this equality holds if and only
 * if the String results, from both
 * objects are equals.
 *
 * @param formattedQueryResult Another FormattedQueryResult object
 * to compare with current instance.
 *
 * @return Boolean, true if equals, false otherwise.
 */
Boolean FormattedQueryResult::operator==(const FormattedQueryResult& formattedQueryResult) const
{
    return this->results == formattedQueryResult.results;
}
