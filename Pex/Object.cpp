#include "Object.hpp"
#include <cassert>

/**
 * @brief Default constructor
 *
 * Creates an empty object.
 */
Pex::Object::Object()
{
}

/**
 * @brief Deafaul destructor
 */
Pex::Object::~Object()
{
}

/**
 * @brief Get the name of the parent class, if one.
 * @return the name index of the class
 */
Pex::StringTable::Index Pex::Object::getParentClassName() const
{
    return m_ParentClassName;
}

/**
 * @brief Set the name of the parent class
 * @param value The new parent class name index.
 */
void Pex::Object::setParentClassName(StringTable::Index value)
{
    assert(value.isValid());
    m_ParentClassName = value;
}

/**
 * @brief Get the name of the default state.
 * @return The default name index
 */
Pex::StringTable::Index Pex::Object::getAutoStateName() const
{
    return m_AutoStateName;
}

/**
 * @brief Set the name of the default state
 * @param value The default state name index
 */
void Pex::Object::setAutoStateName(StringTable::Index value)
{
    assert(value.isValid());
    m_AutoStateName = value;
}

/**
 * @brief Get the variables list
 * @return The const variables list
 */
const Pex::Variables &Pex::Object::getVariables() const
{
    return m_Variables;
}

/**
 * @brief Get the variables list
 * @return The modifiable variables list
 */
Pex::Variables &Pex::Object::getVariables()
{
    return m_Variables;
}

/**
 * @brief Get the properties list
 * @return The const properties list
 */
const Pex::Properties &Pex::Object::getProperties() const
{
    return m_Properties;
}

/**
 * @brief Get the properties list
 * @return The modifiable properties list
 */
Pex::Properties &Pex::Object::getProperties()
{
    return m_Properties;
}


/**
 * @brief Get the states list
 * @return The const states list
 */
const Pex::States &Pex::Object::getStates() const
{
    return m_States;
}

/**
 * @brief Get the states list
 * @return The const states list
 */
Pex::States &Pex::Object::getStates()
{
    return m_States;
}
