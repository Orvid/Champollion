#ifndef ARRAYCREATE_HPP
#define ARRAYCREATE_HPP

#include <cstdint>

#include "Base.hpp"
#include "FieldNodeMixin.hpp"

namespace Node {

class ArrayCreate : public Base
{
public:
    ArrayCreate(size_t ip, const Pex::StringTable::Index& result, const Pex::StringTable::Index& type, std::uint32_t size);
    virtual ~ArrayCreate();

    static std::shared_ptr<ArrayCreate> make(size_t ip, const Pex::StringTable::Index& result, const Pex::StringTable::Index& type, std::uint32_t size);

    virtual void                        visit(Visitor* visitor);

    const Pex::StringTable::Index&  getType() const;

    std::uint32_t                       getSize() const;

protected:
    Pex::StringTable::Index m_Type;
    std::uint32_t               m_Size;
};

}

#endif // ARRAYCREATE_HPP
