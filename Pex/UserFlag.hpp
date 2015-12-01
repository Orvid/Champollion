#pragma once

#include <vector>
#include <cstdint>

#include "StringTable.hpp"
#include "NamedItem.hpp"

namespace Pex {
/**
 * @brief User flag definition
 *
 * The User flag definition associate a name to a flag mask.
 * It can be used to decode user flags sets on items.
 */
class UserFlag :
        public NamedItem
{
public:
    UserFlag();
    ~UserFlag();

    std::uint8_t getFlagIndex() const;
    void setFlagIndex(std::uint8_t value);

    std::uint32_t getFlagMask() const;

private:
    std::uint8_t m_FlagIndex;
    std::uint32_t m_FlagMask;
};

typedef std::vector<UserFlag> UserFlags;

}
