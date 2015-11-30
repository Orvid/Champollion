#ifndef VARIABLE_HPP
#define VARIABLE_HPP

#include "Pex_global.hpp"

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
class PEX_API Variable :
        public NamedItem,
        public TypedItem,
        public UserFlagged
{
public:
    Variable();
    virtual ~Variable();

    const Value&       getDefaultValue() const;
    void               setDefaultValue(const Value& value);

protected:
    Value              m_DefaultValue;

};

class PEX_API Variables : public std::vector<Variable>
{
public:
    const Variable* findByName(const StringTable::Index& name) const;
};

}
#endif // VARIABLE_HPP
