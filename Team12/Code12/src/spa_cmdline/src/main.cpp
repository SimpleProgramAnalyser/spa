#include "FrontendManager.h"
#include "pql/projector/FormattedQueryResult.h"
#include "pql/projector/QueryResultFormatType.h"
#include "pql/PqlManager.h"
#include <Types.h>
#include <iostream>

// Constant declarations
const String LinefeedChar = "\n";
const String SimpleExitStr = "*";
const String PqlExitStr = "exit";
const String GreetMsg = "Welcome to our SIMPLE SPA!";
const String ByeMsg = "Thank you for using our SIMPLE SPA!";
const String SimpleProgramPromptMsg =  "Please enter a SIMPLE source program (when done, enter a '*' on a new line):";
const String SimpleProgramProcessingMsg = "Passing SIMPLE program to SPA frontend...";
const String DoneFeedbackMsg = "Done....";
const String PqlQueryPromptMsg = "Please enter a PQL query (enter 'exit' to end):";

/*
 * Passes the SIMPLE source program to the SPA
 * frontend for evaluation and processing.
 */
void parse(String program) {
    FrontendManager frontendManager;
}

/*
 * Assuming the SPA frontend processing is done,
 * this method feeds (a single) PQL query to the SPA PQL
 * component.
 */
void evaluate(String query){
// call your evaluator to evaluate the query here
  // ...code to evaluate query...

  // store the answers to the query in the results list (it is initially empty)
  // each result must be a string.
  PqlManager pqlManager;

  FormattedQueryResult result = pqlManager.executeQuery(query, UiFormat);
}

/**
 * Reads a simple program from standard input (cin),
 * and supports linefeeds too (i.e, linefeeds in a
 * SIMPLE program).
 *
 * To terminate the read, enter a linefeed character,
 * e.g, Control-D on MacOS.
 */
String readProgram() {
    String program;

    while (true) {
        std::string line;
        std::getline(std::cin, line);

        if (std::cin.fail()) {
            //error
            break;
        }

        if (line.compare(SimpleExitStr) == 0) {
            break;
        }

        // std::cout << line << std::endl;
        program.append(line + LinefeedChar);
    }

    return program;
}

// Main entry-point to our SPA!
int main(int argv, char **args)
{
    std::cout << GreetMsg << std::endl;

    std::cout << SimpleProgramPromptMsg << std::endl;

    String program = readProgram();

    std::cout << std::endl << std::endl;

    std::cout << SimpleProgramProcessingMsg << std::endl;

    parse(program);

    std::cout << DoneFeedbackMsg << std::endl;
    
    while (true) {
        std::cout << PqlQueryPromptMsg << std::endl;

        String declarations;
        String select;

        std::getline(std::cin, declarations);

        if (std::cin.fail()) {
            //error
            break;
        }

        std::getline(std::cin, select);

        if (std::cin.fail()) {
            //error
            break;
        }

        if (declarations.compare(PqlExitStr) == 0) {
            break;
        }

        String query = declarations + LinefeedChar + select;

        evaluate(query);
    }

    std::cout << ByeMsg << std::endl;

    return 0;
}
