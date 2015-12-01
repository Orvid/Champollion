#ifndef PSCCODER_HPP
#define PSCCODER_HPP

#include "Coder.hpp"


namespace Decompiler {
/**
 * @brief Write a PEX file as a PSC file.
 */
class PscCoder :
        public Coder
{
public:
    PscCoder(OutputWriter* writer);
    ~PscCoder();

    virtual void code(const Pex::Binary& pex);

    PscCoder& outputAsmComment(bool commentAsm);
protected:

    void writeHeader(const Pex::Binary& pex);
    void writeObject(const Pex::Object& object, const Pex::Binary& pex);
    void writeProperties(const Pex::Object& object, const Pex::Binary& pex);
    void writeVariables(const Pex::Object& object, const Pex::Binary& pex);
    void writeStates(const Pex::Object& object, const Pex::Binary& pex);
    void writeFunctions(int i, const Pex::State& state, const Pex::Object &object, const Pex::Binary& pex);
    void writeFunction(int i, const Pex::Function& function, const Pex::Object &object, const Pex::Binary& pex, const std::string& name = "");

    void writeUserFlag(std::ostream &stream, const Pex::UserFlagged& flagged, const Pex::Binary& pex);
    void writeDocString(int i, const Pex::DocumentedItem& item);

protected:
    bool m_CommentAsm;
};
}
#endif // PSCCODER_HPP
