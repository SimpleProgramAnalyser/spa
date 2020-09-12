/**
 * The Frontend Manager serves as the interface between
 * the UI and the Parser of Simple Program Analyser.
 *
 * Hence, it is also the main entry point of the SPA.
 * It contains one method to take in a SIMPLE program.
 */

#ifndef SPA_FRONTEND_MANAGER_H
#define SPA_FRONTEND_MANAGER_H

#include <Types.h>

Void parseSimple(const String& rawProgram);

#endif // SPA_FrontendManager_H
