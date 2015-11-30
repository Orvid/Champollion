#ifndef CALLPARAMETERSMIXIN_HPP
#define CALLPARAMETERSMIXIN_HPP

#include "Base.hpp"
#include <vector>
#include <string>

namespace Node {
class CallParametersMixin
{
public:
    typedef std::vector<BasePtr> Parameters;

public:
    CallParametersMixin();
    ~CallParametersMixin();

    const Parameters&  getParameters() const;
    Parameters&        getParameters();

    CallParametersMixin&     operator << (BasePtr param);

protected:
    Parameters m_Parameters;
    bool m_DirtyBounds;

    std::pair<size_t, size_t> getBounds() const;
};
}

#endif // CALLPARAMETERSMIXIN_HPP
