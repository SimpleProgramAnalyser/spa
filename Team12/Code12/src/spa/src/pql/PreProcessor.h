//
// Created by Chester Sim on 6/9/20.
//

#ifndef AUTOTESTER_PREPROCESSOR_H
#define AUTOTESTER_PREPROCESSOR_H

#include "AqTypes.h"

class PreProcessor {
public:
    AbstractQuery processQuery(String query);
//private:
    static void dummyErrorLog(String errMessage);
    static DeclarationTable& processDeclarations(String declarations);
    static StringList* splitByFirstWhitespace(String synonymAndClausesString);
    static ClauseList& processClauses(String clausesString);

};

#endif // AUTOTESTER_PREPROCESSOR_H
