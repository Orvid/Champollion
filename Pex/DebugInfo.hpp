#ifndef DEBUGINFO_HPP
#define DEBUGINFO_HPP

#include "Pex_global.hpp"

#include <ctime>
#include <cstdint>
#include <vector>

#include "StringTable.hpp"

namespace Pex {

/**
 * @brief Debug Information
 *
 * The DebugInfo class describe the debug information associated with objects and function.
 * For the file, the only data is the source file modification time.
 * For each function available in the object, the original source line number are also available.
 *
 */
class PEX_API DebugInfo
{
public:

    /**
     * @brief Function type
     *
     */
    enum class FunctionType : std::uint8_t
    {
        /// Standard method
        Method = 0,
        /// Property getter function
        Getter = 1,
        /// Property setter function
        Setter = 2
    };

    /**
     * @brief Debug information subset
     *
     * The FunctionInfo describer the debug information associated with a specific
     * function, referenced by objet, state and name.
     *
     */
    class PEX_API FunctionInfo
    {
    public:
        typedef std::vector<std::uint16_t> LineNumbers;

    public:
        FunctionInfo();
        ~FunctionInfo();

        StringTable::Index getObjectName() const;
        void setObjectName(StringTable::Index value);

        StringTable::Index getStateName() const;
        void setStateName(StringTable::Index value);

        StringTable::Index getFunctionName() const;
        void setFunctionName(StringTable::Index value);

        FunctionType getFunctionType() const;
        void setFunctionType(FunctionType);

        const LineNumbers& getLineNumbers() const;
        LineNumbers& getLineNumbers();

     private:
        StringTable::Index m_ObjectName;
        StringTable::Index m_StateName;
        StringTable::Index m_FunctionName;
        FunctionType m_FunctionType;

        LineNumbers  m_LineNumbers;
    };

    typedef std::vector<FunctionInfo> FunctionInfos;
public:
    DebugInfo();
    ~DebugInfo();

    const std::time_t& getModificationTime() const;
    void setModificationTime(const std::time_t& value);

    const FunctionInfos& getFunctionInfos() const;
    FunctionInfos& getFunctionInfos();

    const FunctionInfo *getFunctionInfo(const StringTable::Index& object, const StringTable::Index& state, const StringTable::Index& name, FunctionType type = FunctionType::Method) const;
private:
    std::time_t m_ModificationTime;
    FunctionInfos m_FunctionInfo;

};
}
#endif // DEBUGINFO_HPP
