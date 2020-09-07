#include "FrontendManager.h"
#include "pql/FormattedQueryResult.h"
#include "pql/PqlManager.h"
#include "pql/QueryResultFormatType.h"
#include <Types.h>

// Main entry-point to our SPA!
int main(int argv, char **args)
{
    return 0;
}

// method for parsing the SIMPLE source
void parse(String filename) {
    FrontendManager frontendManager;
}

// method to evaluating a query
void evaluate(String query){
// call your evaluator to evaluate the query here
  // ...code to evaluate query...

  // store the answers to the query in the results list (it is initially empty)
  // each result must be a string.
  PqlManager pqlManager;

  FormattedQueryResult result = pqlManager.executeQuery(query, AUTOTESTER_FORMAT);
}
