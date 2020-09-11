/**
 * Identifies Modifies relationships.
 */

#ifndef SPA_FRONTEND_MODIFIES_EXTRACTOR_H
#define SPA_FRONTEND_MODIFIES_EXTRACTOR_H

#include <ast/AstTypes.h>

/**
 * Stores Modifies information in the PKB.
 *
 * @param rootNode Root node of the AST.
 * @return Void.
 */
Void extractModifies(ProgramNode& rootNode);

#endif // SPA_FRONTEND_MODIFIES_EXTRACTOR_H
