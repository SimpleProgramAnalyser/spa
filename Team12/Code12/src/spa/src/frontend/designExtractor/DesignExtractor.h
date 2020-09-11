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

Void extractDesign(ProgramNode& rootNode);

#endif // SPA_FRONTEND_DESIGN_EXTRACTOR_H
