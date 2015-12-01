#include "StringTable.hpp"

#include <algorithm>
#include <stdexcept>
#include <sstream>
#include <cassert>

/**
 * @brief Default constructor
 *
 * Builds an empty table.
 *
 */
Pex::StringTable::StringTable()
{
}

/**
 * @brief Default destructor
 */
Pex::StringTable::~StringTable()
{
}

/**
 * @brief Get the string using the index
 * @param index The index of the string to retrieve
 *
 * @return a const reference to the string
 */
const std::string &Pex::StringTable::operator [](std::uint16_t index) const
{
    return m_Table[index];
}

/**
 * @brief Get the string using the index
 * @param index The index of the string to retrieve
 *
 * @return a modifiable reference to the string
 */
std::string &Pex::StringTable::operator [](std::uint16_t index)
{
    return m_Table[index];
}

/**
 * @brief Get the Index object for the given indice
 * @param index The Indice to wrap
 *
 * @return a new Index pointing to the string.
 * @throws a std::runtime_error if the indice is outside the bounds.
 */
Pex::StringTable::Index Pex::StringTable::get(std::uint16_t index) const
{
    if( index < size())
    {
        return Index(this, index);
    }
    else if (index == Index::UNDEFINED)
    {
        return Index();
    }
    else
    {
        throw std::runtime_error("Invalid string index");
    }
}

/**
 * @brief Find a given string in the table
 *
 * The string is considered as an identifier, meaning that the search is case-insensitive.
 *
 * @param id Identifier to find
 * @return The index of the indentifier. The Index may be invalid if the string is not found.
 */
Pex::StringTable::Index Pex::StringTable::findIdentifier(const std::string &id) const
{
    auto it = std::find_if(m_Table.begin(), m_Table.end(), [&] (const std::string& item) {
        return _stricmp(item.c_str(), id.c_str()) == 0;
    });
    if (it == m_Table.end())
    {
        return Index();
    }
    else
    {
        return Index(this, it-begin());
    }
}

/**
 * @brief Get the begin iterator
 * @return the begin iterator
 */
Pex::StringTable::Table::iterator Pex::StringTable::begin()
{
    return m_Table.begin();
}

/**
 * @brief Get the begin iterator
 * @return the begin iterator
 */
Pex::StringTable::Table::const_iterator Pex::StringTable::begin() const
{
    return m_Table.begin();
}

/**
 * @brief Get the end iterator
 * @return the end iterator
 */
Pex::StringTable::Table::iterator Pex::StringTable::end()
{
    return m_Table.end();
}

/**
 * @brief Get the end iterator
 * @return the end iterator
 */
Pex::StringTable::Table::const_iterator Pex::StringTable::end() const
{
    return m_Table.end();
}

/**
 * @brief Insert a value at the end of the table
 * @param value The string to insert
 */
void Pex::StringTable::push_back(const std::string &value)
{
    m_Table.push_back(value);
}

/**
 * @brief Get the size of the table
 * @return teh size of the table
 */
size_t Pex::StringTable::size() const
{
    return m_Table.size();
}

/**
 * @brief Prepare the table for multiple insertion
 * @param size The expected size of the table after the insertions.
 */
void Pex::StringTable::reserve(size_t size)
{
    return m_Table.reserve(size);
}


/**
 * @brief Constructor
 * Build an index from the table and the index.
 * Only a StringTable object is able to build an index using this constructor.
 *
 * @param table
 * @param index
 */
Pex::StringTable::Index::Index(const Pex::StringTable *table, std::uint16_t index) :
    m_Table(table),
    m_Index(index)
{
    assert(isValid());
}

/**
 * @brief Default constructor
 *
 * Builds a default invalid index.
 */
Pex::StringTable::Index::Index() :
    m_Table(nullptr),
    m_Index()
{
}

/**
 * @brief Default destructor.
 */
Pex::StringTable::Index::~Index()
{
}

/**
 * @brief Default value for undefined indexes
 *
 */
static std::string UNDEFINED_STRING = "undefined";

/**
 * @brief Get the string associated with the index.
 *
 * @return the string.
 */
const std::string &Pex::StringTable::Index::asString() const
{
    assert(isValid());
    if (m_Index != UNDEFINED)
    {
        return m_Table->operator [](m_Index);
    }
    else
    {
        return UNDEFINED_STRING;
    }
}

/**
 * @brief Get string contatenated with the index
 * This function is mainly used for debugging purposes.
 *
 * @return the string and index.
 */

std::string Pex::StringTable::Index::asStringWithIndex() const
{
    assert(isValid());
    std::ostringstream stream;
    stream << asString();
    stream  << "[" << m_Index << "]";
    return stream.str();
}

/**
 * @brief Get the numeric value of the index
 * @return the value of the index.
 */
std::uint16_t Pex::StringTable::Index::asIndex() const
{
    assert(isValid());
    return m_Index;
}

/**
 * @brief Get the table associated with the index
 * @return a pointer to the table. nullptr if the index is invalid.
 */
const Pex::StringTable* Pex::StringTable::Index::getTable() const
{
    return m_Table;
}

/**
 * @brief Check if the index is a valid one
 * @return True if the index can be used to get a string.
 */
bool Pex::StringTable::Index::isValid() const
{
    return (m_Index == UNDEFINED) || (m_Table != nullptr && m_Index < m_Table->size());
}

/**
 * @brief Check if the index is undefined.
 * An index is undefined if it point to a table but not to a string.
 * @return True if the index
 */
bool Pex::StringTable::Index::isUndefined() const
{
    return isValid() && m_Index == UNDEFINED;
}

/**
 * @brief Check if two indexes are equals.
 * @param rhs The index to compare to.
 * @return True is both indexes points to the same string
 */
bool Pex::StringTable::Index::operator ==(const Pex::StringTable::Index &rhs) const
{
    return m_Table == rhs.m_Table && m_Index == rhs.m_Index;
}

/**
 * @brief Check if two indexes are differents.
 * @param rhs The index to compare to.
 * @return True is the indexes points to different strings.
 */
bool Pex::StringTable::Index::operator !=(const Pex::StringTable::Index &rhs) const
{
    return m_Table != rhs.m_Table || m_Index != rhs.m_Index;
}

