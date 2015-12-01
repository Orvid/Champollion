#pragma once

#include <string>

namespace Decompiler {

/**
 * @brief Interface for output writer.
 *
 * This interface insert an abstraction layer between the output file/buffer and the
 * code generation objects.
 *
 */
class OutputWriter
{
public:
    OutputWriter() = default;
    virtual ~OutputWriter() = default;

    virtual void writeLine(const std::string& line) = 0;
};
}
