#include "AsmCoder.hpp"

#include <cassert>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <map>

/**
 * @brief Constructor
 * Builds an object associated with an output writer.
 *
 * @param writer Pointer to the output writer. The ownership is transferred.
 */
Decompiler::AsmCoder::AsmCoder(Decompiler::OutputWriter *writer) :
    Coder(writer)
{
}

/**
 * @brief Default destructor.
 */
Decompiler::AsmCoder::~AsmCoder()
{
}

/**
 * @brief Disassemble a PEX binary.
 * The assembly is written to the output writer defined in the constructor.
 *
 * @param pex The binary to dissassemble.
 */
void Decompiler::AsmCoder::code(const Pex::Binary &pex)
{
    writeInfo(pex);
    writeUserFlagsRef(pex);
    writeObjectTable(pex);
}

/**
 * @brief Writes the header.
 * This function writes the content of the PEX header as an info block.
 * @param pex The source binary.
 */
void Decompiler::AsmCoder::writeInfo(const Pex::Binary &pex)
{
    auto& header = pex.getHeader();
    auto& debug  = pex.getDebugInfo();
    write(".info");
    write(indent(1) << ".source \"" << header.getSourceFileName() << "\"");
    if (debug.getModificationTime() != 0)
    {
        write(indent(1) << ".modifyTime " << debug.getModificationTime() << " ;" << std::put_time(std::localtime(&debug.getModificationTime()), "%Y-%m-%d %H:%M:%S"));
    }
    write(indent(1) << ".compileTime " << header.getCompilationTime() << " ;" << std::put_time(std::localtime(&header.getCompilationTime()), "%Y-%m-%d %H:%M:%S"));

    write(indent(1) << ".user \"" << header.getUserName() << "\"");
    write(indent(1) << ".computer \"" << header.getComputerName() << "\"");
    write(".endInfo");
}

/**
 * @brief Writes the user flag definition.
 * @param pex The source binary.
 */
void Decompiler::AsmCoder::writeUserFlagsRef(const Pex::Binary &pex)
{
    auto& flags   = pex.getUserFlags();
    write(".userFlagsRef");
    for (auto& flag : flags)
    {
        write(indent(1) << ".flag " << flag.getName() << " " << (int)flag.getFlagIndex()
              << " ;0x" << std::setw(8) << std::setfill('0') << std::hex << std::uppercase << (1 << flag.getFlagIndex()));
    }
    write(".endUserFlagsRef");
}

/**
 * @brief Writes the objects definitions.
 * @param pex The source binary.
 */
void Decompiler::AsmCoder::writeObjectTable(const Pex::Binary &pex)
{
    write(".objectTable");
    for (auto& object : pex.getObjects())
    {
        write(indent(1) << ".object " << object.getName() << " " << object.getParentClassName());
        writeUserFlags(indent(2), object, pex);
        write(indent(2) << ".docString \"" << object.getDocString() << '"');
        write(indent(2) << ".autoState " << object.getAutoStateName());
        writeVariableTable(2, object, pex);
        writePropertyTable(2, object, pex);
        writeStateTable(2, object, pex);
        write(indent(1) << ".endObject");
    }
    write(".endObjectTable");
}

/**
 * @brief Writes the variable table of an object
 * @param i The indentation level.
 * @param object The object containing the variables.
 * @param pex The source binary.
 */
void Decompiler::AsmCoder::writeVariableTable(int i, const Pex::Object &object, const Pex::Binary &pex)
{
    write(indent(i) << ".variableTable");
    for (auto& var : object.getVariables())
    {
        write(indent(i+1) << ".variable " << var.getName() << " " << var.getTypeName());
        writeUserFlags(indent(i+2), var, pex);
        write(indent(i+2) << ".initialValue " << var.getDefaultValue().toString());
        write(indent(i+1) << ".endVariable");
    }
    write(indent(i) << ".endVariableTable");
}

/**
 * @brief Writes the properties table of an object
 * @param i The indentation level.
 * @param object The object containing the properties.
 * @param pex The source binary.
 */
void Decompiler::AsmCoder::writePropertyTable(int i, const Pex::Object &object, const Pex::Binary &pex)
{
    write(indent(i) << ".propertyTable");
    for (auto& property : object.getProperties())
    {
        //TODO:Check for auto read only
        write(indent(i+1) << ".property " << property.getName() << " " << property.getTypeName() << (property.hasAutoVar()?" auto":""));
        writeUserFlags(indent(i+2), property, pex);
        write(indent(i+2) << ".docString \"" << property.getDocString() << '"');

        if(property.hasAutoVar())
        {
            write(indent(i+2) << ".autovar " << property.getAutoVarName());
        }
        else
        {
            auto& noState = pex.getStringTable().findIdentifier("");

            if (property.isReadable())
            {
                writeFunction(i+2, property.getReadFunction(), pex, pex.getDebugInfo().getFunctionInfo(object.getName(),noState, property.getName(), Pex::DebugInfo::FunctionType::Getter), "get");
            }
            if (property.isWritable())
            {
                writeFunction(i+2, property.getWriteFunction(), pex, pex.getDebugInfo().getFunctionInfo(object.getName(), noState, property.getName(), Pex::DebugInfo::FunctionType::Setter), "set");
            }
        }
        write(indent(i+1) << ".endProperty");
    }
    write(indent(i) << ".endPropertyTable");
}

/**
 * @brief Writes the states table of an object
 * @param i The indentation level.
 * @param object The object containing the properties.
 * @param pex The source binary.
 */
void Decompiler::AsmCoder::writeStateTable(int i, const Pex::Object &object, const Pex::Binary &pex)
{
    write(indent(i) << ".stateTable");
    for (auto& state : object.getStates())
    {
        write(indent(i+1) << ".state " << state.getName());
        for (auto& function : state.getFunctions())
        {
            writeFunction(i+2, function, pex, pex.getDebugInfo().getFunctionInfo(object.getName(), state.getName(), function.getName()));
        }
        write(indent(i+1) << ".endState");
    }
    write(indent(i) << ".endStateTable");
}

/**
 * @brief Writes a function
 * @param i The indentation level.
 * @param function The function to write.
 * @param pex The source binary.
 * @param info A pointer to the debug information of the function. The pointer can be null.
 * @param name Name of the function. This name overrides the name defined in the function object.
 */
void Decompiler::AsmCoder::writeFunction(int i, const Pex::Function &function, const Pex::Binary &pex, const Pex::DebugInfo::FunctionInfo *info, const std::string &name)
{
    std::string functionName(name);
    if (functionName.empty())
    {
        functionName = function.getName().asString();
    }

    write(indent(i) << ".function " << functionName);
    if (info)
    {
        write(indent(i+1) << " ; function type " << (int)info->getFunctionType());
    }
    writeUserFlags(indent(i+1), function, pex);
    write(indent(i+1) << ".docString \"" << function.getDocString().asString() << '"');

    write(indent(i+1) << ".return " << function.getReturnTypeName().asString());

    write(indent(i+1) << ".paramTable");
    for (auto& param : function.getParams())
    {
        write(indent(i+2) << ".param " << param.getName() << " " << param.getTypeName());
    }
    write(indent(i+1) << ".endParamTable");

    write(indent(i+1) << ".localTable");
    for (auto& local : function.getLocals())
    {
        write(indent(i+2) << ".local " << local.getName() << " " << local.getTypeName());
    }
    write(indent(i+1) << ".endLocalTable");

    write(indent(i+1) << ".code");
    writeCode(i+1, function.getInstructions(), info);
    write(indent(i+1) << ".endCode");


    write(indent(i) << ".endFunction " << ";" << functionName);

}

/**
 * @brief Writes the instructions of a function.
 * @param i The indentation level.
 * @param instructions The instructions list.
 * @param pex The source binary.
 * @param info A pointer to the debug information of the function. The pointer can be null.
 */
void Decompiler::AsmCoder::writeCode(int i, const Pex::Instructions &instructions, const Pex::DebugInfo::FunctionInfo *info)
{
    std::map<std::uint32_t, std::uint32_t> label;
    std::uint32_t nextLabel = 0;

    auto currentLine = -1;

    // Loop through the instruction list to find jumps
    // And compute the labels positions.
    std::uint32_t ip = 0;
    for (auto& ins : instructions)
    {
        switch(ins.getOpCode())
        {
        case Pex::OpCode::JMP:
        {
            assert(ins.getArgs().size() == 1);
            assert(ins.getArgs()[0].getType() == Pex::ValueType::Integer);

            auto target = ip + ins.getArgs()[0].getInteger();
            if (label.find(target) == label.end())
            {
                label[target] = nextLabel;
                ++nextLabel;
            }
        }
            break;
        case Pex::OpCode::JMPF:
        case Pex::OpCode::JMPT:
        {
            assert(ins.getArgs().size() == 2);
            assert(ins.getArgs()[1].getType() == Pex::ValueType::Integer);

            auto target = ip + ins.getArgs()[1].getInteger();
            if (label.find(target) == label.end())
            {
                label[target] = nextLabel;
                ++nextLabel;
            }
        }
            break;
        default:
            break;
        }

        ++ip;
    }

    // Loop through the instruction list and writes the opcodes.
    ip = 0;
    for (auto& ins : instructions)
    {
        // Check the current source line number according to the debug informations.
        if (info && ip < info->getLineNumbers().size() && info->getLineNumbers()[ip] != currentLine )
        {
            currentLine = info->getLineNumbers()[ip];
            write(indent(i+1) << "; line " << currentLine);
        }
        // Check if a label must be emitted.
        if (label.find(ip) != label.end())
        {
            write(indent(i) << "_label" << label[ip] << ':');
        }
        auto stream = indent(i + 1);
        stream << ins.getOpCodeName() << " ";
        switch(ins.getOpCode())
        {
        case Pex::OpCode::JMP:
        {
            assert(ins.getArgs().size() == 1);
            assert(ins.getArgs()[0].getType() == Pex::ValueType::Integer);

            auto target = ip + ins.getArgs()[0].getInteger();

            stream << "_label" << label[target];
        }
            break;
        case Pex::OpCode::JMPF:
        case Pex::OpCode::JMPT:
        {
            assert(ins.getArgs().size() == 2);
            assert(ins.getArgs()[1].getType() == Pex::ValueType::Integer);

            stream << ins.getArgs()[0].toString() << " ";
            auto target = ip + ins.getArgs()[1].getInteger();

            stream << "_label" << label[target];

        }
            break;
        default:
        {
            for (auto& arg : ins.getArgs())
            {
                stream << arg.toString() << " ";
            }

            if (ins.hasVarArgs())
            {
                for (auto& arg : ins.getVarArgs())
                {
                    stream << arg.toString() << " ";
                }
                stream << ";" << ins.getVarArgs().size() << " variable args";
            }
        }
            break;
        }


        write(stream);
        ++ip;
    }
    // Write the last label, if one.
    if (label.find(ip) != label.end())
    {
        write(indent(i) << "_label" << label[ip]);
    }
}

/**
 * @brief Writes the User Flags associated with an element to a stream.
 * @param stream The stream to write.
 * @param flagged The flagged element.
 * @param pex The source binary.
 */
void Decompiler::AsmCoder::writeUserFlags(std::ostream& stream, const Pex::UserFlagged &flagged, const Pex::Binary &pex)
{
    auto& flagsref = pex.getUserFlags();

    auto flags = flagged.getUserFlags();

    stream << ".userFlags " << (int)flags << " ;";

    if (flags != 0x0000)
    {
        for (auto& flagref : flagsref)
        {
            if (flags & (1 << flagref.getFlagIndex()))
            {
                stream << flagref.getName() << " ";
            }
        }
    }
    else
    {
        stream << "none";
    }
    write(stream);
}
