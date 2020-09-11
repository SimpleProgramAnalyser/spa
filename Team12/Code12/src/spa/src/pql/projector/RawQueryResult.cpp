#include "pkb/PkbTypes.h"
#include "RawQueryResult.h"

RawQueryResult::RawQueryResult(Vector<Vector<String>> result)
{
}

RawQueryResult::RawQueryResult()
{
}

RawQueryResult RawQueryResult::emptyRawQueryResult()
{
    RawQueryResult* rawQueryResult = new RawQueryResult();

    return *rawQueryResult;
}

Vector<Vector<String>> RawQueryResult::getResults()
{
    return this->results;
}

Boolean RawQueryResult::isEmpty()
{
    return results.empty();
}
