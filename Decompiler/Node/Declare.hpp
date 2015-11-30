#ifndef DECLARE_HPP
#define DECLARE_HPP

#include "Base.hpp"
#include "FieldNodeMixin.hpp"

namespace Node {

class Declare : public Base,
        public FieldObjectNodeMixin<0>
{
public:
    Declare(size_t ip, BasePtr identifier, const Pex::StringTable::Index& type);
    virtual ~Declare();

    static std::shared_ptr<Declare> make(size_t ip, BasePtr identifier, const Pex::StringTable::Index& type);

    virtual void visit(Visitor* visitor);


    const Pex::StringTable::Index& getType() const;

protected:
    Pex::StringTable::Index m_Type;
};
}
#endif // DECLARE_HPP
