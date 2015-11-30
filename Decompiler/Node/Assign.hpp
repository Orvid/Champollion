#ifndef ASSIGN_HPP
#define ASSIGN_HPP


#include "Base.hpp"
#include "FieldNodeMixin.hpp"

namespace Node {

class Assign : public Base,
        public FieldValueNodeMixin<0>,
        public FieldDestinationNodeMixin<1>
{
public:
    Assign(size_t ip, BasePtr destination, BasePtr expr);
    virtual ~Assign();

    static std::shared_ptr<Assign> make(size_t ip, BasePtr destination, BasePtr expr);

    virtual void                   visit(Visitor* visitor);

protected:

};

}
#endif // ASSIGN_HPP
