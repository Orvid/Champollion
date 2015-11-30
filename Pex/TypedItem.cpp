#include "TypedItem.hpp"
#include <cassert>


/**
 * @brief Default constructor
 */
Pex::TypedItem::TypedItem()
{
}

/**
 * @brief Default destructor
 */
Pex::TypedItem::~TypedItem()
{
}

/**
 * @brief Get the type name
 * @return The type name index
 */
Pex::StringTable::Index Pex::TypedItem::getTypeName() const
{
    return m_TypeName;
}

/**
 * @brief Set the type name
 * @param value The new type name.
 */
void Pex::TypedItem::setTypeName(Pex::StringTable::Index value)
{
    assert(value.isValid());
    m_TypeName = value;
}
