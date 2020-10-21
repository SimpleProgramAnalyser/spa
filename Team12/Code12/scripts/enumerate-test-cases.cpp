/**
 * A program to enumerate AutoTester test queries properly.
 */

#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <sstream>

int main(int argc, char* argv[])
{
    std::regex commentSyntax("(\\d+[a-z]*)(\\ *)(- )");

    if (argc < 2) {
        std::cout << "Usage: ./enumerate-test-cases <filename>" << std::endl;
        return 1;
    }

    std::ifstream testQueries;
    testQueries.open(argv[1]);
    if (testQueries.fail() || testQueries.bad()) {
        std::cerr << "Error opening file. Terminating." << std::endl;
        return 1;
    }

    std::stringstream output;
    std::string line;
    std::int32_t index = 1;
    while (std::getline(testQueries, line)) {
        std::string currentQueryNumber = std::to_string(index) + " - ";
        std::string newLine;

        try {
            newLine = std::regex_replace(line, commentSyntax, currentQueryNumber);
        } catch (const std::regex_error& e) {
            std::cerr << "Regex error. Terminating." << std::endl;
            testQueries.close();
            return 1;
        }
        if (std::regex_search(line, commentSyntax)) {
            index++;
        }
        output << newLine << '\n';
    }

    std::ofstream newTestQueries;
    newTestQueries.open(argv[1]);
    newTestQueries << output.rdbuf();
    if (newTestQueries.fail() || newTestQueries.bad()) {
        std::cerr << "Error writing to file. Terminating." << std::endl;
        testQueries.close();
        newTestQueries.close();
        return 1;
    }

    std::cout << "Enumeration successful. File saved." << std::endl;
    testQueries.close();
    newTestQueries.close();
    return 0;
}
