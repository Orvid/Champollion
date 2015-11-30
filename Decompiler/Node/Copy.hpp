#ifndef COPY_HPP
#define COPY_HPP

#include "Base.hpp"
#include "FieldNodeMixin.hpp"

namespace Node {

class Copy : public Base,
        public FieldValueNodeMixin<0>
{
public:
    Copy(size_t ip, const Pex::StringTable::Index& result, BasePtr expr);
    virtual ~Copy();

    static std::shared_ptr<Copy> make(size_t ip, const Pex::StringTable::Index& result, BasePtr expr);

    virtual void                 visit(Visitor* visitor);

protected:

};

}
#endif // COPY_HPP
