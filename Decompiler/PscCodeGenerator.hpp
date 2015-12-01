#pragma once

#include "Node/Visitor.hpp"
#include "PscDecompiler.hpp"

#include <sstream>

namespace Decompiler
{

/**
 * @brief Write a tree as Papyrus statements.
 *
 * This visitor outputs the expression nodes and writes them
 * as Papyrus statements.
 *
 */
class PscCodeGenerator : public Node::Visitor
{
public:
    PscCodeGenerator(Decompiler::PscDecompiler* decompiler);
    virtual ~PscCodeGenerator() = default;


    virtual void visit(Node::Scope* node);
    virtual void visit(Node::BinaryOperator* node);
    virtual void visit(Node::UnaryOperator* node);
    virtual void visit(Node::Assign* node);
    virtual void visit(Node::AssignOperator* node);
    virtual void visit(Node::Copy* node);
    virtual void visit(Node::Cast* node);
    virtual void visit(Node::CallMethod* node);
    virtual void visit(Node::Params* node);
    virtual void visit(Node::Return* node);
    virtual void visit(Node::PropertyAccess* node);
    virtual void visit(Node::StructCreate* node);
    virtual void visit(Node::ArrayCreate* node);
    virtual void visit(Node::ArrayLength* node);
    virtual void visit(Node::ArrayAccess* node);
    virtual void visit(Node::Constant* node);
    virtual void visit(Node::IdentifierString* node);
    virtual void visit(Node::While* node);
    virtual void visit(Node::IfElse* node);
    virtual void visit(Node::Declare* node);

protected:    
    void newLine();

    std::ostringstream m_Result;
    std::uint8_t m_Level{ 0 };
    Decompiler::PscDecompiler* m_Decompiler;
};

}
