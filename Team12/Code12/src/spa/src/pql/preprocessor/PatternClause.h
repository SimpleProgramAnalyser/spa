#ifndef SPA_PATTERNCLAUSE_H
#define SPA_PATTERNCLAUSE_H

#include "AqTypesUtils.h"
#include "Clause.h"
#include "DeclarationTable.h"
#include "DesignEntityType.h"
#include "ExpressionSpec.h"
#include "Reference.h"

enum PatternStatementType : char { AssignPatternType, WhilePatternType, IfPatternType };

// Hash function for RelationshipReferenceType
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
    static std::unordered_set<DesignEntityType> designEntityTypeValidationSet;
    static Clause* processAssignPatternClause(Synonym patternSynonym, StringVector constraints,
                                              DeclarationTable& declarationTable);
    static Clause* processIfWhilePatternClause(Synonym patternSynonym, DesignEntityType synonymDesignEntityType,
                                               StringVector constraints, DeclarationTable& declarationTable);
    static Boolean isValidVariableEntityRef(Reference ref);

    Synonym patternSynonym;
    PatternStatementType patternStatementType;
    Reference entityReference;
    ExpressionSpec expressionSpec;

public:
    static Clause* createPatternClause(const String& clauseConstraint, DeclarationTable& declarationTable);

    PatternClause(Synonym s, PatternStatementType statementType, Reference entRef);
    PatternClause(Synonym s, PatternStatementType statementType, Reference entRef, ExpressionSpec exprSpec);
    PatternStatementType getStatementType() const;
    Reference getEntRef() const;
    const ExpressionSpec& getExprSpec() const;
    Synonym getPatternSynonym() const;
    Boolean operator==(const PatternClause& patternClause);
};

#endif // SPA_PATTERNCLAUSE_H
