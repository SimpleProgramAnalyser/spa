/**
 * This class represents the PQL query evaluator
 * and encapsulates logic necessary for further
 * processing a PQL query (e.g, optimisation), and
 * finally interfacing with the PKB component,
 * for the results of the query.
 */

#ifndef SPA_PQL_EVALUATOR_H
#define SPA_PQL_EVALUATOR_H

#include "pql/parser/AqTypes.h"
#include "pql/projector/RawQueryResult.h"
#include "Types.h"

class Evaluator {
public:
    RawQueryResult evaluateQuery(AbstractQuery abstractQuery);
private:
    Boolean isQueryVacuouslyTrue(AbstractQuery abstractQuery);
    RawQueryResult processQuery(AbstractQuery abstractQuery);
    Vector<Vector<String>> processQueryClauses(Synonym synonym, ClauseVector clauses, DeclarationTable declarations);
    Vector<String> processQueryClause(Synonym synonym, Clause* clause, DeclarationTable declarations);
    Vector<String> processQuerySuchThatClause(Synonym synonym, SuchThatClause* stClause, DeclarationTable declarations);
    Vector<String> processQuerySuchThatFollowsClause(Synonym synonym, SuchThatClause* stClause, DeclarationTable declarations);
};

#endif // SPA_PQL_EVALUATOR_H
