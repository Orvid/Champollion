#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include "Pex_global.hpp"

#include <vector>

#include "Value.hpp"

namespace Pex{

/**
 * @brief List of available opcodes
 */
enum class OpCode {
    NOP,
    IADD,
    FADD,
    ISUB,
    FSUB,
    IMUL,
    FMUL,
    IDIV,
    FDIV,
    IMOD,
    NOT,
    INEG,
    FNEG,
    ASSIGN,
    CAST,
    CMP_EQ,
    CMP_LT,
    CMP_LTE,
    CMP_GT,
    CMP_GTE,
    JMP,
    JMPT,
    JMPF,
    CALLMETHOD,
    CALLPARENT,
    CALLSTATIC,
    RETURN,
    STRCAT,
    PROPGET,
    PROPSET,
    ARRAY_CREATE,
    ARRAY_LENGTH,
    ARRAY_GETELEMENT,
    ARRAY_SETELEMENT,
    ARRAY_FINDELEMENT,
    ARRAY_RFINDELEMENT,
    MAX_OPCODE
};


/**
 * @brief ByteCode Instruction.
 *
 */
class PEX_API Instruction
{
public:
    typedef std::vector<Value> Args;
public:
    Instruction();
    ~Instruction();

    OpCode getOpCode() const;
    void setOpCode(OpCode value);

    const char* getOpCodeName() const;
    int getOpCodeArgCount() const;

    const Args& getArgs() const;
    Args& getArgs();

    bool hasVarArgs() const;

    const Args& getVarArgs() const;
    Args& getVarArgs();
protected:
    OpCode m_OpCode;
    Args m_Args;
    Args m_VarArgs;
};

typedef std::vector<Instruction> Instructions;
}

#endif // INSTRUCTION_HPP
