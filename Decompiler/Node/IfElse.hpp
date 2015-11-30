#ifndef IFELSE_HPP
#define IFELSE_HPP

#include "Base.hpp"
#include "FieldNodeMixin.hpp"

namespace Node {

class IfElse : public Base,
        public FieldConditionNodeMixin<0>,
        public FieldBodyNodeMixin<1>,
        public FieldElseNodeMixin<2>,
        public FieldElseIfNodeMixin<3>

{
public:
    IfElse(size_t ip, BasePtr condition, BasePtr body, BasePtr elseBody);
    virtual ~IfElse();

    static std::shared_ptr<IfElse> make(size_t ip, BasePtr condition, BasePtr body, BasePtr elseBody);

    virtual void                   visit(Visitor* visitor);

    virtual void                   computeInstructionBounds();
protected:

};

}

#endif // IFELSE_HPP
