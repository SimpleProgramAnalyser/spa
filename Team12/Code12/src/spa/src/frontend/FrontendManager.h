/**
 * The Frontend Manager serves as the interface between
 * the UI and the Parser of Simple Program Analyser.
 *
 * Hence, it is also the main entry point of the SPA.
 * It contains one method to take in a SIMPLE program.
 */

#ifndef SPA_FRONTEND_MANAGER_H
#define SPA_FRONTEND_MANAGER_H

#include "Types.h"
#include "Ui.h"

/**
 * Parses a SIMPLE program string and stores the result
 * in the Program Knowledge Base. If there is any error,
 * call the UI to display the error to the user.
 *
 * @param rawProgram Raw SIMPLE program string.
 * @param ui UI to display errors.
 */
Void parseSimple(const String& rawProgram, Ui& ui);

#endif // SPA_FrontendManager_H
