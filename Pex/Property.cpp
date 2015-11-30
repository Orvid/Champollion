#include "Property.hpp"

#include <cassert>

/**
 * @brief Default constructor
 *
 * Creates an empty property
 */
Pex::Property::Property() :
    m_Flags(PropertyFlag::NO_FLAGS)
{
}
/**
 * @brief Default destructor
 */
Pex::Property::~Property()
{
}

/**
 * @brief Get the name of the associated variable, if one
 * @return the name index of the auto variable
 */
Pex::StringTable::Index Pex::Property::getAutoVarName() const
{
    return m_AutoVarName;
}

/**
 * @brief Set the name of the associated auto variable.
 * @param value The name index of the assoiated variable.
 */
void Pex::Property::setAutoVarName(Pex::StringTable::Index value)
{
    assert(value.isValid());
    m_AutoVarName = value;
}

/**
 * @brief Sets the flags associated with the property
 * @param value The new flag value.
 */
void Pex::Property::setFlags(Pex::PropertyFlag value)
{
    m_Flags = value;
}

/**
 * @brief Check if the property is readable
 * @return true if the property is readable
 */
bool Pex::Property::isReadable() const
{
    return (m_Flags & PropertyFlag::READ) != PropertyFlag::NO_FLAGS;
}

/**
 * @brief Check if the property is writable
 * @return true if the property is writable
 */
bool Pex::Property::isWritable() const
{
    return (m_Flags & PropertyFlag::WRITE) != PropertyFlag::NO_FLAGS;
}

/**
 * @brief Check if the property is auto
 * @return true if the property is auto
 */
bool Pex::Property::hasAutoVar() const
{
    return (m_Flags & PropertyFlag::AUTOVAR) != PropertyFlag::NO_FLAGS;
}

/**
 * @brief Get the getter function.
 * @return a const function.
 */
const Pex::Function &Pex::Property::getReadFunction() const
{
    assert(isReadable() && !hasAutoVar());
    return m_ReadFunction;
}

/**
 * @brief Get the getter function.
 * @return a modifiable function.
 */
Pex::Function &Pex::Property::getReadFunction()
{
    assert(isReadable() && !hasAutoVar());
    return m_ReadFunction;
}

/**
 * @brief Get the setter function.
 * @return a const function.
 */
const Pex::Function &Pex::Property::getWriteFunction() const
{
    assert(isWritable() && !hasAutoVar());
    return m_WriteFunction;
}

/**
 * @brief Get the setter function.
 * @return a modifiable function.
 */
Pex::Function &Pex::Property::getWriteFunction()
{
    assert(isWritable() && !hasAutoVar());
    return m_WriteFunction;
}
