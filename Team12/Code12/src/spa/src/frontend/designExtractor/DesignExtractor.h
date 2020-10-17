/**
 * Concrete API for SPA frontend DesignExtractor.
 * The design extractor traverses tha Abtract Syntax
 * Tree in the Knowledge Base, extracts relationships
 * and stores them in the Knowledge Base and checks
 * for semantic errors.
 */

#ifndef SPA_FRONTEND_DESIGN_EXTRACTOR_H
#define SPA_FRONTEND_DESIGN_EXTRACTOR_H

#include "ast/AstTypes.h"
#include "error/InputError.h"

/**
 * Attempts to extract relationships from an Abstract
 * Syntax Tree and store the relationships in the
 * Program Knowledge Base. If there is a semantic
 * error, false is returned. Otherwise, true.
 *
 * @param rootNode Root node of the AST.
 * @return True, if the extraction is successful.
 *         Otherwise, false.
 */
Boolean extractDesign(ProgramNode& rootNode);

#endif // SPA_FRONTEND_DESIGN_EXTRACTOR_H
