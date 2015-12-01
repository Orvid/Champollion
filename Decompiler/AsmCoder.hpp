#pragma once

#include "Coder.hpp"

namespace Decompiler {

/**
 * @brief Write a PEX file as an ASM file.
 */
class AsmCoder :
        public Coder
{
public:
    AsmCoder(OutputWriter* writer);
    ~AsmCoder();

    virtual void code(const Pex::Binary& pex);

protected:


    void writeInfo(const Pex::Binary& pex);
    void writeUserFlagsRef(const Pex::Binary& pex);
    void writeObjectTable(const Pex::Binary& pex);

    void writeVariableTable(int i, const Pex::Object& object, const Pex::Binary& pex);
    void writePropertyTable(int i, const Pex::Object& object, const Pex::Binary& pex);
    void writeStateTable(int i, const Pex::Object& object, const Pex::Binary& pex);

    void writeFunction(int i, const Pex::Function& function, const Pex::Binary& pex, const Pex::DebugInfo::FunctionInfo* info, const std::string& name="");
    void writeCode(int i, const Pex::Instructions& instructions, const Pex::DebugInfo::FunctionInfo *info);

    void writeUserFlags(std::ostream& stream, const Pex::UserFlagged& flagged, const Pex::Binary& pex);

};
}
