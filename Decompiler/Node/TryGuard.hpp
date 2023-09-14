#pragma once

#include <cassert>
#include <cstdint>
#include <memory>

#include "Base.hpp"
#include "FieldNodeMixin.hpp"
#include "Scope.hpp"
#include "Visitor.hpp"

namespace Node {

    class TryGuard final :
            public Base,
            public FieldParametersNodeMixin<0>,
            public FieldBodyNodeMixin<1>
    {
    public:
        TryGuard(size_t ip, const Pex::StringTable::Index& result, BasePtr body) :
                Base(2, ip, 10, result),
                FieldParametersNodeMixin(this, std::make_shared<Params>()),
                FieldBodyNodeMixin(this, body)
        {
        }
        virtual ~TryGuard() = default;

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
