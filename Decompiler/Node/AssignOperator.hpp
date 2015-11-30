#ifndef ASSIGNOPERATOR_HPP
#define ASSIGNOPERATOR_HPP


#include "Base.hpp"
#include "FieldNodeMixin.hpp"

namespace Node {

class AssignOperator : public Base,
        public FieldValueNodeMixin<0>,
        public FieldDestinationNodeMixin<1>
{
public:
    AssignOperator(size_t ip, BasePtr destination, const std::string& op, BasePtr expr);
    virtual ~AssignOperator();

    static std::shared_ptr<AssignOperator> make(size_t ip, BasePtr destination, const std::string& op, BasePtr expr);

    virtual void                   visit(Visitor* visitor);

    const std::string&             getOperator() const;

protected:

    std::string m_Operator;
};

}

#endif // ASSIGNOPERATOR_HPP
