/**
 * Identifies Follows relationships.
 */

#ifndef SPA_FRONTEND_FOLLOWS_EXTRACTOR_H
#define SPA_FRONTEND_FOLLOWS_EXTRACTOR_H

#include <ast/AstTypes.h>

/**
 * Stores Follows information in the PKB.
 *
 * @param rootNode Root node of the AST.
 * @return Void.
 */
Void extractFollows(ProgramNode& rootNode);

#endif // SPA_FRONTEND_DESIGN_EXTRACTOR_H
