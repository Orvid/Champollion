#ifndef BINARYOPERATOR_HPP
#define BINARYOPERATOR_HPP

#include <string>

#include "Base.hpp"
#include "FieldNodeMixin.hpp"

namespace Node {

class BinaryOperator : public Base,
        public FieldLeftNodeMixin<0>,
        public FieldRightNodeMixin<1>
{
public:
    BinaryOperator(size_t ip, std::uint8_t precedence, const Pex::StringTable::Index& result, BasePtr left, const std::string& op, BasePtr right);
    virtual ~BinaryOperator();

    static std::shared_ptr<BinaryOperator> make(size_t ip, std::uint8_t precedence, const Pex::StringTable::Index& result, BasePtr left, const std::string& op, BasePtr right);

    virtual void                           visit(Visitor* visitor);

    const std::string&                     getOperator() const;

    virtual void                           computeInstructionBounds();
protected:
    std::string m_Op;

};

}
#endif // BINARYOPERATOR_HPP
