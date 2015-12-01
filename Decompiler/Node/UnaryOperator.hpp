#pragma once

#include <cassert>
#include <cstdint>
#include <memory>
#include <string>

#include "Base.hpp"
#include "FieldNodeMixin.hpp"
#include "Visitor.hpp"

namespace Node {

class UnaryOperator final :
    public Base,
    public FieldValueNodeMixin<0>
{
public:
    UnaryOperator(size_t ip, std::uint8_t precedence, const Pex::StringTable::Index& result, const std::string& op, BasePtr value) :
        Base(1, ip, precedence, result),
        FieldValueNodeMixin(this, value),
        m_Op(op)
    {
    }
    virtual ~UnaryOperator() = default;

    static std::shared_ptr<UnaryOperator> make(size_t ip, std::uint8_t precedence, const Pex::StringTable::Index& result, const std::string& op, BasePtr value)
    {
        return std::make_shared<UnaryOperator>(ip, precedence, result, op, value);
    }

    virtual void visit(Visitor* visitor)
    {
        assert(visitor);
        visitor->visit(this);
    }

    const std::string& getOperator() const { return m_Op; }

private:
    std::string m_Op;
};

}
