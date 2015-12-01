#pragma once

#include <cstdint>

#include "StringTable.hpp"

namespace Pex {

/**
 * @brief Types managed in the Value class.
 */
enum class ValueType : std::uint8_t
{
    None = 0,
    Identifier = 1,
    String = 2,
    Integer = 3,
    Float = 4,
    Bool = 5
};

/**
 * @brief Variant class managing value.
 */
class Value
{
public:
    Value();
    explicit Value(const StringTable::Index& value, bool id = false);
    explicit Value(std::int32_t value);
    explicit Value(float value);
    explicit Value(bool value);
    ~Value();

    ValueType          getType() const;

    bool               isNone() const;
    void               clear();

    StringTable::Index getId() const;
    void               setId(const StringTable::Index& value);

    StringTable::Index getString() const;
    void               set(const StringTable::Index& value);

    std::int32_t       getInteger() const;
    void               set(std::int32_t value);

    float              getFloat() const;
    void               set(float value);

    bool               getBool() const;
    void               set(bool value);

    bool               operator==(const Value& rhs) const;

    std::string        toString() const;

protected:
    ValueType m_Type;
    union {
        struct {
            const StringTable* table;
            std::uint16_t      index;
        }                  string;
        std::int32_t       integer;
        float              real;
        bool               boolean;
    } m_Value;

    void ensureType(ValueType type) const;
};

}
