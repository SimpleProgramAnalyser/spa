/**
 * Utility classes and methods for integration tests.
 */

#ifndef INTEGRATION_TESTING_UTILS_H
#define INTEGRATION_TESTING_UTILS_H

#include "Ui.h"

class UiStub: public Ui {
    Void postUiError(InputError err) override;
};

#endif // INTEGRATION_TESTING_UTILS_H
