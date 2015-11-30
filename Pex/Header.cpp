#include "Header.hpp"

#include <iostream>

/**
 * @brief Default constructor
 */
Pex::Header::Header() :
    m_MajorVersion(0),
    m_MinorVersion(0),
    m_CompilationTime(0),
    m_GameID(0)
{
}

/**
 * @brief Retrieve the major version number of the file
 * @return the major version number
 */
std::uint8_t Pex::Header::getMajorVersion() const
{
    return m_MajorVersion;
}

/**
 * @brief Sets the major version number
 * @param value minor version number
 */
void Pex::Header::setMajorVersion(std::uint8_t value)
{
    m_MajorVersion = value;
}


/**
 * @brief Retrieve the minor version number of the file
 * @return the minor version number
 */
std::uint8_t Pex::Header::getMinorVersion() const
{
    return m_MinorVersion;
}

/**
 * @brief Sets the minor version number
 * @param value minor version number
 */
void Pex::Header::setMinorVersion(std::uint8_t value)
{
    m_MinorVersion = value;
}

/**
 * @brief Retrieve the compilation date/time.
 * @return the compilation date.
 */
const std::time_t& Pex::Header::getCompilationTime() const
{
    return m_CompilationTime;
}

/**
 * @brief Set the compilation date/time
 * @param value The compilation date.
 */
void Pex::Header::setCompilationTime(const std::time_t& value)
{
    m_CompilationTime = value;
}

/**
 * @brief Get the game id
 * @return the game id;
 */
std::uint16_t Pex::Header::getGameID() const
{
    return m_GameID;
}

/**
 * @brief Set the game id;
 * @param value the new game id;
 */
void Pex::Header::setGameID(std::uint16_t value)
{
    m_GameID = value;
}

/**
 * @brief Retrieve the name of the original source file.
 * @return the name of the file
 */
std::string Pex::Header::getSourceFileName() const
{
    return m_SourceFileName;
}

/**
 * @brief Set the name of the original source file.
 * @param value The name of the file.
 */
void Pex::Header::setSourceFileName(const std::string &value)
{
    m_SourceFileName = value;
}

/**
 * @brief Retrieve the name of the user which compiled the source.
 * @return the user name
 */
std::string Pex::Header::getUserName() const
{
    return m_UserName;
}

/**
 * @brief Set the name of the user which compiled the source.
 * @param value the user name
 */
void Pex::Header::setUserName(const std::string &value)
{
    m_UserName = value;
}

/**
 * @brief Retrieve the name of the computer which compiled the source
 * @return the computer name
 */
std::string Pex::Header::getComputerName() const
{
    return m_ComputerName;
}

/**
 * @brief Set the name of the computer which compiled the source
 * @param value the computer name
 */
void Pex::Header::setComputerName(const std::string &value)
{
    m_ComputerName = value;
}

