#ifndef STATE_HPP
#define STATE_HPP

#include "Pex_global.hpp"

#include <vector>

#include "StringTable.hpp"
#include "NamedItem.hpp"
#include "Function.hpp"

namespace Pex {
/**
 * @brief State definition
 *
 * This class contains the elements for a state.
 */
class PEX_API State :
        public NamedItem
{
public:
    State();
    virtual ~State();

    const Functions& getFunctions() const;
    Functions& getFunctions();

protected:
    Functions m_Functions;
};

typedef std::vector<State> States;

}
#endif // STATE_HPP
