/**
 * Implementation of Abstract Syntax Tree
 * classes and methods.
 */

#include "AstTypes.h"

#include "Util.h"

StatementNumber StatementNode::getStatementNumber() const
{
    return stmtNum;
}

StatementNode::StatementNode(StatementNumber n): stmtNum(n) {}

bool StatementNode::operator==(const StatementNode& sn) const
{
    return this->compare(sn);
}

StmtlstNode::StmtlstNode(List<StatementNode> stmtLst): statementList(std::move(stmtLst)) {}

bool StmtlstNode::operator==(const StmtlstNode& sln) const
{
    return util::checkListValuesEqual<StatementNode>(this->statementList, sln.statementList);
}

ProcedureNode::ProcedureNode(Name n, const StmtlstNode* stmtLst):
    procedureName(std::move(n)), statementListNode(stmtLst)
{}

ProcedureNode::~ProcedureNode()
{
    delete statementListNode;
}

bool ProcedureNode::operator==(const ProcedureNode& pcn) const
{
    return this->procedureName == pcn.procedureName && *(this->statementListNode) == *(pcn.statementListNode);
}

ProgramNode::ProgramNode(Name n, List<ProcedureNode> procLst, StatementNumber totalStmts):
    programName(std::move(n)), procedureList(std::move(procLst)), totalNumberOfStatements(totalStmts)
{}

bool ProgramNode::operator==(const ProgramNode& pgn) const
{
    return this->totalNumberOfStatements == pgn.totalNumberOfStatements && this->programName == pgn.programName
           && util::checkListValuesEqual<ProcedureNode>(this->procedureList, pgn.procedureList);
}

bool BasicDataType::operator==(const BasicDataType& bdt) const
{
    return this->compare(bdt);
}

Constant::Constant(Integer val): value(val) {}

String Constant::toString()
{
    return std::to_string(static_cast<int>(value));
}

Boolean Constant::isConstant() const noexcept
{
    return true;
}

bool Constant::operator==(const BasicDataType& c) const
{
    return this->compare(c);
}

bool Constant::compare(const BasicDataType& c) const
{
    if (c.isConstant()) {
        // NOLINTNEXTLINE
        return static_cast<const Constant&>(c).value == this->value;
    } else {
        return false;
    }
}

Variable::Variable(Name n): varName(std::move(n)) {}

String Variable::toString()
{
    return static_cast<String>(varName);
}

Boolean Variable::isConstant() const noexcept
{
    return false;
}

bool Variable::operator==(const BasicDataType& v) const
{
    return this->compare(v);
}

bool Variable::compare(const BasicDataType& v) const
{
    if (!v.isConstant()) {
        // NOLINTNEXTLINE
        return static_cast<const Variable&>(v).varName == this->varName;
    } else {
        return false;
    }
}

ReadStatementNode::ReadStatementNode(StatementNumber stmtNum, Variable v): StatementNode(stmtNum), var(std::move(v)) {}

bool ReadStatementNode::operator==(const StatementNode& sn) const
{
    return this->compare(sn);
}

StatementType ReadStatementNode::getStatementType() const
{
    return ReadStatement;
}

bool ReadStatementNode::compare(const StatementNode& sn) const
{
    if (sn.getStatementType() == ReadStatement) {
        return this->getStatementNumber() == sn.getStatementNumber()
               // NOLINTNEXTLINE
               && static_cast<const ReadStatementNode&>(sn).var == this->var;
    } else {
        return false;
    }
}

PrintStatementNode::PrintStatementNode(StatementNumber stmtNum, Variable v): StatementNode(stmtNum), var(std::move(v))
{}

bool PrintStatementNode::operator==(const StatementNode& sn) const
{
    return this->compare(sn);
}

StatementType PrintStatementNode::getStatementType() const
{
    return PrintStatement;
}

bool PrintStatementNode::compare(const StatementNode& sn) const
{
    if (sn.getStatementType() == PrintStatement) {
        return this->getStatementNumber() == sn.getStatementNumber()
               // NOLINTNEXTLINE
               && static_cast<const PrintStatementNode&>(sn).var == this->var;
    } else {
        return false;
    }
}

CallStatementNode::CallStatementNode(StatementNumber stmtNum, Name n):
    StatementNode(stmtNum), procedureName(std::move(n))
{}

bool CallStatementNode::operator==(const StatementNode& sn) const
{
    return this->compare(sn);
}

StatementType CallStatementNode::getStatementType() const
{
    return CallStatement;
}

bool CallStatementNode::compare(const StatementNode& sn) const
{
    if (sn.getStatementType() == CallStatement) {
        return this->getStatementNumber() == sn.getStatementNumber()
               // NOLINTNEXTLINE
               && static_cast<const CallStatementNode&>(sn).procedureName == this->procedureName;
    } else {
        return false;
    }
}

bool ConditionalExpression::operator==(const ConditionalExpression& ce) const
{
    return this->compare(ce);
}

NotExpression::NotExpression(const ConditionalExpression* exp): expression(exp) {}

NotExpression::~NotExpression()
{
    delete expression;
}

bool NotExpression::operator==(const ConditionalExpression& ce) const
{
    return this->compare(ce);
}

ConditionalExpressionType NotExpression::getConditionalType() const noexcept
{
    return NotConditionalExpression;
}

bool NotExpression::compare(const ConditionalExpression& ce) const
{
    if (ce.getConditionalType() == NotConditionalExpression) {
        // NOLINTNEXTLINE
        return *(static_cast<const NotExpression&>(ce).expression) == *(this->expression);
    } else {
        return false;
    }
}

AndExpression::AndExpression(const ConditionalExpression* leftExp, const ConditionalExpression* rightExp):
    leftExpression(leftExp), rightExpression(rightExp)
{}

AndExpression::~AndExpression()
{
    delete leftExpression;
    delete rightExpression;
}

bool AndExpression::operator==(const ConditionalExpression& ce) const
{
    return this->compare(ce);
}

ConditionalExpressionType AndExpression::getConditionalType() const noexcept
{
    return AndConditionalExpression;
}

bool AndExpression::compare(const ConditionalExpression& ce) const
{
    if (ce.getConditionalType() == AndConditionalExpression) {
        // NOLINTNEXTLINE
        const AndExpression& ae = static_cast<const AndExpression&>(ce);
        return *(this->leftExpression) == *(ae.leftExpression) && *(this->rightExpression) == *(ae.rightExpression);
    } else {
        return false;
    }
}

OrExpression::OrExpression(const ConditionalExpression* leftExp, const ConditionalExpression* rightExp):
    leftExpression(leftExp), rightExpression(rightExp)
{}

OrExpression::~OrExpression()
{
    delete leftExpression;
    delete rightExpression;
}

bool OrExpression::operator==(const ConditionalExpression& ce) const
{
    return this->compare(ce);
}

ConditionalExpressionType OrExpression::getConditionalType() const noexcept
{
    return OrConditionalExpression;
}

bool OrExpression::compare(const ConditionalExpression& ce) const
{
    if (ce.getConditionalType() == OrConditionalExpression) {
        // NOLINTNEXTLINE
        const OrExpression& oe = static_cast<const OrExpression&>(ce);
        return *(this->leftExpression) == *(oe.leftExpression) && *(this->rightExpression) == *(oe.rightExpression);
    } else {
        return false;
    }
}

bool Expression::operator==(const Expression& ex) const
{
    return this->compare(ex);
}

ArithmeticExpression::ArithmeticExpression(const Expression* left, const Expression* right, ExpressionOperator op):
    leftFactor(left), rightFactor(right), opr(op)
{}

ArithmeticExpression::~ArithmeticExpression()
{
    delete leftFactor;
    delete rightFactor;
}

bool ArithmeticExpression::operator==(const Expression& ex) const
{
    return this->compare(ex);
}

Boolean ArithmeticExpression::isArithmetic() const noexcept
{
    return true;
}

bool ArithmeticExpression::compare(const Expression& ex) const
{
    if (ex.isArithmetic()) {
        // NOLINTNEXTLINE
        const ArithmeticExpression& ae = static_cast<const ArithmeticExpression&>(ex);
        return this->opr == ae.opr && *(this->leftFactor) == *(ae.leftFactor)
               && *(this->rightFactor) == *(ae.rightFactor);
    } else {
        return false;
    }
}

ReferenceExpression::ReferenceExpression(const BasicDataType* bd): basicData(bd) {}

ReferenceExpression::~ReferenceExpression()
{
    delete basicData;
}

bool ReferenceExpression::operator==(const Expression& re) const
{
    return this->compare(re);
}

Boolean ReferenceExpression::isArithmetic() const noexcept
{
    return false;
}

bool ReferenceExpression::compare(const Expression& ex) const
{
    if (!ex.isArithmetic()) {
        // NOLINTNEXTLINE
        return *(this->basicData) == *(static_cast<const ReferenceExpression&>(ex).basicData);
    } else {
        return false;
    }
}

RelationalExpression::RelationalExpression(const Expression* left, const Expression* right, RelationalOperator ro):
    leftFactor(left), rightFactor(right), opr(ro)
{}

RelationalExpression::~RelationalExpression()
{
    delete leftFactor;
    delete rightFactor;
}

bool RelationalExpression::operator==(const ConditionalExpression& ce) const
{
    return this->compare(ce);
}

ConditionalExpressionType RelationalExpression::getConditionalType() const noexcept
{
    return RelationalConditionalExpression;
}

bool RelationalExpression::compare(const ConditionalExpression& ce) const
{
    if (ce.getConditionalType() == RelationalConditionalExpression) {
        // NOLINTNEXTLINE
        const RelationalExpression& oe = static_cast<const RelationalExpression&>(ce);
        return this->opr == oe.opr && *(this->leftFactor) == *(oe.leftFactor)
               && *(this->rightFactor) == *(oe.rightFactor);
    } else {
        return false;
    }
}

IfStatementNode::IfStatementNode(StatementNumber stmtNum, const ConditionalExpression* pred, const StmtlstNode* ifs,
                                 const StmtlstNode* elses):
    StatementNode(stmtNum),
    predicate(pred), ifStatementList(ifs), elseStatementList(elses)
{}

IfStatementNode::~IfStatementNode()
{
    delete predicate;
    delete ifStatementList;
    delete elseStatementList;
}

bool IfStatementNode::operator==(const StatementNode& sn) const
{
    return this->compare(sn);
}

StatementType IfStatementNode::getStatementType() const
{
    return IfStatement;
}

bool IfStatementNode::compare(const StatementNode& sn) const
{
    if (sn.getStatementType() == IfStatement) {
        // NOLINTNEXTLINE
        const IfStatementNode& isn = static_cast<const IfStatementNode&>(sn);
        return this->getStatementNumber() == isn.getStatementNumber() && *(this->predicate) == *(isn.predicate)
               && *(this->ifStatementList) == *(isn.ifStatementList)
               && *(this->elseStatementList) == *(isn.elseStatementList);
    } else {
        return false;
    }
}

WhileStatementNode::WhileStatementNode(StatementNumber stmtNum, const ConditionalExpression* pred,
                                       const StmtlstNode* stmts):
    StatementNode(stmtNum),
    predicate(pred), statementList(stmts)
{}

WhileStatementNode::~WhileStatementNode()
{
    delete predicate;
    delete statementList;
}

bool WhileStatementNode::operator==(const StatementNode& sn) const
{
    return this->compare(sn);
}

StatementType WhileStatementNode::getStatementType() const
{
    return WhileStatement;
}

bool WhileStatementNode::compare(const StatementNode& sn) const
{
    if (sn.getStatementType() == WhileStatement) {
        // NOLINTNEXTLINE
        const WhileStatementNode& wsn = static_cast<const WhileStatementNode&>(sn);
        return this->getStatementNumber() == wsn.getStatementNumber() && *(this->predicate) == *(wsn.predicate)
               && *(this->statementList) == *(wsn.statementList);
    } else {
        return false;
    }
}

AssignmentStatementNode::AssignmentStatementNode(StatementNumber stmtNum, Variable v, const Expression* expr):
    StatementNode(stmtNum), variable(std::move(v)), expression(expr)
{}

AssignmentStatementNode::~AssignmentStatementNode()
{
    delete expression;
}

bool AssignmentStatementNode::operator==(const StatementNode& sn) const
{
    return this->compare(sn);
}

StatementType AssignmentStatementNode::getStatementType() const
{
    return AssignmentStatement;
}

bool AssignmentStatementNode::compare(const StatementNode& sn) const
{
    if (sn.getStatementType() == AssignmentStatement) {
        // NOLINTNEXTLINE
        const AssignmentStatementNode& asn = static_cast<const AssignmentStatementNode&>(sn);
        return this->getStatementNumber() == asn.getStatementNumber() && this->variable == asn.variable
               && *(this->expression) == *(asn.expression);
    } else {
        return false;
    }
}
