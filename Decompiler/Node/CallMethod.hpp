#pragma once

#include <cassert>
#include <cstdint>
#include <memory>

#include "Base.hpp"
#include "FieldNodeMixin.hpp"
#include "Params.hpp"
#include "Visitor.hpp"

namespace Node {
class CallMethod final :
    public Base,
    public FieldObjectNodeMixin<0>,
    public FieldParametersNodeMixin<1>
{
public:
    CallMethod(size_t ip, const Pex::StringTable::Index& result, BasePtr object, const Pex::StringTable::Index& method, const bool experimental = false) :
        Base(2, ip, 0, result),
        FieldObjectNodeMixin(this, object),
        FieldParametersNodeMixin(this, std::make_shared<Params>()),
        m_Method(method),
        m_Experimental(experimental)
    {
    }
    virtual ~CallMethod() = default;

    void visit(Visitor* visitor) override
    {
        assert(visitor);
        visitor->visit(this);
    }

    bool isExperimentalSyntax(){
        return m_Experimental;
    }

    const Pex::StringTable::Index& getMethod() const { return m_Method; }

private:
    Pex::StringTable::Index m_Method;
    bool m_Experimental;
};

}
