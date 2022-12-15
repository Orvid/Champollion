#include "Instruction.hpp"

#include <cassert>

/**
 * @brief Opcode information structure
 *
 * Contains the data about the opcodes
 */
struct OpCodeInfo {
    /**
     * @brief OpCode name
     */
    const char* name;
    /**
     * @brief number of fixed arguments
     */
    int         args;
    /**
     * @brief Use variable args
     */
    bool        varargs;
};

/**
 * Opcode definition table
 */
static const OpCodeInfo OPCODES[Pex::OpCode::MAX_OPCODE] = {
    {"nop", 0, false},
    {"iadd", 3, false},
    {"fadd", 3, false},
    {"isub", 3, false},
    {"fsub", 3, false},
    {"imul", 3, false},
    {"fmul", 3, false},
    {"idiv", 3, false},
    {"fdiv", 3, false},
    {"imod", 3, false},
    {"not",  2, false},
    {"ineg", 2, false},
    {"fneg", 2, false},
    {"assign", 2, false},
    {"cast", 2, false},
    {"cmp_eq", 3, false},
    {"cmp_lt", 3, false},
    {"cmp_lte", 3, false},
    {"cmp_gt", 3, false},
    {"comp_gte", 3, false},
    {"jmp", 1, false},
    {"jmpt", 2, false},
    {"jmpf", 2, false},
    {"callmethod", 3, true},
    {"callparent", 2, true},
    {"callstatic", 3, true},
    {"return", 1, false},
    {"strcat", 3, false},
    {"propget", 3, false},
    {"propset", 3, false},
    {"array_create", 2, false},
    {"array_length", 2, false},
    {"array_getlement", 3, false},
    {"array_setelement", 3, false},
    {"array_findelement", 4, false},
    {"array_rfindelement", 4, false},
    {"is", 3, false},
    {"struct_create", 1, false},
    {"struct_get", 3, false},
    {"struct_set", 3, false},
    {"array_findstruct", 5, false},
    {"array_rfindstruct", 5, false},
    {"array_add", 3, false},
    {"array_insert", 3, false},
    {"array_removelast", 1, false},
    {"array_remove", 3, false},
    {"array_clear", 1, false},
	{"array_getallmatchingstructs", 6, false},
};

/**
 * @brief Default constructor
 *
 * Create a NOP instruction
 */
Pex::Instruction::Instruction() :
    m_OpCode(OpCode::NOP)
{
}

/**
 * @brief Default desctructor
 */
Pex::Instruction::~Instruction()
{
}

/**
 * @brief Get the opcode of the instruction
 * @return the OpCode
 */
Pex::OpCode Pex::Instruction::getOpCode() const
{
    return m_OpCode;
}

/**
 * @brief Get the opcode name
 * @return the opcode name
 */
const char *Pex::Instruction::getOpCodeName() const
{
    return OPCODES[static_cast<int>(m_OpCode)].name;
}

/**
 * @brief Get the number of mandatory arguments needed by the instruction
 * @return the number of arguments
 */
int Pex::Instruction::getOpCodeArgCount() const
{
    return OPCODES[static_cast<int>(m_OpCode)].args;
}

/**
 * @brief Set the opcode of the instruction
 * @param value The new opcode
 */
void Pex::Instruction::setOpCode(Pex::OpCode value)
{
    assert(value < OpCode::MAX_OPCODE);
    m_OpCode = value;
    m_Args.clear();
    m_VarArgs.clear();
}

/**
 * @brief Get the mandatory arguments list
 * @return the const list of arguments
 */
const Pex::Instruction::Args &Pex::Instruction::getArgs() const
{
    return m_Args;
}

/**
 * @brief Get the mandatory arguments list
 * @return the modifiable list of arguments;
 */
Pex::Instruction::Args &Pex::Instruction::getArgs()
{
    return m_Args;
}

/**
 * @brief Check if the instruction allows a list of variable arguments
 * @return true if the opcode allow variable arguments
 */
bool Pex::Instruction::hasVarArgs() const
{
    return OPCODES[static_cast<int>(m_OpCode)].varargs;
}

/**
 * @brief Get the list of variable arguments
 * @return the const list of variable arguments
 */
const Pex::Instruction::Args &Pex::Instruction::getVarArgs() const
{
    return m_VarArgs;
}

/**
 * @brief Get the list of variable arguments
 * @return the modifiable list of variable arguments
 */
Pex::Instruction::Args &Pex::Instruction::getVarArgs()
{
    return m_VarArgs;
}
