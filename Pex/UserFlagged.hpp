#pragma once

#include <cstdint>

namespace Pex {
/**
 * @brief Base mixin for elements with User Flag.
 *
 */
class UserFlagged
{
public:
    UserFlagged();
    virtual ~UserFlagged();

    std::uint32_t      getUserFlags() const;
    void               setUserFlags(std::uint32_t value);

protected:
    std::uint32_t m_UserFlags;
};
}
