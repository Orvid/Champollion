#ifndef ARRAYINDEXMIXIN_HPP
#define ARRAYINDEXMIXIN_HPP

#include <string>
#include "Base.hpp"


namespace Node {
class ArrayIndexMixin
{
public:
    ArrayIndexMixin(BasePtr index);
    BasePtr getIndex() const;
    void    setIndex(BasePtr index);

protected:
    BasePtr     m_Index;
};
}


#endif // ARRAYINDEXMIXIN_HPP
