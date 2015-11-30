#include "Function.hpp"

/**
 * @brief Default constructor.
 */
Pex::Function::Function() :
    m_Flags(0)
{
}

/**
 * @brief Default destructor
 */
Pex::Function::~Function()
{
}

/**
 * @brief Retrieve the return type of the function.
 *
 * @return the index of the type name.
 */
Pex::StringTable::Index Pex::Function::getReturnTypeName() const
{
    return m_ReturnTypeName;
}

/**
 * @brief Sets the return type of the function
 *
 * @param[in] value Index of the return type.
 */
void Pex::Function::setReturnTypeName(Pex::StringTable::Index value)
{
    m_ReturnTypeName = value;
}

/**
 * @brief Sets the function flags byte.
 * @param[in] value Flag byte.
 */
void Pex::Function::setFlags(std::uint8_t value)
{
    m_Flags = value;
}

/**
 * @brief Check if the function is global
 *
 * @return true if the global flag is set
 */
bool Pex::Function::isGlobal() const
{
    return m_Flags & 0x01;
}

/**
 * @brief Check if the function is native
 * @return true if the native flag is set
 */
bool Pex::Function::isNative() const
{
    return m_Flags & 0x02;
}

/**
 * @brief Retrieve the list of parameters
 * @return the const list of parameters
 */
const Pex::TypedNames &Pex::Function::getParams() const
{
    return m_Params;
}

/**
 * @brief Retrieve the list of parameters
 * @return the modifiable list of parameters
 */
Pex::TypedNames &Pex::Function::getParams()
{
    return m_Params;
}

/**
 * @brief Retrieve the list of local variables
 * @return the const list of local variables
 */
const Pex::TypedNames &Pex::Function::getLocals() const
{
    return m_Locals;
}


/**
 * @brief Retrieve the list of local variables
 * @return the modifiable list of local variables
 */
Pex::TypedNames &Pex::Function::getLocals()
{
    return m_Locals;
}

/**
 * @brief Retrieve the list of bytecode instructions
 * @return the const list of instructions
 */
const Pex::Instructions &Pex::Function::getInstructions() const
{
    return m_Instructions;
}

/**
 * @brief Retrieve the list of bytecode instructions
 * @return the modifiable list of instructions
 */
Pex::Instructions &Pex::Function::getInstructions()
{
    return m_Instructions;
}
