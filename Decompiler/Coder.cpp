#include "Coder.hpp"
#include <cassert>


/**
 * @brief Constructor
 * Builds an coder associated with an output writer.
 *
 * @param writer Pointer to the writer managing the output. The ownership is transferred.
 */
Decompiler::Coder::Coder(Decompiler::OutputWriter *writer) :
    m_Writer(writer)
{
    assert(writer != nullptr);
}

/**
 * @brief Default destructor.
 */
Decompiler::Coder::~Coder()
{
}

/**
 * @brief Write a string to the output
 * @param line String to write.
 */
void Decompiler::Coder::write(const std::string &line)
{
    m_Writer->writeLine(line);
}

/**
 * @brief Write the content of a ostringstream.
 * The ostringstream is passed as an ostream. This is intended
 * to write output in the form
 *  write(indent(i) << "line data");
 * @param stream The stream as an ostream.
 */
void Decompiler::Coder::write(std::ostream &stream)
{
    auto& sstream = static_cast<std::ostringstream&>(stream);
    m_Writer->writeLine(sstream.str());
}

/**
 * @brief Creates an ostringstream and prepare it with identation.
 * @param i Indentation level to apply.
 * @return
 */
std::ostringstream Decompiler::Coder::indent(int i)
{
    std::ostringstream result;
    for(; i != 0; --i)
    {
        result << ' ' << ' ';
    }
    return result;
}
