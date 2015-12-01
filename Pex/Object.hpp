#pragma once

#include <vector>
#include <cstdint>


#include "StringTable.hpp"
#include "NamedItem.hpp"
#include "DocumentedItem.hpp"
#include "StructInfo.hpp"
#include "Variable.hpp"
#include "Property.hpp"
#include "State.hpp"

namespace Pex {

/**
 * @brief Object definition
 *
 * This class contains the elements defined in a pex script.
 *
 *
 */
class Object :
        public NamedItem,
        public UserFlagged,
        public DocumentedItem
{
public:
    Object();
    virtual ~Object();

    StringTable::Index getParentClassName() const;
    void setParentClassName(StringTable::Index value);


    StringTable::Index getAutoStateName() const;
    void setAutoStateName(StringTable::Index value);

    std::uint8_t getConstFlag() const { return m_ConstFlag; }
    void setConstFlag(std::uint8_t value) { m_ConstFlag = value; }

    const StructInfos& getStructInfos() const { return m_StructInfos; }
    StructInfos& getStructInfos() { return m_StructInfos; }

    const Variables& getVariables() const;
    Variables& getVariables();

    const Properties& getProperties() const;
    Properties& getProperties();

    const States& getStates() const;
    States& getStates();

private:
    StringTable::Index m_ParentClassName;
    StringTable::Index m_AutoStateName;
    std::uint8_t m_ConstFlag;

    StructInfos m_StructInfos;
    Variables m_Variables;
    Properties m_Properties;
    States m_States;

};

typedef std::vector<Object> Objects;
}
