/**
 * Implementation of the RawResultFromClauses class,
 * and some handy (public) methods.
 */

#include "RawResultFromClauses.h"

/*
 * Constructs a RawResultFromClauses instance,
 * from a Vector<RawResultFromClauses> 
 * PKB API.
 *
 * Impt Note: This constructor tries to merge similar
 * adjacent RawResultFromClause (entries), hence
 * it (this constructor) only accepts a results
 * (Vector<RawResultFromClause>) vector, where
 * all RawResultFromClause elements are directly
 * related to the synonym (i.e, isClauseRelatedToSynonym
 * is true).
 *
 * @param results A Vector<RawResultFromClause> representing
 * the raw query results of a all clauses in the query,
 * evaluated with respect to a particular synonym.
 *
 * @return A new instance of this class.
 *
 */
RawResultFromClauses::RawResultFromClauses(Vector<RawResultFromClause> results)
{
    Vector<String> mergedResults;

    /*
     * We maintain an unordered_map to keep track
     * of unique items.
     */
    std::unordered_map<String, String> table;

    Integer len1 = results.size();

    for (int i = 0; i < len1; ++i) {
        RawResultFromClause temp = results.at(i);


        Integer len2 = temp.count();

        for (int j = 0; j < len2; ++j) {
            if (!temp.checkIsClauseRelatedToSynonym()) {
                /*
                 * Throw an assert run-time error, please read
                 * documentation for this class, to understand
                 * why we are checking for this.
                 */
            }

            String str = temp.get(j);

            // If str not added before, add it, else ignore it.
            std::unordered_map<String, String>::const_iterator got = table.find(str);
            if (got == table.end()) {
                table.insert({str, str});
                mergedResults.push_back(str);
            }
        }
    }

    this->results =  mergedResults;
}

/*
 * Constructs a RawResultFromClauses instance. This constructor
 * is a private constructor for creating a new instance of
 * the object, to be used internally (e.g, creating
 * empty RawResultFromClauses objects).
 *
 * @return A new instance of this class.
 */
RawResultFromClauses::RawResultFromClauses() {}

/*
 * Returns an instance of an empty RawResultFromClauses object,
 * this method makes use of the private constructor.
 *
 * @return An empty RawResultFromClauses object.
 */
RawResultFromClauses RawResultFromClauses::emptyRawResultFromClauses()
{
    RawResultFromClauses* rawResultFromClauses = new RawResultFromClauses();

    return *rawResultFromClauses;
}

/*
 * A utility method for converting a the current RawResultFromClauses
 * (instance) to a string.
 *
 * This string would be delimted (depending on delimeter passed in),
 * where each element corresponds to an element in the vector.
 *
 * @param Delimeter The character to delimit results by.
 *
 * @return String a comma delimited string
 */
String RawResultFromClauses::toString(String delimeter)
{
    Integer len = this->count();

    String str;

    for (int i = 0; i < len; ++i) {
        str += results.at(i);

        if (i < len - 1) {
            str += delimeter;
        }
    }

    return str;
}

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
 * Retrieves a particular String in the
 * Vector, given an index.
 *
 * @param index The index of the vector, to access.
 *
 * @return The sought RawResultFromClauses
 * (string form).
 */
String RawResultFromClauses::get(Integer index)
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
