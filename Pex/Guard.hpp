#pragma once

#include <cstdint>
#include <vector>

#include "StringTable.hpp"
#include "NamedItem.hpp"

namespace Pex {

/**
 * @brief Guard definition
 *
 * This class contains the names guard.
 *
 */
class Guard :
    public NamedItem {
public:
    Guard() = default;
    ~Guard() = default;
};

typedef std::vector<Guard> Guards;
}
