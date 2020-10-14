/**
 * Implementation of methods that retrieve a synonym's
 * attributes from the Program Knowledge Base.
 */

#include "AttributeMap.h"

#include <functional>

#include "pkb/PKB.h"

typedef std::unordered_map<DesignEntityType,
                           std::unordered_map<AttributeType, std::function<std::string(const std::string&)>>>
    SignificanceMap;

/**
 * Returns a map that describes which attribute type is significant
 * to which synonym, requiring a different value than the default.
 *
 * This map also translates said attribute type and synonym to the
 * mapping function that takes in the default value and returns
 * the different value corresponding to the attribute type.
 */
SignificanceMap getSignificanceMap()
{
    return SignificanceMap({{CallType,
                             {{ProcNameType,
                               [](const std::string& stmtNum) {
                                   Vector<String> procedures = getProcedureCalled(std::stoi(stmtNum));
                                   return procedures.empty() ? "" : procedures[0];
                               }}}},
                            {ReadType,
                             {{VarNameType,
                               [](const std::string& stmtNum) {
                                   Vector<String> variables = getModifiesVariablesFromStatement(std::stoi(stmtNum));
                                   return variables.empty() ? "" : variables[0];
                               }}}},
                            {PrintType, {{VarNameType, [](const std::string& stmtNum) {
                                              Vector<String> variables
                                                  = getUsesVariablesFromStatement(std::stoi(stmtNum));
                                              return variables.empty() ? "" : variables[0];
                                          }}}}});
}

/**
 * Checks whether the evaluation mode should be different
 * from the default, given an attribute for a synonym.
 *
 * @param typeOfSynonym The type of the synonym.
 * @param typeOfAttribute The type of the attribute.
 * @param map The SignificanceMap to be used for the checking.
 * @return True, if the evaluation mode should be different.
 *         Otherwise, if evaluation mode is the same, false.
 */
bool isSignificant(DesignEntityType typeOfSynonym, AttributeType typeOfAttribute, const SignificanceMap& map)
{
    auto positionOfSynonymTypeInMap = map.find(typeOfSynonym);
    if (positionOfSynonymTypeInMap == map.end()) {
        return false;
    } else {
        std::unordered_map<AttributeType, std::function<std::string(const std::string&)>> innerMap
            = positionOfSynonymTypeInMap->second;
        return innerMap.find(typeOfAttribute) != innerMap.end();
    }
}

/**
 * For a single default value, transforms it into the correct
 * value based on the synonym and attribute type provided.
 *
 * @param value The default value matching the synonym.
 * @param synonymType The type of synonym.
 * @param attributeType The type of the attribute.
 * @param map The SignificanceMap which stores the function.
 * @return The correct value matching the attribute.
 */
String getResultForSingleValue(const String& value, DesignEntityType synonymType, AttributeType attributeType,
                               const SignificanceMap& map)
{
    if (isSignificant(synonymType, attributeType, map)) {
        return map[synonymType][attributeType](value);
    } else {
        return value;
    }
}

ClauseResult mapAttributesOne(const ResultsTable& results, const ClauseResult& values, const ResultSynonym& synonym)
{
    DesignEntityType synonymType = results.getTypeOfSynonym(synonym.getSynonym());
    AttributeType attributeType = synonym.getAttribute().getType();
    SignificanceMap significanceMap = getSignificanceMap();
    if (!isSignificant(synonymType, attributeType, significanceMap)) {
        return values;
    } else {
        std::unordered_set<std::string> attributesResult;
        for (const String& value : values) {
            attributesResult.insert(significanceMap[synonymType][attributeType](value));
        }
        return ClauseResult(attributesResult.begin(), attributesResult.end());
    }
}

PairedResult mapAttributesTwo(const ResultsTable& results, const PairedResult& pairs, const ResultSynonym& syn1,
                              const ResultSynonym& syn2)
{
    SignificanceMap significanceMap = getSignificanceMap();
    bool syn1Significant
        = isSignificant(results.getTypeOfSynonym(syn1.getSynonym()), syn1.getAttribute().getType(), significanceMap);
    bool syn2Significant
        = isSignificant(results.getTypeOfSynonym(syn2.getSynonym()), syn2.getAttribute().getType(), significanceMap);
    if (!syn1Significant && !syn2Significant) {
        return pairs;
    } else {
        std::unordered_set<std::pair<std::string, std::string>> attributesResult;
        for (const std::pair<std::string, std::string>& pair : pairs) {
            attributesResult.insert(significanceMap[synonymType][attributeType](value));
        }
        return PairedResult(attributesResult.begin(), attributesResult.end());
    }
}

NtupledResult mapAttributesN(const ResultsTable& results, const NtupledResult& tuples,
                             const Vector<ResultSynonym>& syns)
{
    NtupledResult attributesResult;
    return attributesResult;
}
