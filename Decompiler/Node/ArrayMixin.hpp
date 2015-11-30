#ifndef ARRAYMIXIN_HPP
#define ARRAYMIXIN_HPP

#include <string>
#include "Base.hpp"


namespace Node {
class ArrayMixin
{
public:
    ArrayMixin(BasePtr array);
    BasePtr getArray() const;
    void    setArray(BasePtr array);

protected:
    BasePtr     m_Array;
};
}

#endif // ARRAYMIXIN_HPP
