#pragma once

#include <cassert>
#include <cstdint>
#include <memory>

#include "Base.hpp"
#include "FieldNodeMixin.hpp"
#include "Scope.hpp"
#include "Visitor.hpp"

namespace Node {

    class EndGuard final :
            public Base,
            public FieldParametersNodeMixin<0>
    {
    public:
        EndGuard(size_t ip) :
                Base(1, ip, 10),
                FieldParametersNodeMixin(this, std::make_shared<Params>())
        {
        }
        virtual ~EndGuard() = default;

        void visit(Visitor* visitor) override
        {
            assert(visitor);
            visitor->visit(this);
        }

        void computeInstructionBounds() override
        {
            Base::computeInstructionBounds();
        }
    };

}
