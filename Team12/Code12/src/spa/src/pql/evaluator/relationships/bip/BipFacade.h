/**
 * Facade for methods used by BIP evaluators to access
 * other outside components like Program Knowledge Base.
 */

#ifndef SPA_PQL_BIP_FACADE_H
#define SPA_PQL_BIP_FACADE_H

#include "Types.h"
#include "cfg/CfgTypes.h"

class BipFacade {
public:
    BipFacade() = default;
    BipFacade(const BipFacade&) = default;
    BipFacade(BipFacade&&) = default;
    virtual ~BipFacade() = default;
    BipFacade& operator=(const BipFacade&) = default;
    BipFacade& operator=(BipFacade&&) = default;

    /**
     * Returns the root node of the Control Flow Graph
     * with branching into procedures (CFG BIP) of the
     * procedure with the name provided. If the
     * procedure does not have a CFG BIP, this
     * will return nullptr.
     */
    virtual CfgNode* getCfg(const String& procedureName);

    /**
     * Returns a list of procedures that contain the statement
     * number provided. This list should only contain 0 or 1
     * procedures, as no two procedures can have the same statement.
     */
    virtual Vector<String> getProcedure(Integer stmtNum);

    /**
     * Returns a list of procedures that calls or indirectly
     * calls the procedureName given.s
     */
    virtual Vector<String> getCallersStar(const String& procedureName);
};

#endif // SPA_PQL_BIP_FACADE_H
