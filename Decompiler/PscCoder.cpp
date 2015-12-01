#include "PscCoder.hpp"

#include <cassert>
#include <ctime>
#include <iostream>
#include <iomanip>


#include "PscDecompiler.hpp"

/**
 * @brief Constructor
 * Builds an object associated with an output writer.
 *
 * @param writer Pointer to the output writer. The ownership is transferred.
 */
Decompiler::PscCoder::PscCoder(Decompiler::OutputWriter *writer)  :
    Coder(writer),
    m_CommentAsm(false)
{
}

/**
 * @brief Default desctructor
 */
Decompiler::PscCoder::~PscCoder()
{
}

/**
 * @brief Decompile a PEX binary to a Papyrus file.
 * @param pex
 */
void Decompiler::PscCoder::code(const Pex::Binary &pex)
{
    writeHeader(pex);
    for(auto& object : pex.getObjects())
    {
        writeObject(object, pex);
    }

}

/**
 * @brief Set the option to output Assembly instruction in comments
 * @param commentAsm True to write the comments.
 * @return A reference to this.
 */
Decompiler::PscCoder &Decompiler::PscCoder::outputAsmComment(bool commentAsm)
{
    m_CommentAsm = commentAsm;
    return *this;
}

/**
 * @brief Write the content of the PEX header as a block comment.
 * @param pex Binary to decompile.
 */
void Decompiler::PscCoder::writeHeader(const Pex::Binary &pex)
{
    auto& header = pex.getHeader();
    auto& debug  = pex.getDebugInfo();
    write(";/ Decompiled by Champollion V1.0.1");
    write(indent(0) << "Source   : " << header.getSourceFileName());
    if (debug.getModificationTime() != 0)
    {
        write(indent(0) << "Modified : " << std::put_time(std::localtime(&debug.getModificationTime()), "%Y-%m-%d %H:%M:%S"));
    }
    write(indent(0) << "Compiled : " << std::put_time(std::localtime(&header.getCompilationTime()), "%Y-%m-%d %H:%M:%S"));
    write(indent(0) << "User     : " << header.getUserName());
    write(indent(0) << "Computer : " << header.getComputerName());
    write("/;");
}

/**
 * @brief Write an object contained in the binary.
 * @param object Object to decompile
 * @param pex Binary to decompile.
 */
void Decompiler::PscCoder::writeObject(const Pex::Object &object, const Pex::Binary &pex)
{
    auto stream = indent(0);
    stream <<"scriptName " << object.getName().asString();
    if (! object.getParentClassName().asString().empty())
    {
        stream << " extends " << object.getParentClassName().asString();
    }

    writeUserFlag(stream, object, pex);
    write(stream);

    writeDocString(0, object);

    write("");
    write(";-- Properties --------------------------------------");
    writeProperties(object, pex);

    write("");
    write(";-- Variables ---------------------------------------");
    writeVariables(object, pex);

    writeStates(object, pex);
}

/**
 * @brief Write the properties definitions stored in the object.
 * @param object Object containing the properties definitions.
 * @param pex Binary to decompile.
 */
void Decompiler::PscCoder::writeProperties(const Pex::Object &object, const Pex::Binary &pex)
{
    for(auto& property : object.getProperties())
    {
        auto stream = indent(0);
        stream << property.getTypeName() << " property " << property.getName();
        if (property.hasAutoVar())
        {
            auto var = object.getVariables().findByName(property.getAutoVarName());
            if(var == nullptr)
            {
                throw std::runtime_error("Auto variable for property not found");
            }
            auto initialValue = var->getDefaultValue();
            if (initialValue.getType() != Pex::ValueType::None)
            {
                stream << " = " << initialValue.toString();
            }
            if (! property.isWritable())
            {
                stream << " autoreadonly";
            }
            else
            {
                stream << " auto";
            }

            // The flags defined on the variable must be set on the property
            writeUserFlag(stream, *var, pex);
        }
        writeUserFlag(stream, property, pex);
        write(stream);
        writeDocString(0, property);

        if (!property.hasAutoVar())
        {
            if(property.isReadable())
            {
                writeFunction(1, property.getReadFunction(),object , pex, "get");
            }
            if(property.isWritable())
            {
                writeFunction(1, property.getWriteFunction(),object , pex, "set");
            }
            write(indent(0) << "endproperty");
        }
    }
}

/**
 * @brief Write the variables stored in the object.
 * @param object Object containing the variables.
 * @param pex Binary to decompile.
 */
void Decompiler::PscCoder::writeVariables(const Pex::Object &object, const Pex::Binary &pex)
{
    for (auto& var : object.getVariables())
    {
        auto name = var.getName().asString();
        bool compilerGenerated = (name.size() > 2 && name[0] == ':' && name[1] == ':');
        auto stream = indent(0);
        if (compilerGenerated)
        {
            stream << "; ";
        }
        stream << var.getTypeName() << " " << name;
        auto initialValue = var.getDefaultValue();
        if (initialValue.getType() != Pex::ValueType::None)
        {
            stream << " = " << initialValue.toString();
        }
        writeUserFlag(stream, var, pex);

        if (m_CommentAsm || !compilerGenerated)
        {
            write(stream);
        }
    }
}


/**
 * @brief Write the states contained in the object.
 * @param object Object containing the states.
 * @param pex Binary to decompile.
 */
void Decompiler::PscCoder::writeStates(const Pex::Object &object, const Pex::Binary &pex)
{
    for (auto& state : object.getStates())
    {
        auto name = state.getName().asString();
        if (name.empty())
        {
            write("");
            write(";-- Functions ---------------------------------------");
            writeFunctions(0, state, object, pex);
        }
        else
        {
            write("");
            write(";-- State -------------------------------------------");
            auto stream = indent(0);

            // The auto state name canbe a different index than the state name, event if it is the same value.
            if (_stricmp(state.getName().asString().c_str(), object.getAutoStateName().asString().c_str()) == 0)
            {
                stream << "auto ";
            }
            write(stream << "state " << state.getName().asString());
            writeFunctions(1, state, object, pex);
            write(indent(0) << "endState");
        }
    }
}

/**
 * @brief Writes the functions associated with a state.
 * @param i The indentation level.
 * @param state State containing the functions.
 * @param object Object Containing the states.
 * @param pex Binary to decompile.
 */
void Decompiler::PscCoder::writeFunctions(int i, const Pex::State &state, const Pex::Object& object, const Pex::Binary &pex)
{
    for (auto& func : state.getFunctions())
    {
        write("");
        writeFunction(i, func, object, pex);
    }
}

/**
 * @brief Decompile a function.
 * @param i The indentation level.
 * @param function The function to decompile.
 * @param object The Object containing the function.
 * @param pex Binary to decompile.
 * @param name Name of the function. This parameter override the name stored in the function object.
 */
void Decompiler::PscCoder::writeFunction(int i, const Pex::Function &function, const Pex::Object& object, const Pex::Binary &pex, const std::string &name)
{
    std::string functionName = name;

    if (functionName.empty())
    {
        functionName = function.getName().asString();
    }


    if (functionName != "GetState" && functionName != "GotoState")
    {
        auto stream = indent(i);
        if (_stricmp(function.getReturnTypeName().asString().c_str(), "none") != 0)
        {
            stream << function.getReturnTypeName().asString() << " ";
        }
        stream << "function " << functionName << "(";

        auto first = true;
        for (auto& param : function.getParams())
        {
            if (first)
            {
                first = false;
            }
            else
            {
                stream << ", ";
            }
            stream << param.getTypeName() << " " << param.getName();
        }
        stream << ")";

        if (function.isGlobal())
        {
            stream << " global";
        }
        if (function.isNative())
        {
            stream << " native";
        }
        writeUserFlag(stream, function, pex);
        write(stream);

        writeDocString(i, function);

        if (! function.isNative())
        {
            for (auto& line : PscDecompiler(function, object, m_CommentAsm))
            {
                write(indent(i+1) << line);
            }
            write(indent(i) << "endFunction");
        }
    }
    else
    {
        write(indent(i) << "; Skipped compiler generated " << functionName);
    }
}

/**
 * @brief Write the user flags associated with an item.
 * @param stream Stream to write the flags to.
 * @param flagged Flagged item.
 * @param pex Binary to decompile.
 */
void Decompiler::PscCoder::writeUserFlag(std::ostream& stream, const Pex::UserFlagged &flagged, const Pex::Binary &pex)
{
    auto flags = flagged.getUserFlags();
    for (auto& flag : pex.getUserFlags())
    {
        if (flags & flag.getFlagMask())
        {
            stream << " " << flag.getName();
        }
    }
}

/**
 * @brief Write the documentation string of an item.
 * @param i Indentation level.
 * @param item Documented item.
 */
void Decompiler::PscCoder::writeDocString(int i, const Pex::DocumentedItem &item)
{
    if (! item.getDocString().asString().empty())
    {
        write(indent(i) << "{" << item.getDocString().asString() << "}");
    }
}
