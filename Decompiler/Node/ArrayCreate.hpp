#ifndef ARRAYCREATE_HPP
#define ARRAYCREATE_HPP

#include <cstdint>

#include "Base.hpp"
#include "FieldNodeMixin.hpp"

namespace Node {

class ArrayCreate : public Base,
        public FieldIndexNodeMixin<0>
{
public:
    ArrayCreate(size_t ip, const Pex::StringTable::Index& result, const Node::BasePtr type, Node::BasePtr size);
    virtual ~ArrayCreate();

    static std::shared_ptr<ArrayCreate> make(size_t ip, const Pex::StringTable::Index& result, const Node::BasePtr type, Node::BasePtr size);

    virtual void                        visit(Visitor* visitor);

    const Node::BasePtr  getType() const;

protected:
    const Node::BasePtr     m_Type;
};

}

#endif // ARRAYCREATE_HPP
