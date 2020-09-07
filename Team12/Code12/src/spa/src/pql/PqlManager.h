//
// Created by Chester Sim on 1/9/20.
//

#ifndef AUTOTESTER_PQLMANAGER_H
#define AUTOTESTER_PQLMANAGER_H

#include "Types.h"

/**
 * Handles the entire flow of the business logic
 * for processing and evaluating a PQL query.
 */

class PQLManager {
public:
    String execute(String query);
};

#endif //AUTOTESTER_PQLMANAGER_H
