#pragma once

#include <cstdint>

#include "StringTable.hpp"

namespace Pex {

/**
 * @brief Base Mixin for named item.
 *
 * This mixin defines the name field for named elements
 */
class NamedItem
{
public:
    NamedItem();
    virtual ~NamedItem();

    StringTable::Index getName() const;
    void setName(StringTable::Index value);

protected:
    StringTable::Index m_Name;
};
}
