#pragma once

#include <memory>
#include <sstream>

#include "OutputWriter.hpp"
#include "Pex/Binary.hpp"

namespace Decompiler {
/**
 * @brief Base class for program writer
 *
 * This base class handles the low level writing procedure
 * for class rewriting programs.
 */
class Coder
{
public:
    Coder(OutputWriter* writer);
    virtual ~Coder();

    virtual void code(const Pex::Binary& pex) = 0;

protected:
    void write(const std::string& line);
    void write(std::ostream& stream);


    std::ostringstream indent(int i);

    std::unique_ptr<OutputWriter> m_Writer;
};
}
