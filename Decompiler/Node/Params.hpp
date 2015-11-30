#ifndef PARAMS_HPP
#define PARAMS_HPP

#include "Base.hpp"

namespace Node {

class Params : public Base
{
public:
    Params();
    virtual ~Params();

    static std::shared_ptr<Params> make();

    virtual void    visit(Visitor* visitor);

protected:
};
}

#endif // PARAMS_HPP
