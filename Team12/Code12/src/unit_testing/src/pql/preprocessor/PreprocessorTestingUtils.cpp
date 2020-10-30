/**
 * Implementation of helper methods for testing of preprocessor.
 */

#include "PreprocessorTestingUtils.h"

AbstractQuery processQuery(const String& queryString)
{
    return Preprocessor::processQuery(queryString);
}
