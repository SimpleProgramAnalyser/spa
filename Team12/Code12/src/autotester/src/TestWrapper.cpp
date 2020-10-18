#include "TestWrapper.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <streambuf>
#include <string>

#include "AbstractWrapper.h"
#include "Ui.h"
#include "frontend/FrontendManager.h"
#include "pql/PqlManager.h"
#include "pql/projector/FormattedQueryResult.h"
#include "pql/projector/QueryResultFormatType.h"

// implementation code of WrapperFactory - do NOT modify the next 5 lines
AbstractWrapper* WrapperFactory::wrapper = 0;
AbstractWrapper* WrapperFactory::createWrapper()
{
    if (wrapper == 0)
        wrapper = new TestWrapper;
    return wrapper;
}
// Do not modify the following line
volatile bool AbstractWrapper::GlobalStop = false;

// a default constructor
TestWrapper::TestWrapper()
{
    // create any objects here as instance variables of this class
    // as well as any initialization required for your spa program
}

class AutotesterUi: public Ui {
    Void postUiError(InputError err) override
    {
        std::cout << err.getTypeString() << ": " << err.getMessage() << std::endl;
        if (err.getSource() == ErrorSource::SimpleProgram) {
            throw std::runtime_error("Invalid source program. Terminating.");
        }
    }
};

// method for parsing the SIMPLE source
void TestWrapper::parse(std::string filename)
{
    std::ifstream fileStream(filename);
    std::string program((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());
    std::cout << program << std::endl;

    AutotesterUi ui;
    parseSimple(program, ui);
}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string>& results)
{
    // call your evaluator to evaluate the query here
    // ...code to evaluate query...
    AutotesterUi ui;
    // store the answers to the query in the results list (it is initially empty)
    // each result must be a string.
    FormattedQueryResult result = PqlManager::executeQuery(query, AutotesterFormat, ui);

    // retrieve String and perform split operation by delimiter "," to add into results
    std::string::size_type pos_begin, pos_end = 0;
    std::string input = result.getResults();
    const char* ws = " \t\n\r\f\v";
    while ((pos_begin = input.find_first_not_of(',', pos_end)) != std::string::npos) {
        pos_end = input.find_first_of(',', pos_begin);
        if (pos_end == std::string::npos)
            pos_end = input.length();
        // std::string::iterator end_pos = std::remove(input.begin(), input.end(), ' ');

        // input.erase(end_pos, input.end());
        // retrieve String and perform split operation by delimiter "," to add into results
        std::string substring = input.substr(pos_begin, pos_end - pos_begin);
        substring.erase(substring.find_last_not_of(ws) + 1);
        substring.erase(0, substring.find_first_not_of(ws));
        printf("result %s c\n", substring.c_str());
        results.push_back(substring);
    }
}
