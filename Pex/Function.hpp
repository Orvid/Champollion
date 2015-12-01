#pragma once

#include <vector>

#include "StringTable.hpp"
#include "NamedItem.hpp"
#include "UserFlagged.hpp"
#include "DocumentedItem.hpp"

#include "TypedName.hpp"
#include "Instruction.hpp"

namespace Pex {
/**
 * @brief Function definition
 *
 * The Function class contains all elements needed to define a function in a given object.
 * It contains the function signature and the associated body.
 *
 */
class Function :
        public NamedItem,
        public UserFlagged,
        public DocumentedItem
{
public:
    Function();
    virtual ~Function();

    StringTable::Index getReturnTypeName() const;
    void setReturnTypeName(StringTable::Index value);

    void setFlags(std::uint8_t value);
    bool isGlobal() const;
    bool isNative() const;

    const TypedNames& getParams() const;
    TypedNames& getParams();

    const TypedNames& getLocals() const;
    TypedNames& getLocals();

    const Instructions& getInstructions() const;
    Instructions& getInstructions();
protected:
    StringTable::Index m_ReturnTypeName;
    std::uint8_t m_Flags;
    TypedNames m_Params;
    TypedNames m_Locals;
    Instructions m_Instructions;

};

typedef std::vector<Function> Functions;
}
