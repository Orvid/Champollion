#include "DocumentedItem.hpp"

#include <cassert>

/**
 * @brief Default constructor
 *
 */
Pex::DocumentedItem::DocumentedItem()
{
}

/**
 * @brief Default destructor
 *
 */
Pex::DocumentedItem::~DocumentedItem()
{
}

/**
 * @brief Retrieve the document string describing the item.
 *
 * @return the docstring index.
 */
Pex::StringTable::Index Pex::DocumentedItem::getDocString() const
{
    return m_DocString;
}

/**
 * @brief Sets the document string describing the item.
 * @param[in] value Index of the docstring
 */
void Pex::DocumentedItem::setDocString(Pex::StringTable::Index value)
{
    assert(value.isValid());
    m_DocString = value;
}
