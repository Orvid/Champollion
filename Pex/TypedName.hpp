#pragma once

#include <vector>

#include "NamedItem.hpp"
#include "TypedItem.hpp"

namespace Pex {
/**
 * @brief Typed name information
 *
 * This class associated a name and a type. Its can be used to declare variable or parameters.
 *
 */
class TypedName :
        public NamedItem,
        public TypedItem
{
public:
    TypedName();
    virtual ~TypedName();
};

typedef std::vector<TypedName> TypedNames;
}
