#ifndef CALLMETHOD_HPP
#define CALLMETHOD_HPP

#include "Base.hpp"
#include "FieldNodeMixin.hpp"

#include "Params.hpp"

namespace Node {
class CallMethod : public Base,
        public FieldObjectNodeMixin<0>,
        public FieldParametersNodeMixin<1>
{
public:
    CallMethod(size_t ip, const Pex::StringTable::Index& result, BasePtr& object, const Pex::StringTable::Index& method);
    virtual ~CallMethod();

    static std::shared_ptr<CallMethod> make(size_t ip, const Pex::StringTable::Index& result, BasePtr object, const Pex::StringTable::Index& method);

    virtual void                       visit(Visitor* visitor);

    const Pex::StringTable::Index& getMethod() const;

public:
    Pex::StringTable::Index m_Method;
};
}

#endif // CALLMETHOD_HPP
