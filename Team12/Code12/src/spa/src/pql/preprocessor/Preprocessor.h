#ifndef SPA_PQL_PREPROCESSOR_H
#define SPA_PQL_PREPROCESSOR_H

#include "AqTypes.h"

namespace Preprocessor {
AbstractQuery processQuery(String query);
DeclarationTable processDeclarations(const String& declarationString);
ClauseVector processClauses(const String& clausesString, DeclarationTable& declarationTable);
}; // namespace Preprocessor

#endif // SPA_PQL_PREPROCESSOR_H
