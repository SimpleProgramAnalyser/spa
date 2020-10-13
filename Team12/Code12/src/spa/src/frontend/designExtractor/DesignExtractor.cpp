/**
 * Implementation of SPA frontend design extractor.
 */
#include <iostream>

#include "FollowsExtractor.h"
#include "ModifiesExtractor.h"
#include "ParentExtractor.h"
#include "SemanticErrorsValidator.h"
#include "UsesExtractor.h"
#include "../src/cfg/CfgBuilder.h"


Void extractDesign(ProgramNode& rootNode)
{
    SemanticErrorsValidator seValidator(rootNode);
    Boolean isSemanticallyValid = seValidator.isProgramValid();

    if (!isSemanticallyValid) {
        // Terminate Program
        throw std::runtime_error("Semantically invalid program!!"); // TODO: more elegant way to handle
    }

    extractFollows(rootNode);
    extractParent(rootNode);
    extractUses(rootNode, seValidator);
    extractModifies(rootNode, seValidator);

    // Build Cfg for each procedure    
    const List<ProcedureNode>* procedureList = &(rootNode.procedureList);
    for (size_t i = 0; i < procedureList->size(); i++) {
        Name procName = procedureList->at(i)->procedureName;
        const StmtlstNode* const stmtListNode = procedureList->at(i)->statementListNode;
        CfgNode* cfgRootNode = buildCfg(stmtListNode);
        // Insert Cfg root node into PKB
    }
}
