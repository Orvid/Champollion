#pragma once

#include <cassert>
#include <cstddef>
#include <deque>
#include <functional>

#include "Base.hpp"
#include "Visitor.hpp"

namespace Node {

template<typename T>
class WithNodeImplementation
{
    typedef std::function<bool (T*)> FilterFunction;
    typedef std::function<Node::BasePtr (T*)> TransformFunction;

    typedef std::deque<Node::BasePtr> ResultData;

public:
    WithNodeImplementation()
    {
        static_assert(std::is_base_of<Base, T>::value, "Only use on Base derived classes");

        m_FilterFunction = [=] (T*) {return true;};
    }
    ~WithNodeImplementation() = default;

    WithNodeImplementation& select(FilterFunction selector)
    {
        m_FilterFunction = selector;
        return *this;
    }

    WithNodeImplementation& transform(TransformFunction transform)
    {
        m_TransformFunction = transform;
        return *this;
    }

    ResultData from(BasePtr tree)
    {
        ResultData result;
        Node::DynamicVisitor nodeSelector;
        nodeSelector << (std::function<void (T*,  Node::DynamicVisitor*)>)[&] (T* node, Node::DynamicVisitor* visitor)
        {
            if (m_FilterFunction(node))
            {
                result.push_back(node->shared_from_this());
            }
            visitor->visitChildren(node);
        };

        tree->visit(&nodeSelector);

        return result;
    }

    int on(BasePtr tree)
    {
        assert(m_TransformFunction);

        int result = 0;
        Node::DynamicVisitor nodeSelector;
        nodeSelector << (std::function<void (T*,  Node::DynamicVisitor*)>)[&] (T* node, Node::DynamicVisitor* visitor)
        {
            visitor->visitChildren(node);
            if (m_FilterFunction(node))
            {
                assert(node->getParent());
                auto transformedNode = m_TransformFunction(node);
                if (transformedNode.get() != node)
                {
                    node->getParent()->replaceChild(node->shared_from_this(), transformedNode);
                }
                ++result;
            }
        };

        tree->visit(&nodeSelector);

        return result;
    }

protected:
    FilterFunction    m_FilterFunction;
    TransformFunction m_TransformFunction;
};

template<typename T>
WithNodeImplementation<T> WithNode()
{
    return WithNodeImplementation<T>();
}

}
