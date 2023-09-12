#pragma once

#include <vector>
#include <string>
#include <fstream>

#include <map>

#include "Pex/Object.hpp"
#include "PscCodeBlock.hpp"

#include "Node/Base.hpp"

namespace Decompiler {

/**
 * @brief Decompiler class.
 *
 * This class contains the core process of the decompilation sequence.
 * The result is stored as string in the vector part of the class.
 */
class PscDecompiler :
        public std::vector<std::string>
{
public:
    PscDecompiler(  const Pex::Function& function,
                    const Pex::Object& object, 
                    bool commentAsm, 
                    bool traceDecompilation, 
                    bool dumpTree,
                    std::string outputDir);
    ~PscDecompiler();

    void decodeToAsm(std::uint8_t level, size_t begin, size_t end);
    bool isDebugFunction();

protected:


    void findVarTypes();
    const Pex::StringTable::Index& typeOfVar(const Pex::StringTable::Index& var) const;
    void createFlowBlocks();

    void createNodesForBlocks(size_t bloc);
    size_t findBlockForInstruction(size_t ip);


    void rebuildExpressionsInBlocks();
    void rebuildExpression(Node::BasePtr scope);

    void rebuildBooleanOperators(size_t startBlock, size_t endBlock);
    Node::BasePtr rebuildControlFlow(size_t startBlock, size_t endBlock);

    Node::BasePtr findScopeForVariable(const Pex::StringTable::Index& var, Node::BasePtr enclosingScope);

    void declareVariables(Node::BasePtr program);
    void cleanUpTree(Node::BasePtr program);

    void generateCode(Node::BasePtr program);  

    Pex::StringTable::Index toIdentifier(const Pex::Value& value) const;
    Node::BasePtr fromValue(size_t ip, const Pex::Value& value) const;
    Node::BasePtr checkAssign(Node::BasePtr expression) const;

    void dumpBlock(size_t startBlock, size_t endBlock);
protected:
    typedef std::map<size_t, PscCodeBlock*> CodeBlocs;
    CodeBlocs m_CodeBlocs;

    typedef std::map<std::uint16_t, Pex::StringTable::Index> VarToProperties;
    VarToProperties m_VarToProperties;

    typedef std::map<std::uint16_t, Pex::StringTable::Index> VarTypes;
    VarTypes m_VarTypes;

    Pex::StringTable::Index m_NoneVar;

    const Pex::Function& m_Function;
    const Pex::Object&   m_Object;
    bool m_ReturnNone;

    bool m_CommentAsm;
    bool m_TraceDecompilation;
    bool m_DumpTree;
    std::string m_OutputDir;

    std::ofstream m_Log;
    Pex::StringTable m_TempTable;

    void rebuildLocks(Node::BasePtr &program);
    void RemoveUnlocksFromBody(Node::BasePtr &body, const Node::BasePtr &matchingLock);
    void LiftLockBody(std::shared_ptr<Node::Base> &guard);

};
}
