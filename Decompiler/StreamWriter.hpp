#ifndef STREAMWRITER_HPP
#define STREAMWRITER_HPP

#include "Decompiler_Global.hpp"

#include <iostream>

#include "OutputWriter.hpp"

namespace Decompiler {
class DECOMPILER_API StreamWriter :
        public OutputWriter
{
public:
    StreamWriter(std::ostream& stream);
    virtual ~StreamWriter();

    virtual void writeLine(const std::string& line);

protected:
    std::ostream& m_Stream;
};
}

#endif // STREAMWRITER_HPP
