#include "Base.hpp"

#include <algorithm>
#include <cassert>

Node::Base::Base(size_t childs, size_t ip, uint8_t precedence, const Pex::StringTable::Index &result) :
    std::deque<BasePtr>(childs),
    m_Begin(ip),
    m_End(ip),
    m_FixedSize(childs != 0),
    m_Precedence(precedence),
    m_Result(result)
{
}

Node::Base::~Base()
{
    for (auto child : *this)
    {
        if (child)
            child->m_Parent = nullptr;
    }
}

bool Node::Base::isFinal() const
{
    if (m_Result.isValid() && !m_Result.isUndefined())
    {
        auto& id = m_Result.asString();
        return id.substr(0, 6) != "::temp" && _stricmp(id.c_str(), "::nonevar") != 0;
    }
    return true;

}

Node::Base &Node::Base::operator <<(Node::BasePtr child)
{
    assert(!m_FixedSize);

    if (child->getParent())
        child->getParent()->removeChild(child);

    push_back(child);
    child->m_Parent = this;
    return *this;
}

void Node::Base::setChild(size_t c, Node::BasePtr child)
{
    assert(c < size());

    if (child)
    {
        if (child->getParent())
            child->getParent()->removeChild(child);

        if (operator[](c))
            operator[](c)->m_Parent = nullptr;

        operator[](c) = child;
        child->m_Parent = this;
    }
    else
    {
        operator[](c) = child;
    }
}

void Node::Base::mergeChildren(Node::BasePtr source)
{
    for (auto child : *source)
    {
        push_back(child);
        child->m_Parent = this;
    }
    source->clear();
}

Node::BasePtr Node::Base::getParent() const
{
    if (m_Parent)
        return m_Parent->shared_from_this();
    else
        return nullptr;
}

void Node::Base::removeChild(Node::BasePtr child)
{
    auto it = std::find(begin(), end(), child);
    if (it != end())
    {
        (*it)->m_Parent = nullptr;

        if (m_FixedSize)
            *it = nullptr;
        else
            erase(it);
    }
}

void Node::Base::replaceChild(Node::BasePtr child, Node::BasePtr newChild)
{
    assert(child->m_Parent == this && child != newChild);

    if (newChild->m_Parent)
        newChild->m_Parent->removeChild(newChild);

    auto childPosition = std::find(begin(), end(), child);

    child->m_Parent = nullptr;
    newChild->m_Parent = this;
    *childPosition = newChild;
}

void Node::Base::computeInstructionBounds()
{
    for (auto child : *this)
    {
        if (child)
        {
            child->computeInstructionBounds();

            if (m_Begin == -1)
                m_Begin = child->getBegin();
            else if (child->getBegin() != -1)
                m_Begin = std::min(m_Begin, child->getBegin());

            if (m_End == -1)
                m_End = child->getEnd();
            else if (child->getEnd() != -1)
                m_End = std::max(m_End, child->getEnd());
        }
    }
}

void Node::Base::includeInstruction(size_t ip)
{
    if (m_Begin == -1)
        m_End = m_Begin = ip;
    else if (ip < m_Begin)
        m_Begin = ip;
    else if (ip > m_End)
        m_End = ip;
}
