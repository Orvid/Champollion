#ifndef NODECOMPARER_HPP
#define NODECOMPARER_HPP

#include "Base.hpp"
#include "Visitor.hpp"

namespace Node {

class NodeComparer :
        public Visitor
{
public:
    NodeComparer(BasePtr reference);
    virtual ~NodeComparer();

    bool getResult() const;

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
    virtual void visit(Node::ArrayCreate* node);
    virtual void visit(Node::ArrayLength* node);
    virtual void visit(Node::ArrayAccess* node);
    virtual void visit(Node::Constant* node);
    virtual void visit(Node::IdentifierString* node);
    virtual void visit(Node::While* node);
    virtual void visit(Node::IfElse* node);
    virtual void visit(Node::Declare* node);

protected:
    BasePtr m_Reference;
    DynamicVisitor m_Visitor;
    bool m_Result;
};


bool isSameTree(BasePtr left, BasePtr right);

}


#endif // NODECOMPARER_HPP
