#include "FileReader.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>

#include <cassert>

static uint16_t really_swap16(uint16_t v)
{
    auto bytes = (uint8_t*)&v;
    std::swap(bytes[0], bytes[1]);
    return v;
}

static uint32_t really_swap32(uint32_t v)
{
    auto bytes = (uint8_t*)&v;
    std::swap(bytes[0], bytes[3]);
    std::swap(bytes[1], bytes[2]);
    return v;
}

static uint64_t really_swap64(uint64_t v)
{
    auto bytes = (uint8_t*)&v;
    std::swap(bytes[0], bytes[7]);
    std::swap(bytes[1], bytes[6]);
    std::swap(bytes[2], bytes[5]);
    std::swap(bytes[3], bytes[4]);
    return v;
}

static uint16_t swap16(uint16_t v)
{
    return v;
    //return really_swap16(v);
}

static uint32_t swap32(uint32_t v)
{
    return v;
    //return really_swap32(v);
}

static uint64_t swap64(uint64_t v)
{
    return v;
    //return really_swap64(v);
}

/**
 * @brief Construct from file name
 * @param[in] fileName name of the pex file.
 *
 * @throws runtime_error if the file can't be openened
 */
Pex::FileReader::FileReader(const std::string &fileName) :
    m_File(fileName, std::ifstream::binary),
    m_StringTable(nullptr)
{    
    if (m_File.bad())
    {
        throw std::runtime_error("Unable to open file");
    }
}

/**
 * @brief Default destructor
 */
Pex::FileReader::~FileReader()
{
}

/**
 * @brief Fills in the binary structure with the data read from the associated file input.
 * @param[out] binary Structure to be filed in
 *
 * @throws runtime_error if the structure of the file is incorrect.
 */
void Pex::FileReader::read(Pex::Binary &binary)
{
    readHeader(binary.getHeader());
    read(binary.getStringTable());
    m_StringTable = & binary.getStringTable();
    read(binary.getDebugInfo());
    read(binary.getUserFlags());
    read(binary.getObjects());    
}

/**
 * @brief Reads the Header from the file
 * @param[in] header Header to fill in
 *
 */
void Pex::FileReader::readHeader(Pex::Header &header)
{
    const std::uint32_t MAGIC_NUMBER = 0xFA57C0DE;
    auto magic = (getUint32());

    if (magic != MAGIC_NUMBER)
    {
        throw std::runtime_error("Invalid file magic");
    }

    header.setMajorVersion(getUint8());
    header.setMinorVersion(getUint8());
    header.setGameID(really_swap16(getUint16()));
    header.setCompilationTime(getTime());
    header.setSourceFileName(getSString());
    header.setUserName(getSString());
    header.setComputerName(getSString());

}

/**
 * @brief Reads the string table from the file.
 * @param[in] stringTable table to fill in
 */
void Pex::FileReader::read(Pex::StringTable &stringTable)
{
    auto len = getSUint16();
    stringTable.reserve(len);

    for(auto i = 0; i < len; ++i)
    {
        stringTable.push_back(getSString());
    }

}
/**
 * @brief Reads the debug info package
 * @param[in] debugInfo DebugInfo object to fill in.
 */
void Pex::FileReader::read(Pex::DebugInfo &debugInfo)
{
    auto hasDebugInfo = getUint8();
    if(hasDebugInfo)
    {
        debugInfo.setModificationTime(getSTime());

        auto functionCount = getSUint16();
        auto& functionInfos = debugInfo.getFunctionInfos();
        functionInfos.resize(functionCount);
        for (auto& functionInfo : functionInfos)
        {
            functionInfo.setObjectName(getSStringIndex());
            functionInfo.setStateName(getSStringIndex());
            functionInfo.setFunctionName(getSStringIndex());
            functionInfo.setFunctionType(static_cast<DebugInfo::FunctionType>(getUint8()));
            auto instructionCount = getSUint16();
            auto& lineNumbers = functionInfo.getLineNumbers();
            lineNumbers.reserve(instructionCount);
            for (auto l  = 0; l < instructionCount; ++l)
            {
                lineNumbers.push_back(getSUint16());
            }
        }

        auto groupCount = getSUint16();
        auto& propertyGroups = debugInfo.getPropertyGroups();
        propertyGroups.resize(groupCount);
        for (auto& propertyGroup : propertyGroups)
        {
            propertyGroup.setObjectName(getSStringIndex());
            propertyGroup.setGroupName(getSStringIndex());
            propertyGroup.setGroupDocumentation(getSStringIndex());
            propertyGroup.setUserFlags(getSUint32());
            auto nameCount = getSUint16();
            auto& names = propertyGroup.getNames();
            names.reserve(nameCount);
            for (auto l  = 0; l < nameCount; ++l)
            {
                names.push_back(getSStringIndex());
            }
        }

        auto orderCount = getSUint16();
        auto& structOrders = debugInfo.getStructOrders();
        structOrders.resize(orderCount);
        for (auto& structOrder : structOrders)
        {
            structOrder.setObjectName(getSStringIndex());
            structOrder.setOrderName(getSStringIndex());
            auto nameCount = getSUint16();
            auto& names = structOrder.getNames();
            names.reserve(nameCount);
            for (auto l  = 0; l < nameCount; ++l)
            {
                names.push_back(getSStringIndex());
            }
        }
    }
}

/**
 * @brief Reads the User Flags definition from the file.
 * @param[in] userFlags UserFlag collection to fill in.
 */
void Pex::FileReader::read(Pex::UserFlags &userFlags)
{
    auto count = getSUint16();
    userFlags.resize(count);
    for (auto& userFlag : userFlags)
    {
        userFlag.setName(getSStringIndex());
        userFlag.setFlagIndex(getUint8());
    }
}

/**
 * @brief Reads the Objects definitions from the file.
 * @param[in] objects Object collection to fill in.
 */
void Pex::FileReader::read(Pex::Objects &objects)
{
    auto count = getSUint16();
    objects.resize(count);

    for(auto& object : objects)
    {
        object.setName(getSStringIndex());
        (void)getSUint32();

        object.setParentClassName(getSStringIndex());
        object.setDocString(getSStringIndex());
        object.setConstFlag(getUint8());
        object.setUserFlags(getSUint32());
        object.setAutoStateName(getSStringIndex());

        read(object.getStructInfos());
        read(object.getVariables());
        read(object.getProperties());
        read(object.getStates());
    }
}

/**
 * @brief Reads the StructInfos definition for an object
 * @param[in] struct info collection to fill in.
 */
void Pex::FileReader::read(Pex::StructInfos &structInfos)
{
    auto infoCount = getSUint16();
    structInfos.resize(infoCount);
    for(auto& info : structInfos)
    {
        info.setName(getSStringIndex());

        auto memberCount = getSUint16();
        auto& members = info.getMembers();
        members.resize(memberCount);
        for (auto& member : members)
        {
            member.setName(getSStringIndex());
            member.setTypeName(getSStringIndex());
            member.setUserFlags(getSUint32());
            member.setValue(getValue());
            member.setConstFlag(getUint8());
            member.setDocString(getSStringIndex());
        }
    }
}

/**
 * @brief Reads the Variables definition for an object
 * @param[in] variables collection to fill in.
 */
void Pex::FileReader::read(Pex::Variables &variables)
{
    auto variableCount = getSUint16();
    variables.resize(variableCount);
    for(auto& variable : variables)
    {
        variable.setName(getSStringIndex());
        variable.setTypeName(getSStringIndex());
        variable.setUserFlags(getSUint32());

        variable.setDefaultValue(getValue());
        variable.setConstFlag(getUint8());
    }
}

/**
 * @brief Reads the Properties definition for an object
 * @param[in] properties collection to fill in.
 */
void Pex::FileReader::read(Pex::Properties &properties)
{
    auto propertyCount = getSUint16();
    properties.resize(propertyCount);
    for(auto& property : properties)
    {
        property.setName(getSStringIndex());
        property.setTypeName(getSStringIndex());
        property.setDocString(getSStringIndex());
        property.setUserFlags(getSUint32());
        property.setFlags(static_cast<PropertyFlag>(getUint8()));
        if(property.hasAutoVar())
        {
            property.setAutoVarName(getSStringIndex());
        }
        else
        {
            if (property.isReadable())
            {
                read(property.getReadFunction());
            }
            if(property.isWritable())
            {
                read(property.getWriteFunction());
            }
        }
    }
}

/**
 * @brief Reads the States definition for an object
 * @param[in] states collection to fill in.
 */
void Pex::FileReader::read(Pex::States &states)
{
    auto stateCount = getSUint16();
    states.resize(stateCount);
    for(auto& state : states)
    {
        state.setName(getSStringIndex());
        read(state.getFunctions());
    }
}

/**
 * @brief Reads the Functions definition for a state
 * @param[in] functions collection to fill in.
 */
void Pex::FileReader::read(Pex::Functions &functions)
{
    auto functionCount = getSUint16();
    functions.resize(functionCount);
    for(auto& function : functions)
    {
        function.setName(getSStringIndex());
        read(function);
    }
}

/**
 * @brief Reads a body function
 * @param[in] function Function structure to fill in.
 */
void Pex::FileReader::read(Pex::Function &function)
{
    function.setReturnTypeName(getSStringIndex());
    function.setDocString(getSStringIndex());
    function.setUserFlags(getSUint32());
    function.setFlags(getUint8());
    read(function.getParams());
    read(function.getLocals());
    read(function.getInstructions());
}

/**
 * @brief Reads the local variable definition for a function body
 * @param[in] typednames collection to fill in.
 */
void Pex::FileReader::read(Pex::TypedNames &typednames)
{
    auto nameCount = getSUint16();
    typednames.resize(nameCount);
    for(auto& typedname : typednames)
    {
        typedname.setName(getSStringIndex());
        typedname.setTypeName(getSStringIndex());
    }
}

/**
 * @brief Reads the instruction list for a function body
 * @param[in] instructions collection to fill in.
 */
void Pex::FileReader::read(Pex::Instructions &instructions)
{
    auto instructionCount = getSUint16();
    instructions.resize(instructionCount);
    for(auto& instruction : instructions)
    {
        auto opcode = getUint8();
        if (opcode >= static_cast<std::uint8_t>(OpCode::MAX_OPCODE))
        {
            std::stringstream error;
            error << "Invalid opcode 0x" << std::hex << std::setfill('0') << std::setw(2) << (int)opcode;
            throw std::runtime_error(error.str());
        }
        instruction.setOpCode(static_cast<OpCode>(opcode));
        auto& args = instruction.getArgs();
        args.resize(instruction.getOpCodeArgCount());
        for(auto& arg : args)
        {
            arg = getValue();
        }
        if (instruction.hasVarArgs())
        {
            auto argcount = getValue();
            if (argcount.getType() != ValueType::Integer)
            {
                throw std::runtime_error("Invalid value for varargs");
            }
            auto& varargs = instruction.getVarArgs();
            varargs.resize(argcount.getInteger());
            for(auto& arg : varargs)
            {
                arg = getValue();
            }
        }
    }
}

/**
 * @brief Reads a byte from the file.
 * @return a byte read from the stream
 */
std::uint8_t Pex::FileReader::getUint8()
{
    std::uint8_t value;
    m_File.read(reinterpret_cast<char*>(&value), sizeof(value));
    if(!m_File)
    {
        throw std::runtime_error("Error reading file");
    }
    return value;
}

/**
 * @brief Reads a 16 bit unsigned int from the file.
 * The int is store in the file coded in little endian.
 * @return a short read from the file.
 */
std::uint16_t Pex::FileReader::getUint16()
{
    std::uint16_t value;
    m_File.read(reinterpret_cast<char*>(&value), sizeof(value));
    if(!m_File)
    {
        throw std::runtime_error("Error reading file");
    }
    return value;
}

/**
 * @brief Reads a 16 bit unsigned int from the file.
 * The int is store in the file coded in big endian.
 * @return a short read from the file.
 */
std::uint16_t Pex::FileReader::getSUint16()
{
    return swap16(getUint16());
}

/**
 * @brief Reads a 32 bit unsigned int from the file.
 * The int is store in the file coded in little endian.
 * @return a long read from the file.
 */
std::uint32_t Pex::FileReader::getUint32()
{
    std::uint32_t value = 0;
    m_File.read(reinterpret_cast<char*>(&value), sizeof(value));
    if(m_File.gcount() != sizeof(value))
    {
        throw std::runtime_error("Error reading file");
    }
    return value;
}

/**
 * @brief Reads a 32 bit unsigned int from the file.
 * The int is store in the file coded in big endian.
 * @return a long read from the file.
 */
std::uint32_t Pex::FileReader::getSUint32()
{
    return swap32(getUint32());
}

/**
 * @brief Read a string index from the file.
 * The string table used is the one already read from the file.
 * @return a String Index.
 */
Pex::StringTable::Index Pex::FileReader::getStringIndex()
{
    assert(m_StringTable != nullptr);
    auto index = getUint16();
    if (index >= m_StringTable->size())
    {
        throw std::runtime_error("Invalid string index");
    }

    return m_StringTable->get(index);
}

/**
 * @brief Read a string index from the file.
 * The string table used is the one already read from the file.
 * @return a String Index.
 */
Pex::StringTable::Index Pex::FileReader::getSStringIndex()
{
    assert(m_StringTable != nullptr);
    auto index = getSUint16();
    if (index >= m_StringTable->size())
    {
        throw std::runtime_error("Invalid string index");
    }

    return m_StringTable->get(index);
}

/**
 * @brief Reads a 16 bit signed int from the file.
 * The int is store in the file coded in little endian.
 * @return a short read from the file.
 */
std::int16_t Pex::FileReader::getInt16()
{
    std::int16_t value;
    m_File.read(reinterpret_cast<char*>(&value), sizeof(value));
    if(!m_File)
    {
        throw std::runtime_error("Error reading file");
    }
    return value;
}

/**
 * @brief Reads a 16 bit signed int from the file.
 * The int is store in the file coded in big endian.
 * @return a short read from the file.
 */
std::int16_t Pex::FileReader::getSInt16()
{
    return (int16_t)swap16((uint16_t)getInt16());
}

/**
 * @brief Reads a 32 bit float from the file.
 * The int is store in the file coded in little endian.
 * @return a float read from the file.
 */
float Pex::FileReader::getFloat()
{
    float value;
    m_File.read(reinterpret_cast<char*>(&value), sizeof(value));
    if(!m_File)
    {
        throw std::runtime_error("Error reading file");
    }
    return value;
}

/**
 * @brief Reads a 32 bit float from the file.
 * The int is store in the file coded in big endian.
 * @return a float read from the file.
 */
float Pex::FileReader::getSFloat()
{
    float value = getFloat();
    auto v2 = swap32(*(uint32_t*)&value);
    return *(float*)&v2;
}

/**
 * @brief Reads a 64 bit time_t from the file.
 * The int is store in the file coded in little endian.
 * @return a time_t read from the file.
 */
std::time_t Pex::FileReader::getTime()
{
    static_assert(sizeof(std::time_t) == 8, "time_t is not 64 bits");
    std::time_t value;
    m_File.read(reinterpret_cast<char*>(&value), sizeof(value));
    if(!m_File)
    {
        throw std::runtime_error("Error reading file");
    }
    return value;
}

/**
 * @brief Reads a 64 bit time_t from the file.
 * The int is store in the file coded in big endian.
 * @return a time_t read from the file.
 */
std::time_t Pex::FileReader::getSTime()
{
    return (time_t)swap64((uint64_t)getTime());
}

/**
 * @brief Reads a variable sized string from the file.
 * @return a string.
 */
std::string Pex::FileReader::getString()
{
    auto len = getUint16();
    auto data = new char[len];
    memset(data, 0, len);
    m_File.read(data, len);
    if (m_File.gcount() != len)
    {
        throw std::runtime_error("Unable to read string");
    }
    std::string value(data, data + len);
    delete[] data;
    return value;
}

/**
 * @brief Reads a variable sized string from the file.
 * @return a string.
 */
std::string Pex::FileReader::getSString()
{
    auto len = getSUint16();
    auto data = new char[len];
    memset(data, 0, len);
    m_File.read(data, len);
    if (m_File.gcount() != len)
    {
        throw std::runtime_error("Unable to read Sstring");
    }
    std::string value(data, data + len);
    delete[] data;
    return value;
}

/**
 * @brief Reads a variant typed value from the file.
 * @return a Pew::Value.
 */
Pex::Value Pex::FileReader::getValue()
{
    auto valueType = getUint8();
    switch(valueType)
    {
    case 0:
        return Value();
        break;
    case 1:
    {
        auto value = getSStringIndex();
        return Value(value, true);
    }
        break;
    case 2:
    {
        auto value = getSStringIndex();
        return Value(value);
    }
        break;
    case 3:
    {
        auto value = static_cast<std::int32_t>(getSUint32());
        return Value(value);
    }
        break;
    case 4:
    {
        auto value = getSFloat();
        return Value(value);
    }
        break;
    case 5:
    {
        auto value = (getUint8() != 0);
        return Value(value);
    }
        break;
    default:
        std::stringstream error;
        error << "Invalid value type " << valueType;
        throw std::runtime_error(error.str());

    }
}
