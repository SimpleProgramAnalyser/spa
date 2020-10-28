/**
 * This class contains methods for Affects Evaluator to
 * interact with the rest of the SIMPLE program analyser.
 *
 * To change how Affects Evaluator interacts with the
 * other components, simply extend the facade to
 * provide alternative methods.
 */

#ifndef SPA_PQL_AFFECTS_EVALUATOR_FACADE_H
#define SPA_PQL_AFFECTS_EVALUATOR_FACADE_H

#include "Types.h"
#include "cfg/CfgTypes.h"

class AffectsEvaluatorFacade {
public:
    AffectsEvaluatorFacade() = default;
    AffectsEvaluatorFacade(const AffectsEvaluatorFacade&) = default;
    AffectsEvaluatorFacade(AffectsEvaluatorFacade&&) = default;
    virtual ~AffectsEvaluatorFacade() = default;
    virtual AffectsEvaluatorFacade& operator=(const AffectsEvaluatorFacade&) = default;
    virtual AffectsEvaluatorFacade& operator=(AffectsEvaluatorFacade&&) = default;

    /**
     * Returns a list of variables used by a
     * statement, given the statement's number.
     */
    virtual Vector<String> getUsed(Integer stmtNum);

    /**
     * Returns a list of variables modified by a
     * statement, given the statement's number.
     */
    virtual Vector<String> getModified(Integer stmtNum);

    /**
     * Returns a list of statement numbers that are directly
     * after the statement number specified in the Control
     * Flow Graph of the program, as described by the "Next"
     * program design abstraction relationship.
     */
    virtual Vector<Integer> getNext(Integer stmtNum);

    /**
     * Returns a list of statement numbers that are directly
     * before the statement number specified in the Control
     * Flow Graph of the program, as described by the "Next"
     * program design abstraction relationship.
     */
    virtual Vector<Integer> getPrevious(Integer stmtNum);
    /**
     * Returns the statement type of a statement in
     * the program, given the statement's number.
     */
    virtual StatementType getType(Integer stmtNum);

    /**
     * Returns a list of statement numbers that refer
     * to every assignment statement in the program.
     */
    virtual Vector<Integer> getAssigns();

    /**
     * Returns a list of procedures that modify some variable
     * in the program. The exact variables do not matter.
     *
     * These procedures are relevant for the calculation of
     * Affects relationships as only statements (more
     * specifically, assignments) that modify variables
     * need to be looked at, by the definition.
     */
    virtual Vector<String> getRelevantProcedures();

    /**
     * Returns the root node of the Control Flow Graph
     * of the procedure with the name provided.
     */
    virtual CfgNode* getCfg(const String& procedureName);

    /**
     * Returns true, if the statement with the statement
     * number specified uses the variable name specified.
     */
    virtual Boolean doesStatementUse(Integer stmtNum, const String& variable);

    /**
     * Checks whether a statement number modifies a variable,
     * under the rules described by Affects/Affects*. Namely,
     * statement has to be an assignment, read or procedure
     * call statement and it has to modify the variable.
     *
     * @param stmtNum Statement number to be checked.
     * @param variable Variable to be checked.
     * @return True, if statement is an Assignment, Read or
     *         Call that modifies variable. Otherwise false.
     */
    virtual Boolean doesStatementModify(Integer stmtNum, const String& variable);
};

#endif // SPA_PQL_AFFECTS_EVALUATOR_FACADE_H
