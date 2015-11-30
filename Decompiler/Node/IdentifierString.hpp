#ifndef IDENTIFIERSTRING_HPP
#define IDENTIFIERSTRING_HPP

#include <string>
#include "Base.hpp"

namespace Node {

class IdentifierString : public Base
{
public:
    IdentifierString(size_t ip, const std::string& identifier);
    virtual ~IdentifierString();

    static std::shared_ptr<IdentifierString> make(size_t ip, const std::string& identifier);

    virtual void                             visit(Visitor* visitor);

    const std::string&                       getIdentifier() const;

protected:
    std::string m_Identifier;
};
}
#endif // IDENTIFIERSTRING_HPP
