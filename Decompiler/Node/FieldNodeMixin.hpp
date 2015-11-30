#ifndef FIELDNODEMIXIN_HPP
#define FIELDNODEMIXIN_HPP

#include <cstdint>
#include <cassert>

#include "Base.hpp"

namespace Node {

#define DECLARE_FIELD(NAME)                         \
template<size_t N>                                  \
class Field##NAME##NodeMixin                        \
{                                                   \
   protected:                                       \
    Base* m_This;                                   \
    Field##NAME##NodeMixin (Base* _this, BasePtr init) : \
        m_This(_this)                               \
    {                                               \
        set##NAME(init);                            \
    };                                              \
   public:                                          \
    BasePtr get##NAME() const                       \
    {                                               \
        assert(N < m_This->size());                 \
        return m_This->operator [](N);              \
    }                                               \
    void    set##NAME(BasePtr value)                \
    {                                               \
        assert(N < m_This->size());                 \
        m_This->setChild(N, value);                 \
    }                                               \
};                                                  \


DECLARE_FIELD(Value)
DECLARE_FIELD(Parameters)
DECLARE_FIELD(Object)
DECLARE_FIELD(Left)
DECLARE_FIELD(Right)
DECLARE_FIELD(Array)
DECLARE_FIELD(Index)
DECLARE_FIELD(Condition)
DECLARE_FIELD(Body)
DECLARE_FIELD(Else)
DECLARE_FIELD(ElseIf)
DECLARE_FIELD(Destination)

}
#endif // FIELDNODEMIXIN_HPP
