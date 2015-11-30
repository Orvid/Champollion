#ifndef WHILE_HPP
#define WHILE_HPP

#include "Base.hpp"
#include "FieldNodeMixin.hpp"

namespace Node {

class While : public Base,
        public FieldConditionNodeMixin<0>,
        public FieldBodyNodeMixin<1>

{
public:
    While(size_t ip, BasePtr condition, BasePtr body);
    virtual ~While();

    static std::shared_ptr<While> make(size_t ip, BasePtr condition, BasePtr body);

    virtual void                  visit(Visitor* visitor);

    virtual void                  computeInstructionBounds();
protected:

};

}
#endif // WHILE_HPP
