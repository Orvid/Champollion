#include "UserFlag.hpp"

#include <cassert>

/**
 * @brief Default constructor
 *
 */
Pex::UserFlag::UserFlag() :
    m_FlagIndex(0)
{
}

/**
 * @brief Default destructor
 */
Pex::UserFlag::~UserFlag()
{
}

/**
 * @brief Get the bit number associated with the flag
 * @return the bit number
 */
std::uint8_t Pex::UserFlag::getFlagIndex() const
{
    return m_FlagIndex;
}

/**
 * @brief Set the bit number of the flag
 * @param value the bit number, between 0 and 31
 */
void Pex::UserFlag::setFlagIndex(std::uint8_t value)
{
    assert(0<= value && value < 32);
    m_FlagIndex = value;
    m_FlagMask  = 1 << value;
}

/**
 * @brief Get the flag mask of the flag
 * The flag mask can be used with | and & operations.
 * @return
 */
std::uint32_t Pex::UserFlag::getFlagMask() const
{
    return m_FlagMask;
}
