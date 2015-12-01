#pragma once

#include <memory>
#include <functional>

#include "Base.hpp"

namespace Node {

#define FOR_EACH_NODE_CLASS() \
    DO_NODE(Scope) \
    DO_NODE(BinaryOperator) \
    DO_NODE(UnaryOperator) \
    DO_NODE(Assign) \
    DO_NODE(AssignOperator) \
    DO_NODE(Cast) \
    DO_NODE(CallMethod) \
    DO_NODE(Params) \
    DO_NODE(Copy) \
    DO_NODE(Return) \
    DO_NODE(PropertyAccess) \
    DO_NODE(ArrayCreate) \
    DO_NODE(ArrayLength) \
    DO_NODE(ArrayAccess) \
    DO_NODE(Constant) \
    DO_NODE(IdentifierString) \
    DO_NODE(While) \
    DO_NODE(IfElse) \
    DO_NODE(Declare) \
    DO_NODE(StructCreate) \

#define DO_NODE(NODE) class NODE;
FOR_EACH_NODE_CLASS()
#undef DO_NODE

class Visitor
{
public:
    Visitor() = default;
    virtual ~Visitor() = default;

#define DO_NODE(NODE) virtual void visit(NODE* node) = 0;
    FOR_EACH_NODE_CLASS()
#undef DO_NODE

public:
    void visitChildren(Base* node);
};

class VisitorBase : public Visitor
{
public:
    VisitorBase() = default;
    virtual ~VisitorBase() = default;

#define DO_NODE(NODE) virtual void visit(NODE* node);
    FOR_EACH_NODE_CLASS()
#undef DO_NODE
};

class DynamicVisitor : public VisitorBase
{
public:    
#define DO_NODE(NODE) \
    typedef std::function<void (NODE* node, DynamicVisitor* visitor)> Lambda##NODE;

    FOR_EACH_NODE_CLASS()
#undef DO_NODE

    typedef std::function<void (Base* node, DynamicVisitor* visitor)> LambdaCommon;

public:
    DynamicVisitor();
    virtual ~DynamicVisitor() = default;

    DynamicVisitor& common(LambdaCommon common);

#define DO_NODE(NODE) virtual void visit(NODE* node);
    FOR_EACH_NODE_CLASS()
#undef DO_NODE

#define DO_NODE(NODE) DynamicVisitor& operator<<(Lambda##NODE function);
    FOR_EACH_NODE_CLASS()
#undef DO_NODE

#define DO_NODE(NODE) static const Lambda##NODE Clear##NODE;
    FOR_EACH_NODE_CLASS()
#undef DO_NODE

protected:
#define DO_NODE(NODE) Lambda##NODE m_On##NODE;
    FOR_EACH_NODE_CLASS()
#undef DO_NODE

    LambdaCommon m_OnCommon;
};

}
