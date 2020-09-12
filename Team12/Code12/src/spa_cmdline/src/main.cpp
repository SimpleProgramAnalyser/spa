#include <Types.h>
#include <iostream>

#include "frontend/FrontendManager.h"
#include "pql/FormattedQueryResult.h"
#include "pql/PqlManager.h"
#include "pql/QueryResultFormatType.h"

// Constant declarations
const String LINEFEED_CHAR = "\n";
const String SIMPLE_EXIT_STR = "*";
const String PQL_EXIT_STR = "exit";
const String GREET_MSG = "Welcome to our SIMPLE SPA!";
const String BYE_MSG = "Thank you for using our SIMPLE SPA!";
const String SIMPLE_PROGRAM_PROMPT_MSG = "Please enter a SIMPLE source program (when done, enter a '*' on a new line):";
const String SIMPLE_PROGRAM_PROCESSING_MSG = "Passing SIMPLE program to SPA frontend...";
const String DONE_FEEDBACK_MSG = "Done....";
const String PQL_QUERY_PROMPT_MSG = "Please enter a PQL query (enter 'exit' to end):";

/*
 * Passes the SIMPLE source program to the SPA
 * frontend for evaluation and processing.
 */
void parse(const String& program)
{
    parseSimple(program);
}

/*
 * Assuming the SPA frontend processing is done,
 * this method feeds (a single) PQL query to the SPA PQL
 * component.
 */
void evaluate(String query)
{
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
String readProgram()
{
    String program;

    while (true) {
        std::string line;
        std::getline(std::cin, line);

        if (std::cin.fail()) {
            // error
            break;
        }

        if (line == SIMPLE_EXIT_STR) {
            break;
        }

        // std::cout << line << std::endl;
        program.append(line + LINEFEED_CHAR);
    }

    return program;
}

// Main entry-point to our SPA!
int main(int argv, char** args)
{
    std::cout << GREET_MSG << std::endl;

    std::cout << SIMPLE_PROGRAM_PROMPT_MSG << std::endl;

    String program = readProgram();

    std::cout << std::endl << std::endl;

    std::cout << SIMPLE_PROGRAM_PROCESSING_MSG << std::endl;

    parse(program);

    std::cout << DONE_FEEDBACK_MSG << std::endl;

    while (true) {
        std::cout << PQL_QUERY_PROMPT_MSG << std::endl;

        String declarations;
        String select;

        std::getline(std::cin, declarations);

        if (std::cin.fail()) {
            // error
            break;
        }

        std::getline(std::cin, select);

        if (std::cin.fail()) {
            // error
            break;
        }

        if (declarations.compare(PQL_EXIT_STR) == 0) {
            break;
        }

        String query = declarations + LINEFEED_CHAR + select;

        evaluate(query);
    }

    std::cout << BYE_MSG << std::endl;

    return 0;
}
