#pragma once

#include <ctime>
#include <cstdint>
#include <vector>

#include "DocumentedItem.hpp"
#include "StringTable.hpp"
#include "UserFlagged.hpp"

namespace Pex {

/**
 * @brief Debug Information
 *
 * The DebugInfo class describe the debug information associated with objects and function.
 * For the file, the only data is the source file modification time.
 * For each function available in the object, the original source line number are also available.
 *
 */
class DebugInfo
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
    class FunctionInfo
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

        LineNumbers m_LineNumbers;
    };

    class PropertyGroup :
            public DocumentedItem,
            public UserFlagged
    {
    public:
        typedef std::vector<StringTable::Index> Names;

    public:
        PropertyGroup() = default;
        ~PropertyGroup() = default;

        StringTable::Index getObjectName() const { return m_ObjectName; }
        void setObjectName(StringTable::Index value) { m_ObjectName = value; }

        StringTable::Index getGroupName() const { return m_GroupName; }
        void setGroupName(StringTable::Index value) { m_GroupName = value; }

        const Names& getNames() const { return m_Names; }
        Names& getNames() { return m_Names; }

     private:
        StringTable::Index m_ObjectName;
        StringTable::Index m_GroupName;

        Names m_Names;
    };

    class StructOrder
    {
    public:
        typedef std::vector<StringTable::Index> Names;

    public:
        StructOrder() = default;
        ~StructOrder() = default;

        StringTable::Index getObjectName() const { return m_ObjectName; }
        void setObjectName(StringTable::Index value) { m_ObjectName = value; }

        StringTable::Index getOrderName() const { return m_OrderName; }
        void setOrderName(StringTable::Index value) { m_OrderName = value; }

        const Names& getNames() const { return m_Names; }
        Names& getNames() { return m_Names; }

     private:
        StringTable::Index m_ObjectName;
        StringTable::Index m_OrderName;

        Names m_Names;
    };

    typedef std::vector<FunctionInfo> FunctionInfos;
    typedef std::vector<PropertyGroup> PropertyGroups;
    typedef std::vector<StructOrder> StructOrders;
public:
    DebugInfo();
    ~DebugInfo();

    const std::time_t& getModificationTime() const;
    void setModificationTime(const std::time_t& value);

    const FunctionInfos& getFunctionInfos() const;
    FunctionInfos& getFunctionInfos();

    const PropertyGroups& getPropertyGroups() const { return m_PropertyGroup; }
    PropertyGroups& getPropertyGroups() { return m_PropertyGroup; }

    const StructOrders& getStructOrders() const { return m_StructOrder; }
    StructOrders& getStructOrders() { return m_StructOrder; }

    const FunctionInfo *getFunctionInfo(const StringTable::Index& object, const StringTable::Index& state, const StringTable::Index& name, FunctionType type = FunctionType::Method) const;
private:
    std::time_t m_ModificationTime;
    FunctionInfos m_FunctionInfo;
    PropertyGroups m_PropertyGroup;
    StructOrders m_StructOrder;
};
}
