#ifndef PROPERTYACCESS_HPP
#define PROPERTYACCESS_HPP

#include "Base.hpp"
#include "FieldNodeMixin.hpp"


namespace Node {

class PropertyAccess : public Base,
        public FieldObjectNodeMixin<0>
{
public:
    PropertyAccess(size_t ip, const Pex::StringTable::Index& result, BasePtr object, const Pex::StringTable::Index& property);
    virtual ~PropertyAccess();

    static std::shared_ptr<PropertyAccess> make(size_t ip, const Pex::StringTable::Index& result, BasePtr object, const Pex::StringTable::Index& property);

    virtual void                           visit(Visitor* visitor);

    const Pex::StringTable::Index&     getProperty() const;

protected:
    Pex::StringTable::Index m_Property;

};
}

#endif // PROPERTYACCESS_HPP
