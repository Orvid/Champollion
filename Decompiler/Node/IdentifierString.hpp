#pragma once

#include <cassert>
#include <cstdint>
#include <algorithm>
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
      std::replace(m_Identifier.begin(), m_Identifier.end(), '#', ':');
    }
    virtual ~IdentifierString() = default;

    void visit(Visitor* visitor) override
    {
        assert(visitor);
        visitor->visit(this);
    }

    const std::string& getIdentifier() const { return m_Identifier; }

private:
    std::string m_Identifier;
};

}
