/**
 * Implementation of Uses extractor.
 */
#include "UsesExtractor.h"

#include <unordered_set>

#include "pkb/PKB.h"

typedef std::string ProcedureName;
/**
 * VariablesVector represents a set of variables,
 * possibly used by a certain part of the program.
 */
typedef std::unordered_set<std::string> VariablesSet;
/**
 * A map of ProcedureName to a list of variables used by
 * that procedure, in order to find Uses for call statements.
 */
typedef std::unordered_map<ProcedureName, VariablesSet> ProcedureUsesMap;

Void storeVariablesInPkb(StatementNumber stmt, StatementType type, const VariablesSet& variables)
{
    addUsesRelationships(stmt, type, std::vector<std::string>(variables.begin(), variables.end()));
}

Void storeVariablesInPkb(const ProcedureName& proc, const VariablesSet& variables)
{
    addUsesRelationships(proc, std::vector<std::string>(variables.begin(), variables.end()));
}

/**
 * Helper method to join two or three std::vectors. Note
 * that order of the elements is not preserved, but
 * output is guaranteed to hold unique elements.
 *
 * @param v1 The first VariableVector.
 * @param v2 The second VariableVector.
 * @param v3 The third VariableVector.
 * @return A VariableVector containing unique
 *         elements from both v1 and v2.
 */
VariablesSet concatenateVectors(const VariablesSet& v1, const VariablesSet& v2, const VariablesSet& v3 = VariablesSet())
{
    VariablesSet uniqueSet;
    uniqueSet.reserve(v1.size() + v2.size());
    for (const std::string& var1 : v1) {
        uniqueSet.insert(var1);
    }
    for (const std::string& var2 : v2) {
        uniqueSet.insert(var2);
    }
    for (const std::string& var3 : v3) {
        uniqueSet.insert(var3);
    }
    return uniqueSet;
}

VariablesSet extractUsesExpression(const Expression* expr)
{
    // determine expression type
    if (expr->isArithmetic()) {
        // NOLINTNEXTLINE
        const ArithmeticExpression* arithExp = static_cast<const ArithmeticExpression*>(expr);
        VariablesSet leftUses = extractUsesExpression(arithExp->leftFactor);
        VariablesSet rightUses = extractUsesExpression(arithExp->rightFactor);
        return concatenateVectors(leftUses, rightUses);
    } else {
        // NOLINTNEXTLINE
        const ReferenceExpression* refExp = static_cast<const ReferenceExpression*>(expr);
        const BasicDataType* data = refExp->basicData;
        if (data->isConstant()) {
            // constants cannot be used!
            return VariablesSet();
        } else {
            VariablesSet uses;
            uses.reserve(1);
            // NOLINTNEXTLINE
            uses.insert((static_cast<const Variable*>(data))->varName);
            return uses;
        }
    }
}

VariablesSet extractUsesConditionalExpression(const ConditionalExpression* cond)
{
    VariablesSet leftUses;
    VariablesSet rightUses;
    switch (cond->getConditionalType()) {
    case NotConditionalExpression:
        // NOLINTNEXTLINE
        return extractUsesConditionalExpression(static_cast<const NotExpression*>(cond)->expression);
    case AndConditionalExpression: {
        // NOLINTNEXTLINE
        const AndExpression* andExpr = static_cast<const AndExpression*>(cond);
        leftUses = extractUsesConditionalExpression(andExpr->leftExpression);
        rightUses = extractUsesConditionalExpression(andExpr->rightExpression);
        break;
    }
    case OrConditionalExpression: {
        // NOLINTNEXTLINE
        const OrExpression* orExpr = static_cast<const OrExpression*>(cond);
        leftUses = extractUsesConditionalExpression(orExpr->leftExpression);
        rightUses = extractUsesConditionalExpression(orExpr->rightExpression);
        break;
    }
    case RelationalConditionalExpression: {
        // NOLINTNEXTLINE
        const RelationalExpression* relExpr = static_cast<const RelationalExpression*>(cond);
        leftUses = extractUsesExpression(relExpr->leftFactor);
        rightUses = extractUsesExpression(relExpr->rightFactor);
        break;
    }
    default:
        throw std::runtime_error("Unknown expression type in UsesExtractor extractUsesConditionalExpression");
    }
    return concatenateVectors(leftUses, rightUses);
}

/**
 * Returns all the variables used in the statement list
 */
VariablesSet extractUsesStmtlst(const StmtlstNode* stmtLstNode, ProcedureUsesMap* procedureUses)
{
    size_t numberOfStatements = stmtLstNode->statementList.size();
    VariablesSet allVariablesUsed;
    for (size_t i = 0; i < numberOfStatements; i++) {
        const std::unique_ptr<StatementNode>& currentStatement = stmtLstNode->statementList.at(i);
        VariablesSet usedInStatement;
        StatementType currentStatementType = currentStatement->getStatementType();
        switch (currentStatementType) {
        case AssignmentStatement: {
            // NOLINTNEXTLINE
            const auto* assign = static_cast<const AssignmentStatementNode*>(currentStatement.get());
            usedInStatement = extractUsesExpression(assign->expression);
            break;
        }
        case CallStatement: {
            // NOLINTNEXTLINE
            const auto* call = static_cast<const CallStatementNode*>(currentStatement.get());
            usedInStatement = procedureUses->at(call->procedureName);
            break;
        }
        case IfStatement: {
            // NOLINTNEXTLINE
            const auto* ifStatement = static_cast<const IfStatementNode*>(currentStatement.get());
            VariablesSet usedInConditional = extractUsesConditionalExpression(ifStatement->predicate);
            VariablesSet usedInIf = extractUsesStmtlst(ifStatement->ifStatementList, procedureUses);
            VariablesSet usedInElse = extractUsesStmtlst(ifStatement->elseStatementList, procedureUses);
            usedInStatement = concatenateVectors(usedInConditional, usedInIf, usedInElse);
            break;
        }
        case PrintStatement: {
            // NOLINTNEXTLINE
            usedInStatement.insert(static_cast<const PrintStatementNode*>(currentStatement.get())->var.varName);
            break;
        }
        case ReadStatement: {
            // ReadStatement does not use any variables
            // do nothing
            break;
        }
        case WhileStatement: {
            // NOLINTNEXTLINE
            const auto* whileStatement = static_cast<const WhileStatementNode*>(currentStatement.get());
            VariablesSet usedInConditional = extractUsesConditionalExpression(whileStatement->predicate);
            VariablesSet usedInWhileList = extractUsesStmtlst(whileStatement->statementList, procedureUses);
            usedInStatement = concatenateVectors(usedInConditional, usedInWhileList);
            break;
        }
        default:
            throw std::runtime_error("Unknown statement type in UsesExtractor extractUsesStmtlst");
        }
        // update PKB
        storeVariablesInPkb(currentStatement->getStatementNumber(), currentStatementType, usedInStatement);
        // associate variables with the statement list as well
        allVariablesUsed = concatenateVectors(allVariablesUsed, usedInStatement);
    }
    return allVariablesUsed;
}

/**
 * Extracts the Uses relationships in the SIMPLE program
 * Abstract Syntax Tree, and returns the map of procedures
 * to used variables.
 *
 * @param rootNode Root node of the Abstract Syntax Tree.
 * @param callOrder The order to run the extractor. Procedures
 *                  that call other procedures have to be
 *                  run last to populate the procedure uses
 *                  map. If not, the correct Uses relationships
 *                  cannot be determined for Call statements.
 * @return Map of procedure names to used variables.
 */
ProcedureUsesMap extractUsesReturnMap(ProgramNode& rootNode, const std::vector<int>& callOrder)
{
    // set up procedureUses map
    ProcedureUsesMap procedureUses;
    // process the procedures in the sorted order
    for (int procIndex : callOrder) {
        const std::unique_ptr<ProcedureNode>& currentProcedure = rootNode.procedureList.at(procIndex);
        VariablesSet variablesUsedInCurrentProcedure
            = extractUsesStmtlst(currentProcedure->statementListNode, &procedureUses);
        // update PKB
        storeVariablesInPkb(currentProcedure->procedureName, variablesUsedInCurrentProcedure);
        // store Uses relationship in hash map, for Call statements
        procedureUses.insert(
            std::pair<ProcedureName, VariablesSet>(currentProcedure->procedureName, variablesUsedInCurrentProcedure));
    }
    return procedureUses;
}

Void extractUses(ProgramNode& rootNode, SemanticErrorsValidator& sev)
{
    // determine order to extract Uses with topological sort
    extractUsesReturnMap(rootNode, sev.reverseTopologicalSort());
}
