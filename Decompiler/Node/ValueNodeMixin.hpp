#ifndef VALUENODEMIXIN_HPP
#define VALUENODEMIXIN_HPP

#include "Base.hpp"
#include <string>

namespace Node {
class ValueNodeMixin
{
public:
    ValueNodeMixin(BasePtr value);
    BasePtr getValue() const;
    void    setValue(BasePtr value);

protected:
    BasePtr     m_Value;
};
}
#endif // VALUENODEMIXIN_HPP
