#pragma once

#include <iostream>

#include "OutputWriter.hpp"

namespace Decompiler {

class StreamWriter : public OutputWriter
{
public:
    StreamWriter(std::ostream& stream) : m_Stream(stream) { }
    virtual ~StreamWriter() = default;

    virtual void writeLine(const std::string& line)
    {
        m_Stream << line << '\n';
    }

protected:
    std::ostream& m_Stream;
};

}
