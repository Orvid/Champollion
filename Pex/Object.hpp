#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "Pex_global.hpp"

#include <vector>
#include <cstdint>


#include "StringTable.hpp"
#include "NamedItem.hpp"
#include "DocumentedItem.hpp"
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
class PEX_API Object :
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

    const Variables& getVariables() const;
    Variables& getVariables();

    const Properties& getProperties() const;
    Properties& getProperties();

    const States& getStates() const;
    States& getStates();

private:
    StringTable::Index m_ParentClassName;
    StringTable::Index m_AutoStateName;

    Variables m_Variables;
    Properties m_Properties;
    States m_States;

};

typedef std::vector<Object> Objects;
}

#endif // OBJECT_HPP
