#ifndef STRUCTINFO_HPP
#define STRUCTINFO_HPP

#include "Pex_global.hpp"

#include <vector>

#include "StringTable.hpp"
#include "DocumentedItem.hpp"
#include "NamedItem.hpp"
#include "TypedItem.hpp"
#include "UserFlagged.hpp"
#include "Value.hpp"

namespace Pex {

class PEX_API StructInfo :
        public NamedItem
{
public:
    class PEX_API Member :
            public DocumentedItem,
            public NamedItem,
            public TypedItem,
            public UserFlagged
    {
    public:
        Member() = default;
        ~Member() = default;

        std::uint8_t getConstFlag() const { return m_ConstFlag; }
        void setConstFlag(std::uint8_t value) { m_ConstFlag = value; }

        const Value& getValue() const { return m_Value; }
        void setValue(const Value& value) { m_Value = value; }

    private:
        std::uint8_t m_ConstFlag;
        Value m_Value;
    };

    typedef std::vector<Member> Members;
public:
    StructInfo() = default;
    ~StructInfo() = default;

    const Members& getMembers() const { return m_Members; }
    Members& getMembers() { return m_Members; }

private:
    Members m_Members;

};

typedef std::vector<StructInfo> StructInfos;

}

#endif // STRUCTINFO_HPP

