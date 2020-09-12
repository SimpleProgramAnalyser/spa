/**
 * Identifies Parent relationships.
 */

#ifndef SPA_FRONTEND_USES_EXTRACTOR_H
#define SPA_FRONTEND_USES_EXTRACTOR_H

#include "SemanticErrorsValidator.h"
#include "ast/AstTypes.h"

/**
 * Stores Uses information in the PKB.
 *
 * @param rootNode Root node of the AST.
 * @param sev The SemanticErrorsValidator
 *            used by Design Extractor.
 * @return Void.
 */
Void extractUses(ProgramNode& rootNode, SemanticErrorsValidator& sev);

#endif // SPA_FRONTEND_USES_EXTRACTOR_H
