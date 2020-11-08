/**
 * This class represents a pattern clause in the query.
 */

#ifndef SPA_PQL_PREPROCESSOR_PATTERN_CLAUSE_H
#define SPA_PQL_PREPROCESSOR_PATTERN_CLAUSE_H

#include "AqTypesUtils.h"
#include "Clause.h"
#include "DeclarationTable.h"
#include "DesignEntityType.h"
#include "ExpressionSpec.h"
#include "Reference.h"

// All the types of PatternClause.
enum class PatternStatementType : char { AssignPatternType, WhilePatternType, IfPatternType };

// Hash function for PatternStatementType.
template <>
struct std::hash<PatternStatementType> {
    std::size_t operator()(const PatternStatementType& patternStatementType) const
    {
        // NOLINTNEXTLINE
        return std::hash<char>()(static_cast<const char&>(patternStatementType));
    }
};

class PatternClause: public Clause {
private:
    // Set of valid DesignEntityTypes for types of PatternClause.
    static std::unordered_set<DesignEntityType> designEntityTypeValidationSet;

    /**
     * Returns a Clause* pointing the newly instantiate
     * PatternClause of AssignPatternType. This methods
     * validates both the syntax and semantics of the
     * given parameters, before creating a valid
     * PatternClause. Otherwise, an erroneous Clause*
     * will be returned.
     *
     * @param patternSynonym    Synonym of the clause.
     * @param constraints       Constraints of the Assign PatternClause.
     * @param declarationTable  DeclarationTable to check the validity
     *                          of the Synonym.
     * @return                  Clause* of the Assign PatternClause.
     */
    static Clause* processAssignPatternClause(Synonym patternSynonym, StringVector constraints,
                                              DeclarationTable& declarationTable);

    /**
     * Returns a Clause* pointing the newly instantiate
     * PatternClause of either IfPatternType or
     * WhilePatternType. This methods validates both the
     * syntax and semantics of the given parameters,
     * before creating a valid PatternClause. Otherwise,
     * an erroneous Clause* will be returned.
     *
     * @param patternSynonym    Synonym of the clause.
     * @param constraints       Constraints of the If/While PatternClause.
     * @param declarationTable  DeclarationTable to check the validity
     *                          of the Synonym.
     * @return                  Clause* of the Assign PatternClause.
     */
    static Clause* processIfWhilePatternClause(Synonym patternSynonym, DesignEntityType synonymDesignEntityType,
                                               StringVector constraints, DeclarationTable& declarationTable);

    /**
     * Checks the validity of the variable in PatternClauses.
     *
     * @param ref   Reference to be checked.
     * @return      Returns QuerySyntaxError if the given Reference is not
     *              a Wildcard, Literal or Synonym ReferenceType. If the
     *              Reference is a Synonym, the method returns a
     *              QuerySemanticsError if the Synonym is not of
     *              VariableType. Otherwise, NoQueryError will be returned.
     */
    static QueryErrorType checkValidityOfVariable(const Reference& ref);

    Synonym patternSynonym;
    PatternStatementType patternStatementType;
    Reference entityReference;
    ExpressionSpec expressionSpec;

public:
    /**
     * Processes the clause constraint string for a
     * PatternClause, by abstracting it into its relevant
     * PatternTypeStatement, Reference and ExpressionSpec.
     *
     * @param clauseConstraint  String of the clause constraint with all
     *                          whitespaces removed.
     * @return                  Clause* of the PatternClause that was
     *                          constructed.
     */
    static Clause* createPatternClause(const String& clauseConstraint, DeclarationTable& declarationTable);

    /**
     * Instantiate a PatternClause with the given Synonym,
     * PatternStatementType and Reference. This is usually
     * used for If/While PatternClause since there they do
     * not contain any ExpressionSpec.
     *
     * @param s                 Synonym of the PatternClause.
     * @param statementType     Type of the PatternClause
     * @param entRef            Reference of the variable.
     */
    PatternClause(Synonym s, PatternStatementType statementType, Reference entRef);

    /**
     * Instantiate a PatternClause with the given Synonym,
     * PatternStatementType, Reference and ExpressionSpec.
     * This is usually used for Assign PatternClause, since
     * they can contain an Expression.
     *
     * @param s                 Synonym of the PatternClause.
     * @param statementType     Type of the PatternClause
     * @param entRef            Reference of the variable.
     * @param exprSpec          ExpressionSpec of the PatternClause.
     */
    PatternClause(Synonym s, PatternStatementType statementType, Reference entRef, ExpressionSpec exprSpec);

    // Retrieves the PatterStatementType of the PatternClause.
    PatternStatementType getStatementType() const;

    // Retrieves the variable Reference of the PatternClause.
    Reference getEntRef() const;

    // Retrieves the ExpressionSpec of the PatternCluase.
    const ExpressionSpec& getExprSpec() const;

    // Retrieves the Synonym of the PatternClause.
    Synonym getPatternSynonym() const;

    Boolean operator==(const PatternClause& patternClause);
};

#endif // SPA_PQL_PREPROCESSOR_PATTERN_CLAUSE_H
