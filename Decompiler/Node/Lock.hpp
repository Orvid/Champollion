#pragma once

#include <cassert>
#include <cstdint>
#include <memory>

#include "Base.hpp"
#include "FieldNodeMixin.hpp"
#include "Scope.hpp"
#include "Visitor.hpp"

namespace Node {

    class Lock final :
            public Base,
            public FieldParametersNodeMixin<0>,
            public FieldBodyNodeMixin<1>
    {
    public:
        Lock(size_t ip, BasePtr body) :
                Base(2, ip, 10),
                FieldParametersNodeMixin(this, std::make_shared<Params>()),
                FieldBodyNodeMixin(this, body)
        {
        }
        virtual ~Lock() = default;

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
