#ifndef UNARYOPERATOR_HPP
#define UNARYOPERATOR_HPP

#include <string>

#include "Base.hpp"
#include "FieldNodeMixin.hpp"

namespace Node {

class UnaryOperator : public Base,
        public FieldValueNodeMixin<0>
{
public:
    UnaryOperator(size_t ip, std::uint8_t precedence, const Pex::StringTable::Index& result, const std::string& op, BasePtr value);
    virtual ~UnaryOperator();

    static std::shared_ptr<UnaryOperator> make(size_t ip, std::uint8_t precedence, const Pex::StringTable::Index& result, const std::string& op, BasePtr value);

    virtual void       visit(Visitor* visitor);

    const std::string& getOperator() const;

protected:
    std::string m_Op;

};

}
#endif // UNARYOPERATOR_HPP
