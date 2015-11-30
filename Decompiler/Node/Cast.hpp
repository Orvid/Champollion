#ifndef CAST_HPP
#define CAST_HPP

#include <string>

#include "Base.hpp"
#include "FieldNodeMixin.hpp"

namespace Node {

class Cast : public Base,
        public FieldValueNodeMixin<0>
{
public:
    Cast(size_t ip, const Pex::StringTable::Index& result, BasePtr value, const Pex::StringTable::Index& type);
    virtual ~Cast();

    static std::shared_ptr<Cast> make(size_t ip, const Pex::StringTable::Index& result, BasePtr value, const Pex::StringTable::Index& type);

    virtual void                            visit(Visitor* visitor);
    const Pex::StringTable::Index&      getType();


protected:
    Pex::StringTable::Index m_Type;
};

}
#endif // CAST_HPP
