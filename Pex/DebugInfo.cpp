#include "DebugInfo.hpp"

#include <algorithm>

/**
 * @brief Default constructor
 *
 * Creates an empty debug info object
 */
Pex::DebugInfo::DebugInfo() :
    m_ModificationTime(0)
{
}

/**
 * @brief Default desctrutor
 *
 */
Pex::DebugInfo::~DebugInfo()
{
}

/**
 * @brief Retrieve the modification time of the original source file.
 *
 * The modification time is 0 if no debug information are defined in the PEX file.
 *
 * @return the original modification time.
 */
const std::time_t &Pex::DebugInfo::getModificationTime() const
{
    return m_ModificationTime;
}

/**
 * @brief Sets the modification time of the original source file.
 *
 * @param[in] value Modification time to set.
 */
void Pex::DebugInfo::setModificationTime(const std::time_t &value)
{
    m_ModificationTime = value;
}

/**
 * @brief Retrieve the function info list.
 *
 * @return a const FunctionInfo collection.
 */
const Pex::DebugInfo::FunctionInfos &Pex::DebugInfo::getFunctionInfos() const
{
    return m_FunctionInfo;
}

/**
 * @brief Retrieve the function info list.
 *
 * @return a modifiable FunctionInfo collection.
 */
Pex::DebugInfo::FunctionInfos &Pex::DebugInfo::getFunctionInfos()
{
    return m_FunctionInfo;
}

/**
 * @brief Get the function info for a given function.
 * @param object Name of the object containing the function.
 * @param state Name of the state containing the function.
 * @param name Name of the function.
 * @param type Type of the function (0 normal function, 1 for property getter, 2 for property setter)
 * @return A pointer to the function infos, nulltpr if not found.
 */
const Pex::DebugInfo::FunctionInfo* Pex::DebugInfo::getFunctionInfo(const Pex::StringTable::Index &object, const Pex::StringTable::Index &state, const Pex::StringTable::Index &name, FunctionType type) const
{
    auto it = std::find_if(m_FunctionInfo.begin(), m_FunctionInfo.end(), [&] (FunctionInfos::const_reference item) {
            return item.getObjectName() == object && item.getStateName() == state && item.getFunctionName() == name && item.getFunctionType() == type;
    });
    if (it == m_FunctionInfo.end())
    {
        return nullptr;
    }
    return &(*it);
}

/**
 * @brief Default constructor
 */
Pex::DebugInfo::FunctionInfo::FunctionInfo()
{
}

/**
 * @brief Default Destructor.
 */
Pex::DebugInfo::FunctionInfo::~FunctionInfo()
{
}

/**
 * @brief Retrieve the object name associated with this function debug info
 * @return the string index of the object's name
 */
Pex::StringTable::Index Pex::DebugInfo::FunctionInfo::getObjectName() const
{
    return m_ObjectName;
}

/**
 * @brief Sets the name of the object associated with this function debug info
 *
 * @param[in] value String index of the object's name.
 */
void Pex::DebugInfo::FunctionInfo::setObjectName(StringTable::Index value)
{
    m_ObjectName = value;
}

/**
 * @brief Retrieve the state name associated with this function debug info
 * @return the string index of the state's name
 */
Pex::StringTable::Index Pex::DebugInfo::FunctionInfo::getStateName() const
{
    return m_StateName;
}

/**
 * @brief Sets the name of the state associated with this function debug info
 *
 * @param[in] value String index of the state's name.
 */
void Pex::DebugInfo::FunctionInfo::setStateName(StringTable::Index value)
{
    m_StateName = value;
}

/**
 * @brief Retrieve the function name associated with this function debug info
 * @return the string index of the function's name
 */
Pex::StringTable::Index Pex::DebugInfo::FunctionInfo::getFunctionName() const
{
    return m_FunctionName;
}

/**
 * @brief Sets the name of the function associated with this function debug info
 *
 * @param[in] value String index of the function's name.
 */
void Pex::DebugInfo::FunctionInfo::setFunctionName(StringTable::Index value)
{
    m_FunctionName = value;
}

/**
 * @brief Retrieve the type of the function
 *
 * @return the type of the function
 */
Pex::DebugInfo::FunctionType Pex::DebugInfo::FunctionInfo::getFunctionType() const
{
    return m_FunctionType;
}

/**
 * @brief Sets the type of the function
 *
 * @param[in] value Type of the function
 *
 * @todo extract an enum
 */
void Pex::DebugInfo::FunctionInfo::setFunctionType(FunctionType value)
{
    m_FunctionType  = value;
}

/**
 * @brief Retrieve the line number array
 * Each entry match a bytecode instruction in the function body.
 *
 * @return the const line number array;
 */
const Pex::DebugInfo::FunctionInfo::LineNumbers &Pex::DebugInfo::FunctionInfo::getLineNumbers() const
{
    return m_LineNumbers;
}
/**
 * @brief Retrieve the line number array
 * Each entry match a bytecode instruction in the function body.
 *
 * @return the modifiable line number array;
 */
Pex::DebugInfo::FunctionInfo::LineNumbers &Pex::DebugInfo::FunctionInfo::getLineNumbers()
{
    return m_LineNumbers;
}



