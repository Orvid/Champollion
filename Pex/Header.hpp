#pragma once

#include <cstdint>
#include <ctime>
#include <string>

namespace Pex {
/**
 * @brief Header file content.
 *
 * The Header class contains the data found in the header of a PEX file, such as compilation date
 * or user/computer which compiled the binary
 *
 */
class Header
{
public:
    Header();

    std::uint8_t getMajorVersion() const;
    void setMajorVersion(std::uint8_t value);

    std::uint8_t getMinorVersion() const;
    void setMinorVersion(std::uint8_t value);

    const std::time_t& getCompilationTime() const;
    void setCompilationTime(const std::time_t& value);

    std::uint16_t getGameID() const;
    void setGameID(std::uint16_t value);

    std::string getSourceFileName() const;
    void setSourceFileName(const std::string& value);

    std::string getUserName() const;
    void setUserName(const std::string& value);

    std::string getComputerName() const;
    void setComputerName(const std::string& value);


protected:
    std::uint8_t m_MajorVersion;
    std::uint8_t m_MinorVersion;
    std::time_t m_CompilationTime;
    std::uint16_t m_GameID;
    std::string m_SourceFileName;
    std::string m_UserName;
    std::string m_ComputerName;
};
}
