#include "Binary.hpp"

#include <algorithm>
#include <iostream>

#include "FileReader.hpp"
/**
 * @brief Default constructor.
 *
 * This constructor provides a Binary file with empty elements.
 */
Pex::Binary::Binary():
    m_ScriptType(Unknown)
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

Pex::Binary::ScriptType Pex::Binary::getGameType() const
{
    return m_ScriptType;
}

void Pex::Binary::setScriptType(Pex::Binary::ScriptType game_type)
{
    m_ScriptType = game_type;
}

static bool namedLessThan(const Pex::NamedItem& a, const Pex::NamedItem& b) {
    return a.getName().asString() < b.getName().asString();
}

void Pex::Binary::sort() {
    std::sort(m_Objects.begin(), m_Objects.end(), namedLessThan);
    std::sort(m_UserFlags.begin(), m_UserFlags.end(), namedLessThan);
    for (auto& obj : m_Objects) {
        std::sort(obj.getGuards().begin(), obj.getGuards().end(), namedLessThan);
        std::sort(obj.getProperties().begin(), obj.getProperties().end(), namedLessThan);
        std::sort(obj.getStates().begin(), obj.getStates().end(), namedLessThan);
        std::sort(obj.getStructInfos().begin(), obj.getStructInfos().end(), namedLessThan);
        std::sort(obj.getVariables().begin(), obj.getVariables().end(), namedLessThan);

        for (auto& state : obj.getStates()) {
            std::sort(state.getFunctions().begin(), state.getFunctions().end(), [this, obj, state](const Pex::Function& a, const Pex::Function& b) {
                auto linesA = this->getDebugInfo().getFunctionInfo(obj.getName(), state.getName(), a.getName());
                auto lA = !linesA || linesA->getLineNumbers().empty() ? 0 : linesA->getLineNumbers()[0];
                auto linesB = this->getDebugInfo().getFunctionInfo(obj.getName(), state.getName(), b.getName());
                auto lB = !linesB || linesB->getLineNumbers().empty() ? 0 : linesB->getLineNumbers()[0];
                if (lA == lB) {
                    return a.getName().asString() < b.getName().asString();
                }
                return lA < lB;
            });
        }
    }
}