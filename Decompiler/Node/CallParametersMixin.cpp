#include "CallParametersMixin.hpp"


Node::CallParametersMixin::CallParametersMixin() :
    m_DirtyBounds(true)
{
}

Node::CallParametersMixin::~CallParametersMixin()
{
}

const Node::CallParametersMixin::Parameters &Node::CallParametersMixin::getParameters() const
{
    return m_Parameters;
}

Node::CallParametersMixin::Parameters &Node::CallParametersMixin::getParameters()
{
    m_DirtyBounds = true;
    return m_Parameters;
}

Node::CallParametersMixin &Node::CallParametersMixin::operator << (Node::BasePtr param)
{
    m_Parameters.push_back(param);
    return *this;
}

std::pair<size_t, size_t> Node::CallParametersMixin::getBounds() const
{
    std::pair<size_t, size_t> result = std::make_pair(
        (*std::min_element(m_Parameters.begin(), m_Parameters.end(), [&] (Parameters::const_reference a, Parameters::const_reference b) {
                        return a->getBegin() < b -> getBegin();
        }))->getBegin(),
        (*std::max_element(m_Parameters.begin(), m_Parameters.end(), [&] (Parameters::const_reference a, Parameters::const_reference b) {
                    return a->getEnd() < b -> getEnd();
        }))->getEnd()
    );

    return result;
}
