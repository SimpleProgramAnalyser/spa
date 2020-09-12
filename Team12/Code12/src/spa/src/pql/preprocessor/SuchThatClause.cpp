//
// Created by Chester Sim on 13/9/20.
//

#include "AqTypes.h"

SuchThatClause::SuchThatClause(Relationship& r): Clause(SuchThatClauseType), relationship{r} {}

Relationship SuchThatClause::getRelationship()
{
    return relationship;
}

Boolean SuchThatClause::operator==(const SuchThatClause& suchThatClause)
{
    return this->relationship == suchThatClause.relationship;
}