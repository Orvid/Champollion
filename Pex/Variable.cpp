#include "Variable.hpp"

#include <algorithm>
#include <assert.h>


/**
 * @brief Default constructor
 */
Pex::Variable::Variable()
{
}

/**
 * @brief Default destructor
 */
Pex::Variable::~Variable()
{
}

/**
 * @brief Get the default value of the variable.
 * @return The default value.
 */
const Pex::Value& Pex::Variable::getDefaultValue() const
{
    return m_DefaultValue;
}

/**
 * @brief Set the default a value of the variable.
 * @param value The new default value.
 */
void Pex::Variable::setDefaultValue(const Value &value)
{
    m_DefaultValue = value;
}


const Pex::Variable *Pex::Variables::findByName(const Pex::StringTable::Index &name) const
{
    auto nameIndex = name.asIndex();
    auto it = std::find_if(begin(), end(), [&] (const value_type& item) {return item.getName().asIndex() == nameIndex;});
    if (it == end())
    {
        return nullptr;
    }
    return & (*it);
}
