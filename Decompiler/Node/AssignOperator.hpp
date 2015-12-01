#pragma once

#include <cassert>
#include <cstdint>
#include <string>

#include "Base.hpp"
#include "FieldNodeMixin.hpp"
#include "Visitor.hpp"

namespace Node {

class AssignOperator final :
    public Base,
    public FieldValueNodeMixin<0>,
    public FieldDestinationNodeMixin<1>
{
public:
    AssignOperator(size_t ip, BasePtr destination, const std::string& op, BasePtr expr) :
        Base(2, ip, 10),
        FieldValueNodeMixin(this, expr),
        FieldDestinationNodeMixin(this, destination),
        m_Operator(op)
    {
    }
    virtual ~AssignOperator() = default;

    void visit(Visitor* visitor) override
    {
        assert(visitor);
        visitor->visit(this);
    }

    const std::string& getOperator() const { return m_Operator; }

private:
    std::string m_Operator;
};

}
