/**
 * This class contains methods for Next Evaluator to
 * make use of the Next BIP table with branching
 * into procedures (BIP).
 */

#ifndef SPA_PQL_NEXT_BIP_FACADE_H
#define SPA_PQL_NEXT_BIP_FACADE_H

#include "NextEvaluatorFacade.h"
#include "pql/evaluator/relationships/bip/BipFacade.h"

class NextBipFacade: public NextEvaluatorFacade, public BipFacade {
public:
    NextBipFacade() = default;
    NextBipFacade(const NextBipFacade&) = default;
    NextBipFacade(NextBipFacade&&) = default;
    ~NextBipFacade() override = default;
    NextBipFacade& operator=(const NextBipFacade&) = default;
    NextBipFacade& operator=(NextBipFacade&&) = default;

    /**
     * Returns a list of statement numbers that are directly
     * after the statement number prev in the Control Flow
     * Graph with branching into procedures. The statement
     * numbers must match the type specified in nextType.
     *
     * @param prev A statement number, to query for matching relationships.
     * @param nextType Statement type restriction on the returned statements.
     */
    Vector<Integer> getNext(Integer prev, StatementType nextType) override;

    /**
     * Returns a list of statement numbers that are directly
     * before the statement number next in the Control Flow
     * Graph with branching into procedures. The statement
     * numbers must match the type specified in prevType.
     *
     * @param next A statement number, to query for matching relationships.
     * @param prevType Statement type restriction on the returned statements.
     */
    Vector<Integer> getPrevious(Integer next, StatementType prevType) override;

    /**
     * Given two statement types, finds all NextBip relationships that
     * match NextBip(prev, next) where the first argument is of type
     * prevType and the second argument is of type nextType, and
     * returns all statement numbers that match "next".
     *
     * @param prevType Statement type restriction on the first element, to
     *                 query the Program Knowledge Base with.
     * @param nextType Statement type restriction on the returned statements.
     */
    Vector<Integer> getNextMatching(StatementType prevType, StatementType nextType) override;

    /**
     * Given two statement types, finds all NextBip relationships that
     * match NextBip(prev, next) where the first argument is of type
     * prevType and the second argument is of type nextType, and
     * returns all statement numbers that match "prev".
     *
     * @param prevType Statement type restriction on the returned statements.
     * @param nextType Statement type restriction on the second element, to
     *                 query the Program Knowledge Base with.
     */
    Vector<Integer> getPreviousMatching(StatementType prevType, StatementType nextType) override;

    /**
     * Given two statement types, finds all NextBip relationships that
     * match NextBip(prev, next) where the first argument is of type
     * prevType and the second argument is of type nextType.
     *
     * @param prevType Statement type restriction on the first element of returned pairs.
     * @param nextType Statement type restriction on the second element of returned pairs.
     */
    Vector<std::pair<Integer, Integer>> getNextPairs(StatementType prevType, StatementType nextType) override;

    /**
     * Returns true, if NextBip(prev, next) holds.
     *
     * @param prev First statement that occurs in the Control Flow
     *             with branching into procedures.
     * @param next Next statement that occurs in the Control Flow
     *             with branching into procedures.
     */
    Boolean isNext(Integer prev, Integer next) override;

    /**
     * Returns true if the first Procedure has a Calls* relationship
     * with the second Procedure.
     */
    Boolean checksIfCallsStarHolds(ProcedureName p1, ProcedureName p2) override;

    /**
     * Returns the containing Procedure of the given statement.
     */
    ProcedureName getProcedureOfStmt(StatementNumber stmtNum) override;
};

#endif // SPA_PQL_NEXT_BIP_FACADE_H
