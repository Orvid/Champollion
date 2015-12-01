#pragma once

#include <vector>

#include "StringTable.hpp"
#include "NamedItem.hpp"
#include "TypedItem.hpp"
#include "UserFlagged.hpp"
#include "Value.hpp"

namespace Pex {

/**
 * @brief Variable definition.
 *
 * The variable are defined at the object level.
 */
class Variable :
        public NamedItem,
        public TypedItem,
        public UserFlagged
{
public:
    Variable();
    virtual ~Variable();

    const Value&       getDefaultValue() const;
    void               setDefaultValue(const Value& value);

    std::uint8_t getConstFlag() const { return m_ConstFlag; }
    void setConstFlag(std::uint8_t value) { m_ConstFlag = value; }

protected:
    Value              m_DefaultValue;
    std::uint8_t m_ConstFlag;

};

class Variables : public std::vector<Variable>
{
public:
    const Variable* findByName(const StringTable::Index& name) const;
};

}
