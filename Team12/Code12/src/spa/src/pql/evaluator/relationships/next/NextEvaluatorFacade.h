/**
 * This class contains methods for Next Evaluator to
 * interact with the rest of the SIMPLE program analyser.
 *
 * To change how Next Evaluator interacts with the
 * other components, simply extend the facade to
 * provide alternative methods.
 */

#ifndef SPA_PQL_NEXT_EVALUATOR_FACADE_H
#define SPA_PQL_NEXT_EVALUATOR_FACADE_H

#include "Types.h"

class NextEvaluatorFacade {
public:
    NextEvaluatorFacade() = default;
    NextEvaluatorFacade(const NextEvaluatorFacade&) = default;
    NextEvaluatorFacade(NextEvaluatorFacade&&) = default;
    virtual ~NextEvaluatorFacade() = default;
    NextEvaluatorFacade& operator=(const NextEvaluatorFacade&) = default;
    NextEvaluatorFacade& operator=(NextEvaluatorFacade&&) = default;

    /**
     * Returns a list of statement numbers that are directly
     * after the statement number prev. The statement numbers
     * must match the type specified in nextType.
     *
     * @param prev A statement number, to query for matching relationships.
     * @param nextType Statement type restriction on the returned statements.
     */
    virtual Vector<Integer> getNext(Integer prev, StatementType nextType);

    /**
     * Returns a list of statement numbers that are directly
     * before the statement number next. The statement numbers
     * must match the type specified in prevType.
     *
     * @param next A statement number, to query for matching relationships.
     * @param prevType Statement type restriction on the returned statements.
     */
    virtual Vector<Integer> getPrevious(Integer next, StatementType prevType);

    /**
     * Given two statement types, finds all Next relationships that
     * match Next(prev, next) where the first argument is of type
     * prevType and the second argument is of type nextType, and
     * returns all statement numbers that match "next".
     *
     * @param prevType Statement type restriction on the first element, to
     *                 query the Program Knowledge Base with.
     * @param nextType Statement type restriction on the returned statements.
     */
    virtual Vector<Integer> getNextMatching(StatementType prevType, StatementType nextType);

    /**
     * Given two statement types, finds all Next relationships that
     * match Next(prev, next) where the first argument is of type
     * prevType and the second argument is of type nextType, and
     * returns all statement numbers that match "prev".
     *
     * @param prevType Statement type restriction on the returned statements.
     * @param nextType Statement type restriction on the second element, to
     *                 query the Program Knowledge Base with.
     */
    virtual Vector<Integer> getPreviousMatching(StatementType prevType, StatementType nextType);

    /**
     * Given two statement types, finds all Next relationships that
     * match Next(prev, next) where the first argument is of type
     * prevType and the second argument is of type nextType.
     *
     * @param prevType Statement type restriction on the first element of returned pairs.
     * @param nextType Statement type restriction on the second element of returned pairs.
     */
    virtual Vector<std::pair<Integer, Integer>> getNextPairs(StatementType prevType, StatementType nextType);

    /**
     * Returns a list of statement numbers that match
     * the statement type provided.
     *
     * @param type Statement type restriction on the returned numbers.
     */
    virtual Vector<Integer> getStatements(StatementType type);

    /**
     * Returns true, if Next(prev, next) holds.
     *
     * @param prev First statement that occurs in the Control Flow.
     * @param next Next statement that occurs in the Control Flow.
     */
    virtual Boolean isNext(Integer prev, Integer next);
};

#endif // SPA_PQL_NEXT_EVALUATOR_FACADE_H
