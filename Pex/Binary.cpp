#include "Binary.hpp"

#include <iostream>

#include "FileReader.hpp"
/**
 * @brief Default constructor.
 *
 * This constructor provides a Binary file with empty elements.
 */
Pex::Binary::Binary()
{
}

/**
 * @brief Default destructor.
 *
 */
Pex::Binary::~Binary()
{
}

/**
 * @brief Retrieve the Header part of the Binary
 *
 * @return a const Header.
 */
const Pex::Header &Pex::Binary::getHeader() const
{
    return m_Header;
}

/**
 * @brief Retrieve the Header part of the Binary
 *
 * @return a modifiable Header.
 */
Pex::Header &Pex::Binary::getHeader()
{
    return m_Header;
}

/**
 * @brief Retrieve the StringTable associated with the Binary
 *
 * @return a const string table.
 */
const Pex::StringTable &Pex::Binary::getStringTable() const
{
    return m_StringTable;
}

/**
 * @brief Retrieve the StringTable associated with the Binary
 *
 * @return a modifiable string table.
 */
Pex::StringTable &Pex::Binary::getStringTable()
{
    return m_StringTable;
}

/**
 * @brief Retrieve the debug info associated with the Binary.
 *
 * @return a const DebugInfo
 */
const Pex::DebugInfo &Pex::Binary::getDebugInfo() const
{
    return m_DebugInfo;
}

/**
 * @brief Retrieve the debug info associated with the Binary.
 *
 * @return a modifiable DebugInfo
 */
Pex::DebugInfo &Pex::Binary::getDebugInfo()
{
    return m_DebugInfo;
}

/**
 * @brief Retrieve the user flags definition stored in the Binary
 *
 * @return a const UserFlags;
 */
const Pex::UserFlags &Pex::Binary::getUserFlags() const
{
    return m_UserFlags;
}

/**
 * @brief Retrieve the user flags definition stored in the Binary
 *
 * @return a modifiable UserFlags;
 */
Pex::UserFlags &Pex::Binary::getUserFlags()
{
    return m_UserFlags;
}

/**
 * @brief Retrieve the list of Objects defined in the Binary
 *
 * @return a const Objects.
 */
const Pex::Objects &Pex::Binary::getObjects() const
{
    return m_Objects;
}

/**
 * @brief Retrieve the list of Objects defined in the Binary
 *
 * @return a modifiable Objects.
 */
Pex::Objects &Pex::Binary::getObjects()
{
    return m_Objects;
}


