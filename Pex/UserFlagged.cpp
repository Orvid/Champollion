#include "UserFlagged.hpp"

/**
 * @brief Default constructor
 */
Pex::UserFlagged::UserFlagged() :
    m_UserFlags(0)
{
}

/**
 * @brief Default destructor.
 */
Pex::UserFlagged::~UserFlagged()
{
}

/**
 * @brief Get the user flags byte.
 * The flags can be decoded using the data defined in UserFlag classes.
 * @return
 */
std::uint32_t Pex::UserFlagged::getUserFlags() const
{
    return m_UserFlags;
}

/**
 * @brief Set the user flags byte.
 * @param value
 */
void Pex::UserFlagged::setUserFlags(std::uint32_t value)
{
    m_UserFlags = value;
}
