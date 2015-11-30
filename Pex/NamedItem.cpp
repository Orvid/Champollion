#include "NamedItem.hpp"

#include <cassert>

/**
 * @brief Default constructor
 */
Pex::NamedItem::NamedItem()
{
}

/**
 * @brief Default destructor
 */
Pex::NamedItem::~NamedItem()
{
}

/**
 * @brief Get the name of the item
 * @return a name index
 */
Pex::StringTable::Index Pex::NamedItem::getName() const
{
    return m_Name;
}

/**
 * @brief Sets the name of the item
 * @param value The new name index
 */
void Pex::NamedItem::setName(Pex::StringTable::Index value)
{
    assert(value.isValid());
    m_Name = value;
}
