//
// Created by Chester Sim on 6/9/20.
//

#include "PreProcessor.h"
#include "../lexer/Lexer.h"
#include <iostream>

const std::string WHITESPACE = " \n\r\t\f\v";



std::string ltrim(const std::string& s)
{
    size_t start = s.find_first_not_of(WHITESPACE);
    return (start == std::string::npos) ? "" : s.substr(start);
}

std::string rtrim(const std::string& s)
{
    size_t end = s.find_last_not_of(WHITESPACE);
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

std::string trim(const std::string& s)
{
    return rtrim(ltrim(s));
}


AbstractQuery PreProcessor::processQuery(String query) {

    // Split between declarations and clauses
    StringList* splitBySelectList = splitByDelimiter(query, "Select");
    String declarationString = *(splitBySelectList->at(0));
    String synonymAndClausesString = *(splitBySelectList->at(1));

    // Process declarations into declaration table
    DeclarationTable& declarationTable = processDeclarations(declarationString);

    // Process select synonym and clauses


    // Encapsulate into AbstractQuery and return

//    for (int i = 0; i < splitBySelectList->size(); i++) {
//        std::cout << *(splitBySelectList->at(i)) << "\n";
//    }

    AbstractQuery aq;
    return aq;
}

void PreProcessor::dummyErrorLog(String errMessage)
{
    std::cout << errMessage << "\n";
}

DeclarationTable& PreProcessor::processDeclarations(String declarationsString)
{
    // Split by semicolon
    StringList* declarationsList = splitByDelimiter(declarationsString, ";");
    for (int i = 0; i < declarationsList->size(); i++) {
        // Trim
        String trimmedString = trim(*(declarationsList->at(i)));

        // Split by first whitespace

        // Split by comma
//        StringList* splitByCommaList = splitByDelimiter(*(declarationsList->at(i)), ",");
//        for (int i = 0; i < splitByCommaList->size(); i++) {
//            std::cout << *(splitByCommaList->at(i)) << "\n";
//        }


    }

    DeclarationTable declarationTable;
    return declarationTable;
    // Iterate thru each declaration, create Declaration, store in table
}

StringList* PreProcessor::splitByFirstWhitespace(String synonymAndClausesString)
{

}

ClauseList& PreProcessor::processClauses(String clausesString)
{


}