#pragma once

#include <cstdint>

#include "StringTable.hpp"

namespace Pex {
/**
 * @brief Base mixin for documented items, such as property or functions
 *
 */
class DocumentedItem
{
public:
    DocumentedItem();
    virtual ~DocumentedItem();

    StringTable::Index getDocString() const;
    void setDocString(StringTable::Index value);

protected:
    StringTable::Index m_DocString;
};
}
