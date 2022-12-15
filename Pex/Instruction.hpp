#pragma once

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
    // New in Fallout 4
    IS,
    STRUCT_CREATE,
    STRUCT_GET,
    STRUCT_SET,
    ARRAY_FINDSTRUCT,
    ARRAY_RFINDSTRUCT,
    ARRAY_ADD,
    ARRAY_INSERT,
    ARRAY_REMOVELAST,
    ARRAY_REMOVE,
    ARRAY_CLEAR,
	//New in Fallout 76
	ARRAY_GETALLMATCHINGSTRUCTS,
    MAX_OPCODE
};


/**
 * @brief ByteCode Instruction.
 *
 */
class Instruction
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
