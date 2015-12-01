#pragma once

#include <vector>

#include "Pex/StringTable.hpp"

#include "Node/Scope.hpp"

namespace Decompiler {

/**
 * @brief Code block data.
 *
 * The code block represent a direct sequence of instructions. A block ends with a jump, conditional or not,
 * and begins at the target of another jump. The obvious exceptions are the first and last block, which begins at the first instruction
 * and ends at the last instruction respectively.
 *
 * A block can be linked with another block which will be executed after it, or attached to a condition variable and two other blocks run if wether the
 * condition is true or false. The blocks sequence reflect the program instructions flow.
 *
 * Finally each block contains a Scope node which contains the statements and expression nodes computed by the block.
 *
 * The instructions and blocks are identified by their indices in the instruction array.
 */
class PscCodeBlock
{
public:
    static const size_t END = 0xFFFFFFFF;

public:
    PscCodeBlock(size_t getBegin, size_t getEnd);
    ~PscCodeBlock();


    size_t getBegin() const;
    size_t getEnd() const;
    size_t getNext() const;
    size_t onTrue() const;
    size_t onFalse() const;
    const Pex::StringTable::Index& getCondition() const;

    bool isConditional() const;

    void setEnd(size_t getEnd);
    void setNext(size_t getNext);
    void setCondition(const Pex::StringTable::Index& getCondition, size_t ontrue, size_t onfalse);

    PscCodeBlock* split(size_t split);

    Node::Scope* getScope() const;

protected:
    size_t m_Begin;
    size_t m_End;

    size_t m_Next;
    size_t m_OnFalse;
    Pex::StringTable::Index m_Condition;

    Node::BasePtr m_Scope;

};

}
