#pragma once

#include <cassert>
#include <cstdint>
#include <memory>

#include "Base.hpp"
#include "FieldNodeMixin.hpp"
#include "Scope.hpp"
#include "Visitor.hpp"

namespace Node {

    class Unlock final :
            public Base,
            public FieldParametersNodeMixin<0>
    {
    public:
        Unlock(size_t ip) :
                Base(1, ip, 10),
                FieldParametersNodeMixin(this, std::make_shared<Params>())
        {
        }
        virtual ~Unlock() = default;

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
