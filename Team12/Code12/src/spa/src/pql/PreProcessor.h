//
// Created by Chester Sim on 6/9/20.
//

#ifndef SPA_PREPROCESSOR_H
#define SPA_PREPROCESSOR_H

#include "AqTypes.h"

class PreProcessor {
public:
    AbstractQuery processQuery(String query);

private:
    static DeclarationTable processDeclarations(String declarations);
    static ClauseVector processClauses(String clausesString);
};

#endif // SPA_PREPROCESSOR_H
