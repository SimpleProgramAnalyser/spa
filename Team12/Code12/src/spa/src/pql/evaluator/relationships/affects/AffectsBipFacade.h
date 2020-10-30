/**
 * This class contains methods for Affects Evaluator to
 * retrieve and make use of the Control Flow Graph and
 * Next table with branching into procedures (BIP).
 */

#ifndef SPA_PQL_AFFECTS_BIP_FACADE_H
#define SPA_PQL_AFFECTS_BIP_FACADE_H

#include "AffectsEvaluatorFacade.h"

class AffectsBipFacade: public AffectsEvaluatorFacade {
public:
    AffectsBipFacade() = default;
    AffectsBipFacade(const AffectsBipFacade&) = default;
    AffectsBipFacade(AffectsBipFacade&&) = default;
    ~AffectsBipFacade() override = default;
    AffectsBipFacade& operator=(const AffectsBipFacade&) = default;
    AffectsBipFacade& operator=(AffectsBipFacade&&) = default;

    /**
     * Returns a list of variables modified by a
     * statement, given the statement's number, unless
     * the statement is a Call statement.
     *
     * Because for the branching into procedures Affects
     * relationship, Call statements should lead to other
     * nodes where we can find the true source of variable
     * modification, we ignore Calls here.
     */
    Vector<String> getModified(Integer stmtNum) override;

    /**
     * Returns a list of statement numbers that are directly
     * after the statement number specified in the Control
     * Flow Graph with branching into other procedures,
     * as described by the "NextBip" relationship.
     */
    Vector<Integer> getNext(Integer stmtNum) override;

    /**
     * Returns a list of statement numbers that are directly
     * before the statement number specified in the Control
     * Flow Graph with branching into other procedures,
     * as described by the "NextBip" relationship.
     */
    Vector<Integer> getPrevious(Integer stmtNum) override;

    /**
     * Returns a list of procedures that have a Control Flow
     * Graph with branching into procedures in the program.
     */
    Vector<String> getRelevantProcedures() override;

    /**
     * Returns the root node of the Control Flow Graph
     * with branching into procedures (CFG BIP) of the
     * procedure with the name provided. If the
     * procedure does not have a CFG BIP, this
     * will return nullptr.
     */
    CfgNode* getCfg(const String& procedureName) override;
};

#endif // SPA_PQL_AFFECTS_BIP_FACADE_H
