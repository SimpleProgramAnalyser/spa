/**
 * Interface for SPA UI component.
 */

#ifndef SPA_UI_H
#define SPA_UI_H

#include "Types.h"
#include "error/InputError.h"

class Ui {
public:
    virtual Void postUiError(InputError err) = 0;
};

#endif // SPA_UI_H
