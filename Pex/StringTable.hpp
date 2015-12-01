#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <cstdint>

namespace Pex {

/**
 * @brief String table
 *
 * The String Table can be used as a vector for the insertion.
 * It is accessed throught the StringTable::Index classes
 * which references the table and the numeric index.
 *
 */
class StringTable
{
protected:
    typedef std::vector<std::string> Table;

public:
    StringTable();
    ~StringTable();

    /**
     * @brief Index to a string in a StringTable
     *
     * The Index class provides access to a string in the string table.
     * It contains both a pointer to the table and the value of the numeric index.
     *
     */
    class Index
    {
    public:
        static const std::uint16_t UNDEFINED  = 0xFFFF;
    public:
        Index();
        ~Index();

        const std::string& asString() const;
        std::string asStringWithIndex() const;
        std::uint16_t asIndex() const;
        const StringTable* getTable() const;

        bool isValid() const;
        bool isUndefined() const;

        bool operator == (const Index& rhs) const;
        bool operator != (const Index& rhs) const;

    protected:
        Index(const StringTable* table, std::uint16_t asIndex);
        std::uint16_t m_Index;
        const StringTable*  m_Table;

        friend class StringTable;
    };

    const std::string& operator[] (std::uint16_t index) const;
    std::string& operator[] (std::uint16_t index);

    Index get(std::uint16_t index) const;
    Index findIdentifier(const std::string& id) const;

    Table::iterator begin();
    Table::iterator end();
    Table::const_iterator begin() const;
    Table::const_iterator end() const;

    void push_back(const std::string& value);
    size_t size() const;
    void reserve(size_t size);
protected:
    Table m_Table;

};
}

inline std::ostream& operator << (std::ostream& stream, const Pex::StringTable::Index& index)
{
    if(index.isValid())
    {
        stream << index.asString();
    }
    else
    {
        stream << "*invalid*";
    }
    return stream;
}
