/**
 * A data structure that stores the Program Query
 * Language query after it is parsed.
 *
 * Three important pieces of information are relayed:
 * 1. The synonym that is being selected
 * 2. The declared Synonyms and their respective Design Entities
 * 3. The Clauses that constrains the result of the query
 */

#ifndef SPA_PQL_PREPROCESSOR_ABSTRACT_QUERY_H
#define SPA_PQL_PREPROCESSOR_ABSTRACT_QUERY_H

#include "ClauseVector.h"
#include "DeclarationTable.h"
#include "Errorable.h"
#include "ResultSynonymVector.h"

class AbstractQuery: public Errorable {
private:
    // Stores all synonyms to be selected. Empty Vector but valid AbstractQuery => Select BOOLEAN
    const ResultSynonymVector resultSynonyms;
    // Stores all clauses to be evaluated.
    ClauseVector clauses;
    // Stores all declarations.
    DeclarationTable declarationTable;
    // Set to true if query is semantically invalid and the result clause is BOOLEAN.
    Boolean isToReturnFalseResult;

public:
    /**
     * Returns an erroneous AbstractQuery with the given QueryErrorType
     * and ErrorMessage.
     *
     * @param queryErrorType    Type of error.
     * @param errorMessage      Reason of the error.
     */
    AbstractQuery(QueryErrorType queryErrorType, ErrorMessage errorMessage);

    /**
     * Returns an erroneous AbstractQuery with the given QueryErrorType
     * and ErrorMessage. This also allows toggling of the
     * isToReturnFalseResult member when the query is semantically
     * invalid and the result clause is BOOLEAN.
     *
     * @param queryErrorType    Type of error.
     * @param errorMessage      Reason of the error.
     * @param returnFalseResult Toggle for isToReturnFalseResult.
     */
    AbstractQuery(QueryErrorType queryErrorType, ErrorMessage errorMessage, Boolean returnFalseResult);

    /**
     * Returns a valid AbstractQuery that does not have any Clauses to evaluate.
     * @param synonyms      Synonyms to be selected.
     * @param declarations  DeclarationTable of the declarations
     *                      in the PQL query.
     */
    AbstractQuery(ResultSynonymVector synonyms, DeclarationTable& declarations);

    /**
     * Returns a valid AbstractQuery that stores the synonyms to be selected,
     * declarations in a DeclarationTable, and the clauses to be evaluated in
     * a ClauseVector.
     *
     * @param synonyms      Synonyms to be selected.
     * @param declarations  DeclarationTable of the declarations
     *                      in the PQL query.
     * @param clauseVector  Clauses to be evaluateed in a
     *                      ClauseVector.
     */
    AbstractQuery(ResultSynonymVector synonyms, DeclarationTable& declarations, ClauseVector& clauseVector);

    // Retrieves all the selected synonyms.
    Vector<ResultSynonym> getSelectedSynonyms() const;

    // Retrieves all the clauses in the query.
    const ClauseVector& getClauses() const;

    // Retrieves the DeclarationTable of all declarations.
    DeclarationTable getDeclarationTable() const;

    // Checks if the query is to return FALSE
    Boolean toReturnFalseResult() const;

    // Retrieves the error message.
    ErrorMessage getErrorMessage() const;

    Boolean operator==(const AbstractQuery& abstractQuery) const;
};

#endif // SPA_PQL_PREPROCESSOR_ABSTRACT_QUERY_H
