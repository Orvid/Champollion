#pragma once

#include <cassert>
#include <functional>
#include <sstream>

#include "Node/Nodes.hpp"

namespace Decompiler
{
/**
 * @brief Inline node visitor dumping the tree for debug purposes.
 *
 * The output is made using a callback function taking a ostring stream as it's input.
 */
class DumpTree : public Node::VisitorBase
{
private:
    std::function<void (std::ostream& stream)> m_Callback;
    std::uint32_t m_Indent;
public:
    DumpTree(decltype(m_Callback) callback) :
        m_Callback(callback),
        m_Indent(0)
    {
        assert(callback);
    }

    virtual ~DumpTree()
    {

    }
    std::ostringstream indent()
    {
        std::ostringstream result;
        result << ";";
        for (auto i = 0; i < m_Indent; ++i)
        {
            result << " ";
        }
        return result;
    }

    void enter(const char* text, Node::Base* node)
    {
        m_Callback(indent() << text << "<" << node << "> (" << node->getBegin() << "," << node->getEnd() << ")");
        ++m_Indent;
    }
    void leave()
    {
        --m_Indent;
        m_Callback(indent() << "]");
    }

    virtual void visit(Node::Scope* node)
    {
        enter("Scope", node);
        VisitorBase::visit(node);
        leave();
    }

    virtual void visit(Node::BinaryOperator* node)
    {
        enter("Binary", node);
        m_Callback(indent() << "In:" << node->getResult()) ;
        m_Callback(indent() << "Left:");
        node->getLeft()->visit(this);
        m_Callback(indent() << "Op:" << node->getOperator() << " P:" << (int)node->getPrecedence());
        m_Callback(indent() << "Right:");
        node->getRight()->visit(this);
        leave();
    }

    virtual void visit(Node::UnaryOperator* node)
    {
        enter("Unary", node);
        m_Callback(indent() << "Op:" << node->getOperator() << " P:" << (int)node->getPrecedence());
        m_Callback(indent() << "Value:");
        node->getValue()->visit(this);
        leave();
    }
    virtual void visit(Node::Assign* node)
    {
        enter("Assign", node);
        m_Callback(indent() << "In:");
        node->getDestination()->visit(this);
        m_Callback(indent() << "Value:");
        node->getValue()->visit(this);
        leave();
    }

    virtual void visit(Node::AssignOperator* node)
    {
        enter("AssignOperator", node);
        m_Callback(indent() << "In:");
        node->getDestination()->visit(this);
        m_Callback(indent() << "Operator:" << node->getOperator());
        m_Callback(indent() << "Value:");
        node->getValue()->visit(this);
        leave();
    }
    virtual void visit(Node::Copy* node)
    {
        enter("Copy", node);
        m_Callback(indent() << "In:" << node->getResult());
        m_Callback(indent() << "Value:");
        node->getValue()->visit(this);
        leave();
    }
    virtual void visit(Node::Cast* node)
    {
        enter("Cast", node);
        m_Callback(indent() << "In:" << node->getResult());
        m_Callback(indent() << "As:" << node->getType());
        m_Callback(indent() << "Value:");
        node->getValue()->visit(this);
        leave();
    }

    virtual void visit(Node::CallMethod* node)
    {
        enter("Call", node);
        m_Callback(indent() << "In:" << node->getResult());
        m_Callback(indent() << "Method:" << node->getMethod());
        m_Callback(indent() << "On:");
        node->getObject()->visit(this);
        for (auto param : *node->getParameters())
        {
            m_Callback(indent() << "Param:");
            param->visit(this);
        }
        leave();
    }

    virtual void visit(Node::Params* node)
    {
        enter("Params", node);
        VisitorBase::visit(node);
        leave();
    }

    virtual void visit(Node::Return* node)
    {
        enter("Return", node);
        m_Callback(indent() << "Value:");
        node->getValue()->visit(this);
        leave();
    }

    virtual void visit(Node::PropertyAccess* node)
    {
        enter("PropertyAccess", node);
        m_Callback(indent() << "In:" << node->getResult());
        m_Callback(indent() << "Property:" << node->getProperty());
        m_Callback(indent() << "On:");
        node->getObject()->visit(this);
        leave();
    }

    virtual void visit(Node::ArrayCreate* node)
    {
        enter("ArrayCreate", node);
        m_Callback(indent() << "Type:" << node->getType());
        //m_Callback(indent() << "Size:" << node->getSize());
        leave();
    }

    virtual void visit(Node::ArrayLength* node)
    {
        enter("ArrayLength", node);
        m_Callback(indent() << "Array:");
        node->getArray()->visit(this);
        leave();
    }

    virtual void visit(Node::ArrayAccess* node)
    {
        enter("ArrayAccess", node);
        m_Callback(indent() << "In:" << node->getResult());
        m_Callback(indent() << "Array:");
        node->getArray()->visit(this);
        m_Callback(indent() << "Index:");
        node->getIndex()->visit(this);
        leave();
    }

    virtual void visit(Node::Constant* node)
    {
        auto& value = node->getConstant();
        enter("Constant", node);

        switch(value.getType())
        {
        case Pex::ValueType::None:
        {
           m_Callback(indent() << "None:none");
        }
            break;
        case Pex::ValueType::Identifier:
        {
            m_Callback(indent() << "Identifier:" << value.getId());
        }
            break;
        case Pex::ValueType::String:
        {
            m_Callback(indent() << "String:" << value.getString());
        }
            break;
        case Pex::ValueType::Integer:
        {
            m_Callback(indent() << "Integer:" << value.getInteger());
        }
            break;
        case Pex::ValueType::Float:
        {
            m_Callback(indent() << "Float:" <<  std::showpoint << value.getFloat());
        }
            break;
        case Pex::ValueType::Bool:
        {
            m_Callback(indent() << "Bool:" << (value.getBool()?"true":"false"));
        }
            break;
        }
        leave();
    }

    virtual void visit(Node::IdentifierString* node)
    {
        enter("IdentifierString", node);
        m_Callback(indent() << "Id:" << node->getIdentifier());
        leave();
    }

    virtual void visit(Node::While* node)
    {
        enter("While", node);
        m_Callback(indent() << "Condition:");
        node->getCondition()->visit(this);
        m_Callback(indent() << "Body:");
        node->getBody()->visit(this);
        leave();
    }

    virtual void visit(Node::IfElse* node)
    {
        enter("If", node);
        m_Callback(indent() << "Condition:");
        node->getCondition()->visit(this);
        m_Callback(indent() << "Body:");
        node->getBody()->visit(this);
        m_Callback(indent() << "ElseIf:");
        node->getElseIf()->visit(this);
        m_Callback(indent() << "Else:");
        node->getElse()->visit(this);
        leave();
    }

    virtual void visit(Node::Declare* node)
    {
        enter("Declare", node);
        m_Callback(indent() << "Type:" << node->getType());
        m_Callback(indent() << "Name:");
        node->getObject()->visit(this);
        leave();
    }
};

}
