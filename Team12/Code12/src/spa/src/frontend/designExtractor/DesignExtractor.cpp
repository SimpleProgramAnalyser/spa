/**
 * Implementation of SPA frontend design extractor.
 */
#include <iostream>
#include <utility>

#include "FollowsExtractor.h"
#include "ModifiesExtractor.h"
#include "ParentExtractor.h"
#include "SemanticErrorsValidator.h"
#include "NextExtractor.h"
#include "UsesExtractor.h"
#include "CallsExtractor.h"
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
    extractCalls(rootNode, seValidator.adjacencyMatrixOfCalls);

    
    // Build Cfg for each procedure  + extract Next relationships  
    const List<ProcedureNode>* procedureList = &(rootNode.procedureList);
    for (size_t i = 0; i < procedureList->size(); i++) {
        Name procName = procedureList->at(i)->procedureName;
        const StmtlstNode* const stmtListNode = procedureList->at(i)->statementListNode;
        std::pair<CfgNode*, size_t> cfgInfo = buildCfg(stmtListNode);
        // TODO: Insert Cfg root node into PKB

        extractNext(cfgInfo);
    }
}
