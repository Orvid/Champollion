#ifndef BASE_HPP
#define BASE_HPP

#include <cstdint>
#include <memory>
#include <deque>

#include "Pex/Value.hpp"

namespace Node {

class Visitor;

class Base;
typedef std::shared_ptr<Base> BasePtr;

class Base :
    public std::deque<BasePtr>,
    public std::enable_shared_from_this<Base>
{
public:
    Base(size_t childs, size_t ip, std::uint8_t precedence, const Pex::StringTable::Index& result = Pex::StringTable::Index());
    virtual ~Base();

    template<typename T>
    bool is() const
    {
        return dynamic_cast<const T*>(this) != nullptr;
    }

    template<typename T>
    T* as()
    {
        return dynamic_cast<T*>(this);
    }

    size_t getBegin() const;
    size_t getEnd() const;
    std::uint8_t getPrecedence() const;

    const Pex::StringTable::Index& getResult() const;
    void clearResult();

    bool isFinal() const;

    virtual void visit(Visitor* visitor) = 0;

    Base& operator << (BasePtr child);
    void setChild(size_t c, BasePtr child);
    void mergeChildren(BasePtr source);

    BasePtr getParent() const;
    void removeChild(BasePtr child);
    void replaceChild(BasePtr child, BasePtr newChild);

    virtual void computeInstructionBounds();
    void includeInstruction(size_t ip);

protected:
    size_t m_Begin;
    size_t m_End;
    bool m_FixedSize;
    std::uint8_t m_Precedence;
    Pex::StringTable::Index m_Result;
    Base* m_Parent;

};


}


#endif // BASE_HPP
