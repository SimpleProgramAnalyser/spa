/**
 * This class represents the PQL query evaluator
 * and encapsulates logic necessary for further
 * processing a PQL query (e.g, optimisation), and
 * finally interfacing with the PKB component,
 * for the results of the query.
 */

#ifndef SPA_PQL_EVALUATOR_H
#define SPA_PQL_EVALUATOR_H

#include "Types.h"
#include "pql/preprocessor/AqTypes.h"
#include "pql/projector/RawQueryResult.h"

class Evaluator {
public:
    RawQueryResult evaluateQuery(AbstractQuery abstractQuery);

private:
    const Integer DummyVectorIntRes = 12345;
    Boolean isQueryVacuouslyTrue(ClauseVector clauses);
    Boolean isQueryVacuouslyTrue(Vector<RawResultFromClause> results);
    Vector<RawResultFromClause> filterResultsRelatedToSyn(Vector<RawResultFromClause> results);
    RawResultFromClause retrieveResultsForVacuouslyTrueQueries(DesignEntityType entTypeOfSynonym);
    RawQueryResult processQuery(AbstractQuery abstractQuery);
    RawResultFromClauses processQueryClauses(Synonym synonym, ClauseVector clauses, DeclarationTable declarations);
    RawResultFromClause processQueryClause(Synonym synonym, Clause* clause, DeclarationTable declarations);
    RawResultFromClause processQuerySuchThatClause(Synonym synonym, SuchThatClause* stClause, DeclarationTable declarations);
    RawResultFromClause processQuerySuchThatFollowsClause(Synonym synonym, SuchThatClause* stClause,
                                                          DeclarationTable declarations);
    RawResultFromClause processQuerySuchThatFollowsStarClause(Synonym synonym, SuchThatClause* stClause,
                                                          DeclarationTable declarations);
    StatementType mapToStatementType(DesignEntityType entType);
};

#endif // SPA_PQL_EVALUATOR_H
