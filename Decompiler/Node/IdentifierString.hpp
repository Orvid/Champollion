#pragma once

#include <cassert>
#include <cstdint>
#include <memory>
#include <string>

#include "Base.hpp"
#include "Visitor.hpp"

namespace Node {

class IdentifierString final : public Base
{
public:
    IdentifierString(size_t ip, const std::string& identifier) :
        Base(0, ip, 0),
        m_Identifier(identifier)
    {
    }
    virtual ~IdentifierString() = default;

    static std::shared_ptr<IdentifierString> make(size_t ip, const std::string& identifier)
    {
        return std::make_shared<IdentifierString>(ip, identifier);
    }

    virtual void visit(Visitor* visitor)
    {
        assert(visitor);
        visitor->visit(this);
    }

    const std::string& getIdentifier() const { return m_Identifier; }

private:
    std::string m_Identifier;
};

}
