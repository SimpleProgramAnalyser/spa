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
        return map.at(synonymType).at(attributeType)(value);
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
    DesignEntityType syn1Type = results.getTypeOfSynonym(syn1.getSynonym());
    AttributeType syn1Attr = syn1.getAttribute().getType();
    DesignEntityType syn2Type = results.getTypeOfSynonym(syn2.getSynonym());
    AttributeType syn2Attr = syn2.getAttribute().getType();
    bool syn1Significant = isSignificant(syn1Type, syn1Attr, significanceMap);
    bool syn2Significant = isSignificant(syn2Type, syn2Attr, significanceMap);

    if (!syn1Significant && !syn2Significant) {
        return pairs;
    }
    std::unordered_set<ResultsRelation, ResultsRelationHasher> attributesResult;
    if (syn1Significant && syn2Significant) {
        for (const std::pair<std::string, std::string>& pair : pairs) {
            attributesResult.insert(ResultsRelation(significanceMap[syn1Type][syn1Attr](pair.first),
                                                    significanceMap[syn2Type][syn2Attr](pair.second)));
        }
    } else if (syn1Significant) {
        for (const std::pair<std::string, std::string>& pair : pairs) {
            attributesResult.insert(ResultsRelation(significanceMap[syn1Type][syn1Attr](pair.first), pair.second));
        }
    } else {
        // syn2Significant
        for (const std::pair<std::string, std::string>& pair : pairs) {
            attributesResult.insert(ResultsRelation(pair.first, significanceMap[syn2Type][syn2Attr](pair.second)));
        }
    }
    PairedResult resultsList;
    for (const ResultsRelation& rr : attributesResult) {
        resultsList.emplace_back(rr.value1, rr.value2);
    }
    return resultsList;
}

NtupledResult mapAttributesN(const ResultsTable& results, const NtupledResult& tuples,
                             const Vector<ResultSynonym>& syns)
{
    SignificanceMap significanceMap = getSignificanceMap();
    std::unordered_set<std::vector<std::string>, NtupleHasher> attributesResult;
    for (const std::vector<std::string>& tuple : tuples) {
        std::vector<std::string> mappedTuple;
        size_t length = tuple.size();
        mappedTuple.reserve(length);
        for (size_t i = 0; i < length; i++) {
            const ResultSynonym& currentSynonym = syns.at(i);
            mappedTuple.push_back(getResultForSingleValue(tuple[i],
                                                          results.getTypeOfSynonym(currentSynonym.getSynonym()),
                                                          currentSynonym.getAttribute().getType(), significanceMap));
        }
        attributesResult.insert(mappedTuple);
    }
    return NtupledResult(attributesResult.begin(), attributesResult.end());
}
