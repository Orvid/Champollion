#ifndef RETURN_HPP
#define RETURN_HPP

#include <string>

#include "Base.hpp"
#include "FieldNodeMixin.hpp"

namespace Node {

class Return : public Base,
        public FieldValueNodeMixin<0>
{
public:
    Return(size_t ip, BasePtr expr);
    virtual ~Return();

    static std::shared_ptr<Return> make(size_t ip, BasePtr expr);

    virtual void visit(Visitor* visitor);

protected:

};

}
#endif // RETURN_HPP
