/**
 * Identifies Parent relationships.
 */

#ifndef SPA_FRONTEND_USES_EXTRACTOR_H
#define SPA_FRONTEND_USES_EXTRACTOR_H

#include <ast/AstTypes.h>

/**
 * Stores Uses information in the PKB.
 *
 * @param rootNode Root node of the AST.
 * @return Void.
 */
Void extractUses(ProgramNode& rootNode);

#endif // SPA_FRONTEND_USES_EXTRACTOR_H
