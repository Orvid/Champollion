#include "StreamWriter.hpp"

Decompiler::StreamWriter::StreamWriter(std::ostream &stream) :
    m_Stream(stream)
{
}

Decompiler::StreamWriter::~StreamWriter()
{
}

void Decompiler::StreamWriter::writeLine(const std::string &line)
{
    m_Stream << line << '\n';
}
