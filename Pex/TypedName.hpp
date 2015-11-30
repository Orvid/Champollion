#ifndef TYPEDNAME_HPP
#define TYPEDNAME_HPP

#include "Pex_global.hpp"

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
class PEX_API TypedName :
        public NamedItem,
        public TypedItem
{
public:
    TypedName();
    virtual ~TypedName();
};

typedef std::vector<TypedName> TypedNames;
}

#endif // TYPEDNAME_HPP
