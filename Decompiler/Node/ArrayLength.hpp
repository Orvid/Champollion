#ifndef ARRAYLENGTH_HPP
#define ARRAYLENGTH_HPP


#include <string>

#include "Base.hpp"
#include "FieldNodeMixin.hpp"

namespace Node {

class ArrayLength : public Base,
        public FieldArrayNodeMixin<0>
{
public:
    ArrayLength(size_t ip, const Pex::StringTable::Index& result, BasePtr array);
    virtual ~ArrayLength();

    static std::shared_ptr<ArrayLength> make(size_t ip, const Pex::StringTable::Index& result, BasePtr array);

    virtual void                        visit(Visitor* visitor);

protected:

};

}
#endif // ARRAYLENGTH_HPP
