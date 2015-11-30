#ifndef USERFLAGGED_HPP
#define USERFLAGGED_HPP

#include "Pex_global.hpp"

#include <cstdint>

namespace Pex {
/**
 * @brief Base mixin for elements with User Flag.
 *
 */
class PEX_API UserFlagged
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

#endif // USERFLAGGED_HPP
