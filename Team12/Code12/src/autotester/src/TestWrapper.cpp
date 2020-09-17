#include "TestWrapper.h"

#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>

#include "AbstractWrapper.h"
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

// method for parsing the SIMPLE source
void TestWrapper::parse(std::string filename)
{
    std::ifstream fileStream(filename);
    std::string program((std::istreambuf_iterator<char>(fileStream)),
        std::istreambuf_iterator<char>());
    std::cout << program << std::endl;
    parseSimple(program);
}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string>& results)
{
    // call your evaluator to evaluate the query here
    // ...code to evaluate query...

    // store the answers to the query in the results list (it is initially empty)
    // each result must be a string.
    PqlManager pqlManager;

    FormattedQueryResult result = pqlManager.executeQuery(query, AutotesterFormat);
    std::string::size_type pos_begin, pos_end = 0;
    std::string input = result.getResults();

    input.erase(std::remove_if(input.begin(), input.end(), [](auto x) { return std::isspace(x); }), input.end());

    while ((pos_begin = input.find_first_not_of(",", pos_end)) != std::string::npos) {
        pos_end = input.find_first_of(",", pos_begin);
        if (pos_end == std::string::npos)
            pos_end = input.length();

        results.push_back(input.substr(pos_begin, pos_end - pos_begin));
    }
}
