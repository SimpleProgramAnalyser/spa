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

Void unifyLeftKnown(const Reference& leftRef, const Reference& rightRef, ResultsTable* resultsTable)
{
    ReferenceType typeOfLiteral = leftRef.getReferenceType();
    ReferenceType typeOfVariable = rightRef.getReferenceType();
    Vector<String> resultsForVariable;
    SynonymTypeToClosureMap synonymTypeMap = getSynonymTypeMap();
    AttributeTypeToClosureMap attributeTypeMap = getAttributeTypeMap();
    if (typeOfVariable == AttributeRefType) {
        resultsForVariable
            = attributeTypeMap[rightRef.getDesignEntity().getType()][rightRef.getAttribute().getType()]();
    } else {
        // typeOfVariable == SynonymRefType
        resultsForVariable = synonymTypeMap[rightRef.getDesignEntity().getType()]();
    }
    std::function<bool(const String&, const String&)> comparisonFunction = [](const String& str1, const String& str2) {
        return str1 == str2;
    };
    if (typeOfLiteral == IntegerRefType) {
        comparisonFunction = [](const String& intStr1, const String& intStr2) {
            return std::stoi(intStr1) == std::stoi(intStr2);
        };
    }
    Vector<String> matchingResults;
    String rawLiteral = leftRef.getValue();
    for (const String& str : resultsForVariable) {
        // try to find a substitution that matches
        if (comparisonFunction(str, rawLiteral)) {
            matchingResults.push_back(rawLiteral);
            break;
        }
    }
    resultsTable->storeResultsOne(rightRef.getValue(), matchingResults);
}

Void unifyRightKnown(const Reference& leftRef, const Reference& rightRef, ResultsTable* resultsTable)
{
    // "with ... = ..." is an associative relation, so we just swap the terms
    unifyLeftKnown(rightRef, leftRef, resultsTable);
}

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

Void unifyBothAny(const Reference& leftRef, const Reference& rightRef, ResultsTable* resultsTable)
{
    if (leftRef == rightRef) {
        resultsTable->storeResultsZero(true);
    }
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
