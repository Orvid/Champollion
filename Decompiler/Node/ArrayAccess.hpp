#ifndef ARRAYACCESS_HPP
#define ARRAYACCESS_HPP

#include "Base.hpp"
#include "FieldNodeMixin.hpp"


namespace Node {

class ArrayAccess : public Base,
        public FieldArrayNodeMixin<0>,
        public FieldIndexNodeMixin<1>
{
public:
    ArrayAccess(size_t ip, const Pex::StringTable::Index& result, BasePtr object, BasePtr index);
    virtual ~ArrayAccess();

    static std::shared_ptr<ArrayAccess> make(size_t ip, const Pex::StringTable::Index& result, BasePtr object, BasePtr index);

    virtual void                        visit(Visitor* visitor);


};
}
#endif // ARRAYACCESS_HPP
