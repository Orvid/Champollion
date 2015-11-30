#ifndef SCOPE_HPP
#define SCOPE_HPP

#include "Base.hpp"

namespace Node {

class Scope : public Base
{
public:
    Scope();
    virtual ~Scope();

    static std::shared_ptr<Scope> make();

    virtual void    visit(Visitor* visitor);

protected:
};

}
#endif // SCOPE_HPP
