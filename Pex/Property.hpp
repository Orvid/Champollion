#pragma once

#include <cstdint>
#include <vector>

#include "StringTable.hpp"
#include "NamedItem.hpp"
#include "TypedItem.hpp"
#include "DocumentedItem.hpp"
#include "UserFlagged.hpp"
#include "Function.hpp"

namespace Pex {

enum class PropertyFlag : std::uint8_t {
    NO_FLAGS = 0,
    READ     = 1 << 0,
    WRITE    = 1 << 1,
    AUTOVAR  = 1 << 2
};

inline PropertyFlag operator|(PropertyFlag a, PropertyFlag b)
{
  typedef std::underlying_type<PropertyFlag>::type enum_type;
  return static_cast<PropertyFlag>(static_cast<enum_type>(a) | static_cast<enum_type>(b));
}
inline PropertyFlag operator&(PropertyFlag a, PropertyFlag b)
{
  typedef std::underlying_type<PropertyFlag>::type enum_type;
  return static_cast<PropertyFlag>(static_cast<enum_type>(a) & static_cast<enum_type>(b));
}

/**
 * @brief Property definition
 *
 * This class contains the type, names and associated flags of a property.
 *
 * The getter and stter function are also stored here if they exists.
 *
 */
class Property :
        public NamedItem,
        public TypedItem,
        public UserFlagged,
        public DocumentedItem
{
public:
    Property();
    ~Property();

    StringTable::Index getAutoVarName() const;
    void setAutoVarName(StringTable::Index value);

    void setFlags(PropertyFlag value);
    bool isReadable() const;
    bool isWritable() const;
    bool hasAutoVar() const;

    const Function& getReadFunction() const;
    Function& getReadFunction();

    const Function& getWriteFunction() const;
    Function& getWriteFunction();

protected:
    StringTable::Index m_DocString;
    PropertyFlag m_Flags;
    StringTable::Index m_AutoVarName;

    Function m_ReadFunction;
    Function m_WriteFunction;

};

typedef std::vector<Property> Properties;
}
