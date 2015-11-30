#ifndef OUTPUTWRITER_HPP
#define OUTPUTWRITER_HPP

#include "Decompiler_Global.hpp"
#include <string>

namespace Decompiler {

/**
 * @brief Interface for output writer.
 *
 * This interface insert an abstraction layer between the output file/buffer and the
 * code generation objects.
 *
 */
class DECOMPILER_API OutputWriter
{
public:
    OutputWriter();
    virtual ~OutputWriter();

    virtual void writeLine(const std::string& line) = 0;
};
}

#endif // OUTPUTWRITER_HPP
