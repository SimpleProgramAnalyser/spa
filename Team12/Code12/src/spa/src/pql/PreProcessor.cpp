//
// Created by Chester Sim on 6/9/20.
//

#include "PreProcessor.h"

#include <iostream>

#include "../lexer/Lexer.h"

StringVector splitByFirstConsecutiveWhitespace(String str);

AbstractQuery PreProcessor::processQuery(String query)
{
    // Split between declarations and clauses
    StringList* splitBySelectList = splitByDelimiter(query, "Select");
    // Validate both strings exists.
    if (splitBySelectList->size() != 2) {
        return AbstractQuery::invalidAbstractQuery();
    }
    String declarationString = *(splitBySelectList->at(0));       // TODO: trim
    String synonymAndClausesString = *(splitBySelectList->at(1)); // TODO: trim

    // Process declarations into declaration table
    DeclarationTable declarationTable = processDeclarations(declarationString);
    if (declarationTable.hasInvalidDeclaration()) {
        return AbstractQuery::invalidAbstractQuery();
    }

    // Extract select synonym
    StringVector synonymAndClausesVector = splitByFirstConsecutiveWhitespace(synonymAndClausesString);
    Synonym selectSynonym = synonymAndClausesVector.at(0);
    if (!isValidSynonym(selectSynonym)) {
        return AbstractQuery::invalidAbstractQuery();
    }

    // Process clauses into ClauseList

    // Encapsulate into AbstractQuery and return

    AbstractQuery aq;
    return aq;
}

ClauseVector PreProcessor::processClauses(String clausesString) {}

DeclarationTable PreProcessor::processDeclarations(String declarationsString)
{
    DeclarationTable declarationTable;

    StringList* splitByWhiteSpacesList = splitByWhitespace(declarationsString);
    DesignEntity* currentDesignEntityPtr;
    bool hasCurrentDesignEntity = false;

    for (auto& token : *splitByWhiteSpacesList) {
        if (!hasCurrentDesignEntity) {
            currentDesignEntityPtr = new DesignEntity(*token);
            if (currentDesignEntityPtr->getType() == NonExistentType) {
                declarationTable.setInvalidDeclaration();
                return declarationTable;
            }

            hasCurrentDesignEntity = true;
        } else {
            char lastChar = token->at(token->size() - 1);
            if (lastChar == ';' || lastChar == ',') {
                String synonymToken = (*token).substr(0, token->size() - 1);

                if (!isValidSynonym(synonymToken) || declarationTable.hasSynonym(synonymToken)) {
                    declarationTable.setInvalidDeclaration();
                    return declarationTable;
                }

                declarationTable.addDeclaration(synonymToken, *currentDesignEntityPtr);

                if (lastChar == ';') {
                    hasCurrentDesignEntity = false;
                }

            } else {
                declarationTable.setInvalidDeclaration();
                return declarationTable;
            }
        }
    }

    return declarationTable;
}

// Utils

/**
 * Splits up the given string by the first
 * consecutive whitespace, into two substrings.
 *
 * @param str String to be split
 * @return vector of 2 strings
 */
StringVector splitByFirstConsecutiveWhitespace(String str)
{
    const char* currentChar = str.c_str();
    String* currentToken = new String();
    StringVector splitByFirstWhitespaceVector;

    // Find first whitespace
    while (!isWhitespace(currentChar)) {
        currentToken->push_back(*currentChar);
        currentChar++;
    }

    splitByFirstWhitespaceVector.push_back(*currentToken);
    currentToken = new String();

    // skip past all whitespaces
    while (isWhitespace(currentChar)) {
        currentChar++;
    }

    while (*currentChar != '\0') {
        currentToken->push_back(*currentChar);
        currentChar++;
    }

    splitByFirstWhitespaceVector.push_back(*currentToken);

    return splitByFirstWhitespaceVector;
}