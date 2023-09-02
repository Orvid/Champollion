#pragma once

#include <cstdint>
#include <ctime>
#include <fstream>

#include "Binary.hpp"

namespace Pex {

/**
 * @brief Binary structure file reading.
 *
 * The FileReader class provides a function to read a PEX file into a Binary structure.
 * The filename is provided as a parameter of the constructor.
 */
class FileReader
{
public:
    FileReader(std::istream *stream);
    FileReader(const std::string& fileName);
    ~FileReader();

    void read(Binary& binary);
    enum Endianness{
        BIG_ENDIAN,
        LITTLE_ENDIAN
    };
    static constexpr std::uint32_t LE_MAGIC_NUMBER = 0xFA57C0DE;
    static constexpr std::uint32_t BE_MAGIC_NUMBER = 0xDEC057FA;

protected:

    void readHeader(Header& header);
    void read(StringTable& stringTable);
    void read(DebugInfo& debugInfo);
    void read(UserFlags& userFlags);
    void read(const Pex::Header& header, Objects& objects);
    void read(StructInfos& structInfos);
    void read(Variables& variables);
    void read(Properties& properties);
    void read(States& states);
    void read(Guards& guards);
    void read(Functions& functions);
    void read(Function& function);
    void read(TypedNames& typednames);
    void read(Instructions& instructions);


    std::uint8_t getUint8();
    std::uint16_t getUint16();
    std::uint32_t getUint32(bool le_override = false);
    StringTable::Index getStringIndex();

    std::int16_t getInt16();
    float getFloat();
    std::time_t getTime();
    std::string getString();
    Value getValue();

    const StringTable* m_StringTable;

private:
    Endianness m_endianness;
    std::istream* m_iStream;
    std::ifstream m_fileStream;
};
}
