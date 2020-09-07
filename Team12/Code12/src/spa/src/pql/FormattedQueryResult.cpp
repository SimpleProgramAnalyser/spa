#include "FormattedQueryResult.h"

FormattedQueryResult::FormattedQueryResult(String result)
{
    this->result = result;
}

String FormattedQueryResult::getResult()
{
    return this->result;
}
