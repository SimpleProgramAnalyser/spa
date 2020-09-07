#include "FrontendManager.h"
#include "pql/FormattedQueryResult.h"
#include "pql/PqlManager.h"
#include "pql/QueryResultFormatType.h"
#include <Types.h>
#include <iostream>

// Constant declarations
const String LINEFEED_CHAR = "\n";
const String SIMPLE_EXIT_STR = "*";
const String PQL_EXIT_STR = "exit";

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

  FormattedQueryResult result = pqlManager.executeQuery(query, AUTOTESTER_FORMAT);
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

        if (line.compare(SIMPLE_EXIT_STR) == 0) {
            break;
        }

        // std::cout << line << std::endl;
        program.append(line + LINEFEED_CHAR);
    }

    return program;
}

// Main entry-point to our SPA!
int main(int argv, char **args)
{
    std::cout << "Welcome to our SIMPLE SPA!" << std::endl;
    std::cout << "Please enter a SIMPLE source program (when done, enter a '*' on a new line):" << std::endl;

    String program = readProgram();

    std::cout << std::endl << std::endl;

    // std::cout << "Program entered:" << std::endl << program << std::endl;

    std::cout << "Passing SIMPLE program to SPA frontend..." << std::endl;

    parse(program);

    std::cout << "Done..." << std::endl;
    
    while (true) {
        std::cout << "Please enter a PQL query (enter 'exit' to end)..." << std::endl;

        String declarations;
        String select;

        std::getline(std::cin, declarations);

        if (std::cin.fail()) {
            std::cout << "failed here" << std::endl;
            //error
            break;
        }

        std::getline(std::cin, select);

        if (std::cin.fail()) {
            std::cout << "failed here" << std::endl;
            //error
            break;
        }

        if (declarations.compare(PQL_EXIT_STR) == 0) {
            break;
        }

        String query = declarations + LINEFEED_CHAR + select;

        evaluate(query);
    }

    std::cout << "Thank you for using our SIMPLE SPA!" << std::endl;

    return 0;
}


