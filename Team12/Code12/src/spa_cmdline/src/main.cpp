#include <Types.h>
#include <iostream>

#include "frontend/FrontendManager.h"
#include "pql/PqlManager.h"

class CmdLineUi: public Ui {
public:
    bool hasError = false;
    Void postUiError(InputError err) override
    {
        hasError = true;
        std::cout << err.getSourceString() << " " << err.getTypeString() << ": " << err.getMessage() << std::endl;
    }
};

/*
 * Passes the SIMPLE source program to the SPA
 * frontend for evaluation and processing.
 *
 * @return True, if parsing succeeded.
 *         False, if there is an error.
 */
bool parse(const String& program, CmdLineUi& ui)
{
    parseSimple(program, ui);
    return !ui.hasError;
}

/*
 * Assuming the SPA frontend processing is done,
 * this method feeds (a single) PQL query to the SPA PQL
 * component.
 */
void evaluate(const String& query, CmdLineUi& ui)
{
    FormattedQueryResult result = PqlManager::executeQuery(query, UiFormat, ui, true);
    std::cout << result.getResults() << std::endl;
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
    // Constant declarations
    const String LinefeedChar = "\n";
    const String SimpleExitStr = "\\";

    String program;

    while (true) {
        std::string line;
        std::getline(std::cin, line);

        if (std::cin.fail()) {
            // error
            std::cout << "Error in reading from command line input!" << std::endl;
            break;
        }

        if (line == SimpleExitStr) {
            break;
        }

        // std::cout << line << std::endl;
        program.append(line + LinefeedChar);
    }

    return program;
}

// Main entry-point to our SPA!
int main(int argv, char** args)
{
    // Constant declarations
    const String greetMsg = "Hi! I am SPA-SER! Welcome to our SIMPLE SPA!";
    const String simpleProgramPromptMsg
        = "Please enter a SIMPLE source program (when done, enter a '\\' on a new line):";
    const String tryAgainMsg = "\nPlease check your program and try again.";
    const String doneFeedbackMsg = "Done...";
    const String pqlQueryPromptMsg = "Please enter a PQL query (enter '\\' on a new line when done, or 'exit' to end):";
    const String byeMsg = "Thank you for using our SIMPLE SPA!";
    const String simpleProgramProcessingMsg = "Passing SIMPLE program to SPA frontend...";
    const String linefeedChar = "\n";
    const String pqlEndStr = "\\";
    const String pqlExitStr = "exit";
    const String spaSer = "           ,     ,\n"
                          "  .   ,--'<\"-._.-\">'--,\n"
                          "<. .>/   (o )   (o )   \\\n"
                          " | | |   _.        ..   )\n"
                          " | | /    '-.__________.'\n"
                          " | |/    /            /\n"
                          " \\ /   .' simple    ,'\n"
                          "  |   :   program   |\n"
                          "  |   :   analyser  ',\n"
                          "  |    \\              \\\n";

    // UI to print messages to
    CmdLineUi ui;

    std::cout << spaSer << greetMsg << std::endl;
    bool parsingNotYetSucceeded = true;
    while (parsingNotYetSucceeded) {
        std::cout << simpleProgramPromptMsg << std::endl;

        String program = readProgram();

        std::cout << std::endl << std::endl;
        std::cout << simpleProgramProcessingMsg << std::endl;

        bool parsingSucceeded = parse(program, ui);
        if (parsingSucceeded) {
            parsingNotYetSucceeded = false;
        } else {
            std::cout << tryAgainMsg << std::endl;
            ui.hasError = false;
        }
    }

    std::cout << doneFeedbackMsg << std::endl;

    std::cout << pqlQueryPromptMsg << std::endl;
    std::string query;
    while (true) {
        std::string current;
        std::getline(std::cin, current);
        if (std::cin.fail()) {
            // error
            std::cout << "Error in reading from command line input!" << std::endl;
            break;
        } else if (current == pqlExitStr) {
            break;
        } else if (current == pqlEndStr) {
            evaluate(query, ui);
            query.clear();
            std::cout << pqlQueryPromptMsg << std::endl;
        } else {
            query.append(current);
        }
    }
    std::cout << byeMsg << std::endl;
    return 0;
}
