#ifndef TYPEDITEM_HPP
#define TYPEDITEM_HPP

#include "Pex_global.hpp"

#include <cstdint>

#include "StringTable.hpp"

namespace Pex {
/**
 * @brief Base Mixin for typed item.
 *
 * This mixin defines the type name name field for typed elements
 */
class PEX_API TypedItem
{
public:
    TypedItem();
    virtual ~TypedItem();

    StringTable::Index getTypeName() const;
    void setTypeName(StringTable::Index value);

protected:
    StringTable::Index m_TypeName;
};
}
#endif // TYPEDITEM_HPP
