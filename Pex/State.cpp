#include "State.hpp"

/**
 * @brief Default constructor
 *
 */
Pex::State::State()
{
}

/**
 * @brief Default destructor state
 */
Pex::State::~State()
{
}

/**
 * @brief Get the function list
 * @return the const function list
 */
const Pex::Functions &Pex::State::getFunctions() const
{
    return m_Functions;
}

/**
 * @brief Get the function list
 * @return the modifiable function list
 */
Pex::Functions &Pex::State::getFunctions()
{
    return m_Functions;
}
