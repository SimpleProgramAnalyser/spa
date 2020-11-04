/**
 * Implementation of tables library.
 */

#include "Tables.h"

#include <cassert>
#include <pkb/PKBUtils.h>

// Procedure Table
std::vector<String> ProcedureTable::getAllProcedures()
{
    return listOfProcedureNames;
}

Boolean ProcedureTable::isProcedureInProgram(const String& procName)
{
    return setOfProceduresNames.find(procName) != setOfProceduresNames.end();
}

/**
 * Insert a procedure name and its associated statement number range, characterized by the first and last statement
 * numbers (inclusive) into the Procedure Table. Idempotent.
 *
 * @param procName
 * @param firstStmtNum
 * @param lastStmtNum
 */
void ProcedureTable::insertIntoProcedureTable(const String& procName, StatementNumber firstStmtNum,
                                              StatementNumber lastStmtNum)
{
    deduplicatedAdd(procName, listOfProcedureNames, setOfProceduresNames);
    procNameStmtRangeMap[procName] = StatementNumberRange{firstStmtNum, lastStmtNum};
    firstStmtToProc[firstStmtNum] = procName;
}

/**
 * Given a procedure name, returns a StatementNumberRange struct, with properties first and last, which are the first
 * and last statement numbers of that procedure (inclusive). If procedure name is invalid, returns [0,0] in a
 * StatementNumberRange struct.
 *
 * @param procedureName
 * @return
 */
StatementNumberRange ProcedureTable::getStatementRangeByProcedure(const ProcedureName& procedureName)
{
    if (isProcedureInProgram(procedureName)) {
        return procNameStmtRangeMap[procedureName];
    } else {
        return StatementNumberRange{0, 0};
    }
}

Vector<ProcedureName> ProcedureTable::getContainingProcedure(StatementNumber statementNumber)
{
    Vector<ProcedureName> toReturn;
    /**
     * For example, if we have statement number 10, and the entries we search are 1 4 8 15,
     * lower bound will return iterator to 15, but it is the procedure that contains 8-14 that we want to consider.
     * hence we declare the call invalid if begin() iterator is found.
     */
    auto upperBoundIT = firstStmtToProc.upper_bound(statementNumber);
    if (upperBoundIT == firstStmtToProc.begin()) {
        return toReturn;
    } else {
        // decrement pointer so procedure's range contains statementNumber
        upperBoundIT--;
        /**
         * Here, we just check if the statement number is in the range, for the cases where statement number given is
         * more than even that contained by the last procedure.
         */
        StatementNumberRange range = procNameStmtRangeMap[upperBoundIT->second];
        // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
        assert(range.first <= statementNumber && "Should have been filtered out by begin()");
        if (range.last < statementNumber) { // illegal
            return toReturn;
        }
        toReturn.push_back(upperBoundIT->second);
        return toReturn;
    }
}

// Variable Table
void VariableTable::insertIntoVariableTable(const String& varName)
{
    if (setOfVariables.find(varName) == setOfVariables.end()) {
        listOfVariables.push_back(varName);
        setOfVariables.insert(varName);
    }
}
Vector<String> VariableTable::getAllVariables()
{
    return listOfVariables;
}
Boolean VariableTable::isVariableInProgram(const String& varName)
{
    return setOfVariables.find(varName) != setOfVariables.end();
}

// Statement Table
/**
 * For statements other than Call Statements - for call statements, look for the overloaded method with ProcedureName as
 * the second parameter.
 *
 * Inserts a statement number with its type into the statement table. Idempotent.
 *
 * @param stmtNum natural number from 1 to 2^31-1
 * @param stmtType values from StatementType enum other than AnyStatement and CallStatement
 */
void StatementTable::insertIntoStatementTable(Integer stmtNum, StatementType stmtType)
{
    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
    assert(stmtType > AnyStatement && stmtType < StatementTypeCount && stmtType != CallStatement
           && "Statement type cannot be AnyStatement, CallStatement or STATEMENT_TYPE_COUNT. Use overloaded method for "
              "CallStatement."); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
    assert(stmtNum >= 1 && "Statement number must be a natural number between 1 and 2^31-1");
    if (setOfStatements.find(stmtNum) == setOfStatements.end()) {
        setOfStatements.insert(stmtNum);
        statementTypes.insert({stmtNum, stmtType});
        listOfAllStatement.byType[stmtType].push_back(stmtNum);
        listOfAllStatement.byType[AnyStatement].push_back(stmtNum);
        setOfAllStatement.byType[stmtType].insert(stmtNum);
        setOfAllStatement.byType[AnyStatement].insert(stmtNum);
    }
}

Boolean StatementTable::isStatementInProgram(Integer stmtNum)
{
    return setOfStatements.find(stmtNum) != setOfStatements.end();
}

Vector<Integer> StatementTable::getAllStatements(StatementType stmtType)
{
    return listOfAllStatement.byType[stmtType];
}

/**
 * For Call Statements only. For other statement types, use the overloaded method of the same name with second argument
 * as StatementType.
 *
 * Insert a Call Statement with stmtNum and the procName it is calling into the statement table. Idempotent.
 *
 * @param stmtNum Statement number of the call statement
 * @param procName Procedure name of the procedure called by the call statement
 */
void StatementTable::insertIntoStatementTable(Integer stmtNum, const ProcedureName& procName)
{
    // general statement table
    if (setOfStatements.find(stmtNum) == setOfStatements.end()) {
        setOfStatements.insert(stmtNum);
        statementTypes.insert({stmtNum, CallStatement});
        listOfAllStatement.byType[CallStatement].push_back(stmtNum);
        listOfAllStatement.byType[AnyStatement].push_back(stmtNum);
        setOfAllStatement.byType[CallStatement].insert(stmtNum);
        setOfAllStatement.byType[AnyStatement].insert(stmtNum);
    }

    // call statement-> proc called and inverse
    procCalled[stmtNum] = procName;
    if (stmtsCallingSet[procName].find(stmtNum) == stmtsCallingSet[procName].end()) {
        stmtsCallingSet[procName].insert(stmtNum);
        stmtsCalling[procName].push_back(stmtNum);
    }

    // all procedures list
    if (allProcCalledSet.find(procName) == allProcCalledSet.end()) {
        allProcCalledSet.insert(procName);
        allProcCalled.push_back(procName);
    }
}

/**
 * Given a call statement's statement number, return the name of the procedure called in a vector. Vector is empty if
 * the statement number given is invalid or is not a call statement.
 *
 * @param callStmtNum
 * @return
 */
Vector<String> StatementTable::getProcedureCalled(Integer callStmtNum)
{
    Vector<ProcedureName> toReturn;
    if (procCalled.find(callStmtNum) != procCalled.end()) {
        toReturn.push_back(procCalled.at(callStmtNum));
    }
    return toReturn;
}

/**
 * Given a procedure name, returns a vector of all statement numbers which called the procedure. Vector returned is
 * empty if the procedure is invalid or not called in the entire SIMPLE program.
 *
 * @param procName
 * @return
 */
Vector<Integer> StatementTable::getAllCallStatementsByProcedure(const String& procName)
{
    return stmtsCalling[procName];
}

/**
 * Fetches all procedures called by some call statement in the SIMPLE program in a vector. Vector is empty if there are
 * no call statements in the program.
 *
 * @return
 */
Vector<String> StatementTable::getAllProceduresCalled()
{
    return allProcCalled;
}

/**
 * Returns the StatementType of statement stmtNum.
 *
 * @param stmtNum   StatementNumber of statement. If StatementNumber
 *                  is not valid, return NonExistentStatement type.
 * @return
 */
StatementType StatementTable::getStatementType(StatementNumber stmtNum)
{
    if (statementTypes.find(stmtNum) == statementTypes.end()) {
        return NonExistentStatement;
    }
    return statementTypes.find(stmtNum)->second;
}

// Constant Table
void ConstantTable::insertIntoConstantTable(Integer constant)
{
    if (setOfConstants.find(constant) == setOfConstants.end()) {
        setOfConstants.insert(constant);
        listOfConstants.push_back(constant);
    }
}
Boolean ConstantTable::isConstantInProgram(Integer constant)
{
    return setOfConstants.find(constant) != setOfConstants.end();
}
Vector<Integer> ConstantTable::getAllConstants()
{
    return listOfConstants;
}
