/**
 * Implementation of Modifies extractor.
 */
#include <stdexcept>
#include <unordered_set>

#include "ModifiesExtractor.h"
#include "pkb/PKB.h"

typedef std::string ProcedureName;
/**
 * VariablesVector represents a set of variables,
 * possibly modified by a certain part of the program.
 */
typedef std::unordered_set<std::string> VariablesSet;
/**
 * A map of ProcedureName to a list of variables modified by
 * that procedure, in order to find Modifies for call statements.
 */
typedef std::unordered_map<ProcedureName, VariablesSet> ProcedureModifiesMap;

Void storeVariablesInPkb(StatementNumber stmt, StatementType type, const VariablesSet& variables)
{
    addModifiesRelationships(stmt, type, std::vector<std::string>(variables.begin(), variables.end()));
}

Void storeVariablesInPkb(const ProcedureName& proc, const VariablesSet& variables)
{
    addModifiesRelationships(proc, std::vector<std::string>(variables.begin(), variables.end()));
}

/**
 * Helper method to join two std::vectors. Note
 * that order of the elements is not preserved, but
 * output is guaranteed to hold unique elements.
 *
 * @param v1 The first VariableVector.
 * @param v2 The second VariableVector.
 * @return A VariableVector containing unique
 *         elements from both v1 and v2.
 */
VariablesSet concatenateVectors(const VariablesSet& v1, const VariablesSet& v2)
{
    VariablesSet uniqueSet;
    uniqueSet.reserve(v1.size() + v2.size());
    for (const std::string& var1 : v1) {
        uniqueSet.insert(var1);
    }
    for (const std::string& var2 : v2) {
        uniqueSet.insert(var2);
    }
    return uniqueSet;
}

/**
 * Returns all the variables modified in the statement list
 */
VariablesSet extractModifiesStmtlst(const StmtlstNode* stmtLstNode, ProcedureModifiesMap* procedureModifies)
{
    size_t numberOfStatements = stmtLstNode->statementList.size();
    VariablesSet allVariablesModified;
    for (size_t i = 0; i < numberOfStatements; i++) {
        const std::unique_ptr<StatementNode>& currentStatement = stmtLstNode->statementList.at(i);
        VariablesSet modifiedInStatement;
        StatementType currentStatementType = currentStatement->getStatementType();
        switch (currentStatementType) {
        case AssignmentStatement: {
            modifiedInStatement.insert(
                static_cast<const AssignmentStatementNode*>(currentStatement.get())->variable.varName);
            break;
        }
        case CallStatement: {
            // NOLINTNEXTLINE
            const auto* call = static_cast<const CallStatementNode*>(currentStatement.get());
            modifiedInStatement = procedureModifies->at(call->procedureName);
            break;
        }
        case IfStatement: {
            // NOLINTNEXTLINE
            const auto* ifStatement = static_cast<const IfStatementNode*>(currentStatement.get());
            VariablesSet modifiedInIf = extractModifiesStmtlst(ifStatement->ifStatementList, procedureModifies);
            VariablesSet modifiedInElse = extractModifiesStmtlst(ifStatement->elseStatementList, procedureModifies);
            modifiedInStatement = concatenateVectors(modifiedInIf, modifiedInElse);
            break;
        }
        case PrintStatement: {
            // PrintStatement does not modify any variables
            // do nothing
            break;
        }
        case ReadStatement: {
            // NOLINTNEXTLINE
            modifiedInStatement.insert(static_cast<const ReadStatementNode*>(currentStatement.get())->var.varName);
            break;
        }
        case WhileStatement: {
            // NOLINTNEXTLINE
            const auto* whileStatement = static_cast<const WhileStatementNode*>(currentStatement.get());
            VariablesSet modifiedInWhileList = extractModifiesStmtlst(whileStatement->statementList, procedureModifies);
            modifiedInStatement = modifiedInWhileList;
            break;
        }
        default:
            throw std::runtime_error("Unknown statement type in ModifiesExtractor extractModifiesStmtlst");
        }
        // update PKB
        storeVariablesInPkb(currentStatement->getStatementNumber(), currentStatementType, modifiedInStatement);
        // associate variables with the statement list as well
        allVariablesModified = concatenateVectors(allVariablesModified, modifiedInStatement);
    }
    return allVariablesModified;
}

/**
 * Extracts the Modifies relationships in the SIMPLE program
 * Abstract Syntax Tree, and returns the map of procedures
 * to modified variables.
 *
 * @param rootNode Root node of the Abstract Syntax Tree.
 * @param callOrder The order to run the extractor. Procedures
 *                  that call other procedures have to be
 *                  run last to populate the procedure modifies
 *                  map. If not, the correct Modifies relationships
 *                  cannot be determined for Call statements.
 * @return Map of procedure names to modified variables.
 */
ProcedureModifiesMap extractModifiesReturnMap(ProgramNode& rootNode, const std::vector<int>& callOrder)
{
    // set up procedureModifies map
    ProcedureModifiesMap procedureModifies;
    // process the procedures in the sorted order
    for (int procIndex : callOrder) {
        const std::unique_ptr<ProcedureNode>& currentProcedure = rootNode.procedureList.at(procIndex);
        VariablesSet variablesModfiedInCurrentProcedure
            = extractModifiesStmtlst(currentProcedure->statementListNode, &procedureModifies);
        // update PKB
        storeVariablesInPkb(currentProcedure->procedureName, variablesModfiedInCurrentProcedure);

        // store Modifies relationship in hash map, for Call statements in other procedures to know 
        // which variables are modified in this procedure
        procedureModifies.insert(std::pair<ProcedureName, VariablesSet>(currentProcedure->procedureName,
                                                                       variablesModfiedInCurrentProcedure));
    }
    // return for unit testing
    return procedureModifies;
}

Void extractModifies(ProgramNode& rootNode, SemanticErrorsValidator& sev)
{
    // determine order to extract Modifies with topological sort
    extractModifiesReturnMap(rootNode, sev.reverseTopologicalSort());
}
