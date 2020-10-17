/**
 * Implementation of WithUnifier for Query Evaluator.
 * WithUnifier performs a unification to obtain results.x`
 */

#include "WithUnifier.h"

#include <functional>

#include "pkb/PKB.h"
#include "pql/evaluator/relationships/RelationshipsUtil.h"

std::function<std::vector<std::string>()> createGetStatementsFunction(StatementType stmtType)
{
    return [stmtType]() {
        return convertToClauseResult(getAllStatements(stmtType));
    };
}

typedef std::unordered_map<DesignEntityType,
                           std::unordered_map<AttributeType, std::function<std::vector<std::string>()>>>
    AttributeTypeToClosureMap;

typedef std::unordered_map<DesignEntityType, std::function<std::vector<std::string>()>> SynonymTypeToClosureMap;

AttributeTypeToClosureMap getAttributeTypeMap()
{
    return {{StmtType, {{StmtNumberType, createGetStatementsFunction(AnyStatement)}}},
            {ReadType,
             {{StmtNumberType, createGetStatementsFunction(ReadStatement)},
              {VarNameType,
               []() {
                   return getAllModifiesVariablesFromStatementType(ReadStatement);
               }}}},
            {PrintType,
             {{StmtNumberType, createGetStatementsFunction(PrintStatement)},
              {VarNameType,
               []() {
                   return getAllUsesVariablesFromStatementType(PrintStatement);
               }}}},
            {CallType,
             {{StmtNumberType, createGetStatementsFunction(CallStatement)}, {ProcNameType, getAllProceduresCalled}}},
            {WhileType, {{StmtNumberType, createGetStatementsFunction(WhileStatement)}}},
            {IfType, {{StmtNumberType, createGetStatementsFunction(IfStatement)}}},
            {AssignType, {{StmtNumberType, createGetStatementsFunction(AssignmentStatement)}}},
            {VariableType, {{VarNameType, getAllVariables}}},
            {ConstantType,
             {{ValueType,
               []() {
                   return convertToClauseResult(getAllConstants());
               }}}},
            {ProcedureType, {{ProcNameType, getAllProcedures}}}};
}

SynonymTypeToClosureMap getSynonymTypeMap()
{
    return {{StmtType, createGetStatementsFunction(AnyStatement)},
            {ReadType, createGetStatementsFunction(ReadStatement)},
            {PrintType, createGetStatementsFunction(PrintStatement)},
            {CallType, createGetStatementsFunction(CallStatement)},
            {WhileType, createGetStatementsFunction(WhileStatement)},
            {IfType, createGetStatementsFunction(IfStatement)},
            {AssignType, createGetStatementsFunction(AssignmentStatement)},
            {VariableType, getAllVariables},
            {ConstantType,
             []() {
                 return convertToClauseResult(getAllConstants());
             }},
            {ProcedureType, getAllProcedures}};
}

SynonymTypeToClosureMap synonymTypeMap = getSynonymTypeMap();
AttributeTypeToClosureMap attributeTypeMap = getAttributeTypeMap();

/**
 * Given a reference of either AttributeRefType or SynonymRefType,
 * gets all results that match this reference from the Program
 * Knowledge Base, with the help of hash maps.
 *
 * @param ref Reference to retrieve results for.
 * @return List of results matching this reference.
 */
Vector<String> retrieveResultsForVariableReference(const Reference& ref)
{
    if (ref.getReferenceType() == AttributeRefType) {
        return attributeTypeMap[ref.getDesignEntity().getType()][ref.getAttribute().getType()]();
    } else {
        // leftRefType == SynonymRefType
        return synonymTypeMap[ref.getDesignEntity().getType()]();
    }
}

/**
 * Unifies an expression where the left expression is
 * a literal and the right expression is a variable.
 *
 * @param literalRef Reference of a String or Integer literal.
 * @param varRef Reference of a Synonym or Synonym.Attribute.
 * @param resultsTable ResultsTable to store the results.
 */
Void unifyLeftKnown(const Reference& literalRef, const Reference& varRef, ResultsTable* resultsTable)
{
    ReferenceType typeOfLiteral = literalRef.getReferenceType();
    Vector<String> resultsForVariable = retrieveResultsForVariableReference(varRef);
    std::function<bool(const String&, const String&)> comparisonFunction = [](const String& str1, const String& str2) {
        return str1 == str2;
    };
    if (typeOfLiteral == IntegerRefType) {
        comparisonFunction = [](const String& intStr1, const String& intStr2) {
            return std::stoi(intStr1) == std::stoi(intStr2);
        };
    }
    ClauseResult matchingResults;
    String rawLiteral = literalRef.getValue();
    for (const String& str : resultsForVariable) {
        // try to find a substitution that matches
        if (comparisonFunction(str, rawLiteral)) {
            matchingResults.push_back(str);
            break;
        }
    }
    resultsTable->storeResultsOne(varRef.getValue(), matchingResults);
}

/**
 * Unifies an expression where the left expression is
 * a variable and the right expression is a literal.
 *
 * @param varRef Reference of a Synonym or Synonym.Attribute.
 * @param literalRef Reference of a String or Integer literal.
 * @param resultsTable ResultsTable to store the results.
 */
Void unifyRightKnown(const Reference& varRef, const Reference& literalRef, ResultsTable* resultsTable)
{
    // "with ... = ..." is an associative relation, so we just swap the terms
    unifyLeftKnown(literalRef, varRef, resultsTable);
}

/**
 * Unifies an expression where both expressions
 * are String or Integer literals.
 *
 * @param leftRef The left reference in the equality.
 * @param rightRef The right reference in the equality.
 * @param resultsTable ResultsTable to store the results.
 */
Void unifyBothKnown(const Reference& leftRef, const Reference& rightRef, ResultsTable* resultsTable)
{
    bool unificationSuccessful = false;
    if (leftRef == rightRef) {
        unificationSuccessful = true;
    } else if (leftRef.getReferenceType() == IntegerRefType && rightRef.getReferenceType() == IntegerRefType) {
        unificationSuccessful = std::stoi(leftRef.getValue()) == std::stoi(rightRef.getValue());
    }
    resultsTable->storeResultsZero(unificationSuccessful);
}

/**
 * Unifies an expression where both expressions
 * are Synonym or Synonym.Attributes.
 *
 * @param leftRef The left reference in the equality.
 * @param rightRef The right reference in the equality.
 * @param resultsTable ResultsTable to store the results.
 */
Void unifyBothAny(const Reference& leftRef, const Reference& rightRef, ResultsTable* resultsTable)
{
    if (leftRef == rightRef) {
        // all subtitutions work
        resultsTable->storeResultsZero(true);
    }
    Vector<String> resultsForLeft = retrieveResultsForVariableReference(leftRef);
    Vector<String> resultsForRight = retrieveResultsForVariableReference(rightRef);
    PairedResult matchingResults;
    for (const String& leftRes : resultsForLeft) {
        for (const String& rightRes : resultsForRight) {
            // find subtitutions that work and add them to results table
            if (leftRes == rightRes) {
                matchingResults.emplace_back(leftRes, rightRes);
                break;
            }
        }
    }
    resultsTable->storeResultsTwo(leftRef.getValue(), rightRef.getValue(), matchingResults);
}

Void evaluateWith(WithClause* whClause, ResultsTable* resultsTable)
{
    const Reference& leftRef = whClause->getLeftReference();
    const Reference& rightRef = whClause->getRightReference();
    ReferenceType leftRefType = leftRef.getReferenceType();
    ReferenceType rightRefType = rightRef.getReferenceType();
    // assume leftRef and rightRef are not wildcard,
    void (*unifyFunction)(const Reference&, const Reference&, ResultsTable*)
        = canMatchMultiple(leftRefType) ? (canMatchMultiple(rightRefType) ? unifyBothAny : unifyRightKnown)
                                        : (canMatchMultiple(rightRefType) ? unifyLeftKnown : unifyBothKnown);
    unifyFunction(leftRef, rightRef, resultsTable);
}
