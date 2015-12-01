#include "PscCodeBlock.hpp"

#include <cassert>


/**
 * @brief Constructor.
 * This constructor builds a blocks containing instructions from ranging from begin to end, inclusive.
 * @param begin Indice of the first instruction of the block
 * @param end Indice of the last instruction of the block.
 */
Decompiler::PscCodeBlock::PscCodeBlock(size_t begin, size_t end) :
    m_Begin(begin),
    m_End(end),
    m_Next(END),
    m_OnFalse(END),
    m_Scope(std::make_shared<Node::Scope>())
{
}

/**
 * @brief Default destructor
 */
Decompiler::PscCodeBlock::~PscCodeBlock()
{
}

/**
 * @brief Get the indice of the first instruction of the block
 * @return The indice of the first instruction.
 */
size_t Decompiler::PscCodeBlock::getBegin() const
{
    return m_Begin;
}

/**
 * @brief Get the indice of the last instruction of the block.
 * @return The indice of the last instruction.
 */
size_t Decompiler::PscCodeBlock::getEnd() const
{
    return m_End;
}

/**
 * @brief Get the indice of the next block for inconditional jump.
 * @return The indice of the next block.
 */
size_t Decompiler::PscCodeBlock::getNext() const
{
    return m_Next;
}

/**
 * @brief Get the indice of the block for the true condition.
 * @return The indice of the true block.
 */
size_t Decompiler::PscCodeBlock::onTrue() const
{
    return m_Next;
}

/**
 * @brief Get the indice of the block for the false condition.
 * @return The indice of the false block.
 */
size_t Decompiler::PscCodeBlock::onFalse() const
{
    return m_OnFalse;
}

/**
 * @brief Get the name of variable holding the value for the true/false block selection.
 * @return The name of the variable.
 */
const Pex::StringTable::Index &Decompiler::PscCodeBlock::getCondition() const
{
    return m_Condition;
}


/**
 * @brief Check if the block is conditional.
 * @return True if the block has a condition variable.
 */
bool Decompiler::PscCodeBlock::isConditional() const
{
    return m_Condition.isValid() && !m_Condition.isUndefined();
}

/**
 * @brief Set the end indice of the block.
 * @param end The new end of the block.
 */
void Decompiler::PscCodeBlock::setEnd(size_t end)
{
    m_End = end;
}

/**
 * @brief Set the next block for unconditional block.
 * @param next The next block.
 */
void Decompiler::PscCodeBlock::setNext(size_t next)
{
    m_Next = next;
}

/**
 * @brief Set the condition variable and the two destination blocks.
 * @param condition The name index of the condition variable.
 * @param ontrue The indice of the block for the true branch.
 * @param onfalse The indice of the block for the false branch.
 */
void Decompiler::PscCodeBlock::setCondition(const Pex::StringTable::Index &condition, size_t ontrue, size_t onfalse)
{
    m_Condition = condition;
    m_Next = ontrue;
    m_OnFalse = onfalse;
}

/**
 * @brief Split the block in two blocks and return the second one.
 * @param split Indice of the instruction starting the new block
 * @return A pointer to the new block. The ownership is transferred.
 */
Decompiler::PscCodeBlock *Decompiler::PscCodeBlock::split(size_t split)
{
    //assert(m_Begin < split && split <= m_End);
    auto* result = new PscCodeBlock(split, m_End);
    result->m_Next = m_Next;
    result->m_Condition = m_Condition;
    result->m_OnFalse = m_OnFalse;

    m_End = split - 1;
    m_Next = split;
    m_Condition = Pex::StringTable::Index();
    m_OnFalse = END;
    return result;
}

/**
 * @brief Get the scope node associated with the block.
 * @return A pointer to the scope block.
 */
Node::Scope *Decompiler::PscCodeBlock::getScope() const
{
    return static_cast<Node::Scope*>(m_Scope.get());
}
