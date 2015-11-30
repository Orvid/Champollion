#ifndef CONSTANT_HPP
#define CONSTANT_HPP

#include "Pex/Value.hpp"

#include "Base.hpp"


namespace Node {

class Constant : public Base
{
public:
    Constant(size_t ip, const Pex::Value& constant);
    virtual ~Constant();

    static std::shared_ptr<Constant> make(size_t ip, const Pex::Value& constant);

    virtual void                     visit(Visitor* visitor);

    const Pex::Value&            getConstant() const;

protected:
    Pex::Value m_Constant;
};

}

#endif // CONSTANT_HPP
