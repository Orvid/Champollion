#pragma once

#include <string>

#include "Header.hpp"
#include "StringTable.hpp"
#include "DebugInfo.hpp"
#include "UserFlag.hpp"
#include "Object.hpp"

namespace Pex {

/**
 * @brief Pex main data structure
 *
 * The Binary class reflect the content of a PEX file.
 *
 */
class FileReader;
class Binary
{
public:
    enum ScriptType {
        Unknown = -1,
        SkyrimScript,
        Fallout4Script,
        StarfieldScript
    };
    Binary();
    virtual ~Binary();

    const Header& getHeader() const;
    Header& getHeader();

    const StringTable& getStringTable() const;
    StringTable& getStringTable();

    const DebugInfo& getDebugInfo() const;
    DebugInfo& getDebugInfo();

    const UserFlags& getUserFlags() const;
    UserFlags& getUserFlags();

    const Objects& getObjects() const;
    Objects& getObjects();

    ScriptType getGameType() const;

    void sort();
    
protected:
    friend FileReader;
    void setScriptType(ScriptType game_type);
    Header m_Header;
    StringTable m_StringTable;
    DebugInfo m_DebugInfo;
    UserFlags m_UserFlags;
    Objects m_Objects;
    ScriptType m_ScriptType;
};
}
