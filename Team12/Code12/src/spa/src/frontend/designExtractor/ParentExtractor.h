/**
 * Identifies Parent relationships.
 */

#ifndef SPA_FRONTEND_PARENT_EXTRACTOR_H
#define SPA_FRONTEND_PARENT_EXTRACTOR_H

#include <ast/AstTypes.h>

/**
 * Stores Parent information in the PKB.
 *
 * @param rootNode Root node of the AST.
 * @return Void.
 */
Void extractParent(const ProgramNode& rootNode);

#endif // SPA_FRONTEND_PARENT_EXTRACTOR_H
