/**
 * Implementation of WithUnifier for Query Evaluator.
 * WithUnifier performs a unification to obtain results.x`
 */

#include "WithUnifier.h"

#include <functional>

#include "pkb/PKB.h"
#include "pql/evaluator/relationships/RelationshipsUtil.h"

/**
 * Helper class to associate the default result of a
 * synonym with the result of a certain attribute
 */
class WithPair {
public:
    String synonymResult;
    String attributeResult;
    WithPair(String synonym, String attribute): synonymResult(std::move(synonym)), attributeResult(std::move(attribute))
    {}

    explicit WithPair(const Pair<String, String>& synAttrPair):
        synonymResult(synAttrPair.first), attributeResult(synAttrPair.second)
    {}
};

/**
 * Given raw results from the PKB, convert them into WithPairs
 * where the attribute result is the same as the synonym result.
 *
 * @param numericalResults Raw results {r1, r2, ..., rn} from PKB.
 * @return WithPairs of (rn, rn) where rn == rn and rn is a string.
 */
std::vector<WithPair> convertToWithPairs(const Vector<Integer>& numericalResults)
{
    std::vector<WithPair> pairs;
    for (Integer res : numericalResults) {
        std::string resStr = std::to_string(res);
        pairs.emplace_back(resStr, resStr);
    }
    return pairs;
}

/**
 * Given raw results from the PKB, convert them into WithPairs
 * where the attribute result is the same as the synonym result.
 *
 * @param stringResults Raw results {r1, r2, ..., rn} from PKB.
 * @return WithPairs of (rn, rn) where rn == rn.
 */
std::vector<WithPair> convertToWithPairs(const Vector<String>& stringResults)
{
    std::vector<WithPair> pairs;
    for (const String& res : stringResults) {
        pairs.emplace_back(res, res);
    }
    return pairs;
}

/**
 * Given raw results from the PKB, convert them into WithPairs.
 *
 * @param pairedResult Raw results {(s1, r1), (s2, r2), ..., (sn, rn)} from PKB.
 * @return WithPairs of (sn, rn) where sn is a string.
 */
std::vector<WithPair> convertToWithPairs(const Vector<Pair<Integer, String>>& pairedResult)
{
    std::vector<WithPair> pairs;
    for (const Pair<Integer, String>& res : pairedResult) {
        pairs.emplace_back(std::to_string(res.first), res.second);
    }
    return pairs;
}

/**
 * Given raw results from the PKB, convert them into WithPairs.
 *
 * @param pairedResult Raw results {(s1, r1), (s2, r2), ..., (sn, rn)} from PKB.
 * @return WithPairs of (sn, rn).
 */
std::vector<WithPair> convertToWithPairs(const Vector<Pair<String, String>>& pairedResult)
{
    std::vector<WithPair> pairs;
    for (const Pair<String, String>& res : pairedResult) {
        pairs.emplace_back(res);
    }
    return pairs;
}

std::function<std::vector<WithPair>()> createGetStatementsFunction(StatementType stmtType)
{
    return [stmtType]() {
        return convertToWithPairs(getAllStatements(stmtType));
    };
}

typedef std::unordered_map<DesignEntityType, std::unordered_map<AttributeType, std::function<std::vector<WithPair>()>>>
    AttributeTypeToClosureMap;

typedef std::unordered_map<DesignEntityType, std::function<std::vector<WithPair>()>> SynonymTypeToClosureMap;

AttributeTypeToClosureMap getAttributeTypeMap()
{
    return {{StmtType, {{StmtNumberType, createGetStatementsFunction(AnyStatement)}}},
            {ReadType,
             {{StmtNumberType, createGetStatementsFunction(ReadStatement)},
              {VarNameType,
               []() {
                   return convertToWithPairs(getAllModifiesStatementTuple(ReadStatement));
               }}}},
            {PrintType,
             {{StmtNumberType, createGetStatementsFunction(PrintStatement)},
              {VarNameType,
               []() {
                   return convertToWithPairs(getAllUsesStatementTuple(PrintStatement));
               }}}},
            {CallType,
             {{StmtNumberType, createGetStatementsFunction(CallStatement)},
              {ProcNameType,
               []() {
                   // TODO: Use PKB direct method
                   std::vector<WithPair> pairs;
                   Vector<Integer> allCallStatements = getAllStatements(CallStatement);
                   for (Integer call : allCallStatements) {
                       pairs.emplace_back(std::to_string(call), getProcedureCalled(call).at(0));
                   }
                   return pairs;
               }}}},
            {WhileType, {{StmtNumberType, createGetStatementsFunction(WhileStatement)}}},
            {IfType, {{StmtNumberType, createGetStatementsFunction(IfStatement)}}},
            {AssignType, {{StmtNumberType, createGetStatementsFunction(AssignmentStatement)}}},
            {VariableType,
             {{VarNameType,
               []() {
                   return convertToWithPairs(getAllVariables());
               }}}},
            {ConstantType,
             {{ValueType,
               []() {
                   return convertToWithPairs(getAllConstants());
               }}}},
            {ProcedureType, {{ProcNameType, []() {
                                  return convertToWithPairs(getAllProcedures());
                              }}}}};
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
            {VariableType,
             []() {
                 return convertToWithPairs(getAllVariables());
             }},
            {ConstantType,
             []() {
                 return convertToWithPairs(getAllConstants());
             }},
            {ProcedureType, []() {
                 return convertToWithPairs(getAllProcedures());
             }}};
}

SynonymTypeToClosureMap synonymTypeMap = getSynonymTypeMap();
AttributeTypeToClosureMap attributeTypeMap = getAttributeTypeMap();

/**
 * Given a reference of either AttributeRefType or SynonymRefType,
 * gets all results that match this reference from the Program
 * Knowledge Base, with the help of hash maps.
 *
 * @param ref Reference to retrieve results for.
 * @return List of (syn, syn.attr) matching this reference.
 */
Vector<WithPair> retrieveResultsForVariableReference(const Reference& ref)
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
    Vector<WithPair> resultsForVariable = retrieveResultsForVariableReference(varRef);
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
    for (const WithPair& pair : resultsForVariable) {
        // try to find a substitution that matches
        if (comparisonFunction(pair.attributeResult, rawLiteral)) {
            matchingResults.push_back(pair.synonymResult);
           // break;
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
        // all substitutions work
        resultsTable->storeResultsZero(true);
    }
    Vector<WithPair> resultsForLeft = retrieveResultsForVariableReference(leftRef);
    Vector<WithPair> resultsForRight = retrieveResultsForVariableReference(rightRef);
    PairedResult matchingResults;
    for (const WithPair& leftRes : resultsForLeft) {
        for (const WithPair& rightRes : resultsForRight) {
            // find substitutions that work and add them to results table
            if (leftRes.attributeResult == rightRes.attributeResult) {
                matchingResults.emplace_back(leftRes.synonymResult, rightRes.synonymResult);
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
