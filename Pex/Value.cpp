#include "Value.hpp"

#include <stdexcept>
#include <cassert>
#include <sstream>
#include <iomanip>


/**
 * @brief Constructor for None value.
 *
 * Builds a value object containing the None value.
 */
Pex::Value::Value() :
    m_Type(ValueType::None)
{
}

/**
 * @brief Constructor for string or identifier value
 *
 * Builds a value object containing a string or an identifier.
 *
 * @param value Index of the string in the String Table.
 * @param id True specifying if the string is an identifier.
 */
Pex::Value::Value(const StringTable::Index& value, bool id)
{
    m_Type = (id)? ValueType::Identifier : ValueType::String;
    m_Value.string.table = value.getTable();
    m_Value.string.index = value.asIndex();
}

/**
 * @brief Constructor for an integer value.
 * @param value Value to store in the object.
 */
Pex::Value::Value(std::int32_t value) :
    m_Type(ValueType::Integer)
{
    m_Value.integer = value;
}

/**
 * @brief Constructor for a real value.
 * @param value Value to store in the object
 */
Pex::Value::Value(float value) :
    m_Type(ValueType::Float)
{
    m_Value.real = value;
}

/**
 * @brief Constructor for a bool value.
 * @param value Value to store in the object
 */

Pex::Value::Value(bool value) :
    m_Type(ValueType::Bool)
{
    m_Value.boolean = value;
}

/**
 * @brief Default destructor
 */
Pex::Value::~Value()
{
}

/**
 * @brief Get the type of the value.
 * @return The enum value defining the type contained in the value.
 */
Pex::ValueType Pex::Value::getType() const
{
    return m_Type;
}

/**
 * @brief Check if the value is None.
 * @return True of the value is None.
 */
bool Pex::Value::isNone() const
{
    return m_Type == ValueType::None;
}

/**
 * @brief Set the value to None.
 */
void Pex::Value::clear()
{
    m_Type = ValueType::None;
}

/**
 * @brief Get the value as an identifier.
 * @return The identifier string index.
 */
Pex::StringTable::Index Pex::Value::getId() const
{
    ensureType(ValueType::Identifier);
    return (m_Value.string.table)?m_Value.string.table->get(m_Value.string.index): StringTable::Index();
}

/**
 * @brief Set the value to an identifier
 * @param value The value to store
 */
void Pex::Value::setId(const StringTable::Index &value)
{
    assert(value.isValid());
    m_Type = ValueType::Identifier;
    m_Value.string.table = value.getTable();
    m_Value.string.index = value.asIndex();
}

/**
 * @brief Get the value as a string.
 * @return The string index.
 */
Pex::StringTable::Index Pex::Value::getString() const
{
    ensureType(ValueType::String);
    return (m_Value.string.table)?m_Value.string.table->get(m_Value.string.index): StringTable::Index();
}

/**
 * @brief Set the value to a string.
 * @param value The value to store.
 */
void Pex::Value::set(const StringTable::Index& value)
{
    assert(value.isValid());
    m_Type = ValueType::String;
    m_Value.string.table = value.getTable();
    m_Value.string.index = value.asIndex();
}

/**
 * @brief Get the value as an integer.
 * @return The integer value.
 */
std::int32_t Pex::Value::getInteger() const
{
    ensureType(ValueType::Integer);
    return m_Value.integer;
}

/**
 * @brief Set the value to an integer.
 * @param value The value to store.
 */
void Pex::Value::set(std::int32_t value)
{
    m_Type = ValueType::Integer;
    m_Value.integer = value;
}

/**
 * @brief Get the value as a float.
 * @return The float value.
 */
float Pex::Value::getFloat() const
{
    ensureType(ValueType::Float);
    return m_Value.real;
}

/**
 * @brief Set the value to a float.
 * @param value The value to store.
 */
void Pex::Value::set(float value)
{
    m_Type = ValueType::Float;
    m_Value.real = value;
}

/**
 * @brief Get the value as a boolean.
 * @return The boolean value.
 */
bool Pex::Value::getBool() const
{
    ensureType(ValueType::Bool);
    return m_Value.boolean;
}

/**
 * @brief Set the value as a boolean.
 * @param value The value to store.
 */
void Pex::Value::set(bool value)
{
    m_Type = ValueType::Bool;
    m_Value.boolean = value;
}

/**
 * @brief Check if two value are equals.
 * @param rhs The value to compare to.
 * @return True if the two values are equals.
 */
bool Pex::Value::operator ==(const Pex::Value &rhs) const
{
    if(m_Type != rhs.m_Type)
    {
        return false;
    }
    else
    {
        switch (m_Type) {
        case ValueType::None:
            return true;
        case ValueType::Identifier:
            return m_Value.string.table == rhs.m_Value.string.table && m_Value.string.index == rhs.m_Value.string.index;
        case ValueType::String:
            return m_Value.string.table == rhs.m_Value.string.table && m_Value.string.index == rhs.m_Value.string.index;
        case ValueType::Float:
            return m_Value.real == rhs.m_Value.real;
        case ValueType::Integer:
            return m_Value.integer == rhs.m_Value.integer;
        case ValueType::Bool:
            return m_Value.boolean == rhs.m_Value.boolean;
        default:
            return false;
        }
    }
}

/**
 * @brief Convert the value to a string.
 * @return the value as a Papyrus literal.
 */
std::string Pex::Value::toString() const
{
    std::ostringstream result;

    switch(getType())
    {
        case Pex::ValueType::None:
        {
            result << "None";
            break;
        }
        case Pex::ValueType::Identifier:
        {
            result << getId().asString();
            break;
        }
        case Pex::ValueType::String:
        {
            result << '"';
            for (auto c : getString().asString())
            {
                switch(c)
                {
                case '\n':
                    result << "\\n";
                    break;
                case '\t':
                    result << "\\t";
                    break;
                case '\\':
                    result << "\\\\";
                    break;
                case '\"':
                    result << "\\\"";
                    break;
                default:
                    result << c;
                    break;
                }
            }
            result << '"';
            break;
        }
        case Pex::ValueType::Integer:
        {
            result << getInteger();
            break;
        }
        case Pex::ValueType::Float:
        {
            result << std::showpoint << std::defaultfloat << getFloat();

            // Prevent a point to be the last character.
            if (result.str().back() == '.') {
                result << "0";
            }

            auto str = result.str();
            size_t i = str.length() - 1;
            for (; i >= 0; i--) {
                if (str[i] != '0') {
                    if (str[i] == '.')
                        i--;
                    break;
                }
            }
            return str.substr(0, i + 1);
        }
        case Pex::ValueType::Bool:
            result << (getBool() ? "True" : "False");
            break;
        default:
            result << "*error invalid value type*";
            break;
    }

    return result.str();
}

/**
 * @brief Ensure the type of the value.
 * Check if the value is of the given type or throw an exception if not.
 * @param type The type enum
 */
void Pex::Value::ensureType(Pex::ValueType type) const
{
    if (m_Type != type)
    {
        throw std::runtime_error("Invalid type");
    }
}
