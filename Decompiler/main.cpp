#include <iostream>
#include <iomanip>
#include <fstream>

#include "Pex/Binary.hpp"
#include "Pex/FileReader.hpp"

#include "StreamWriter.hpp"
#include "AsmCoder.hpp"
#include "PscCoder.hpp"


int main(int argc, char* argv[])
{
    Pex::Binary pex;

    if (argc > 1)
    {
        std::cout << argv[1] << std::endl;
        std::string pexFile = std::string(argv[1]);
        std::string asmFile = pexFile.substr(0, pexFile.rfind('.')) + ".pasm";
        std::string pscFile = pexFile.substr(0, pexFile.rfind('.')) + ".psc";

        try
        {
            Pex::FileReader reader(argv[1]);
            reader.read(pex);

            std::ofstream asmStream(asmFile);
            Decompiler::AsmCoder asmCoder(new Decompiler::StreamWriter(asmStream));

            asmCoder.code(pex);

            std::ofstream pscStream(pscFile);
            Decompiler::PscCoder pscCoder(new Decompiler::StreamWriter(pscStream));

            pscCoder.code(pex);

        }
        catch(const std::exception& ex)
        {
            std::cerr << "Error : " << ex.what() << std::endl;
        }
    }

    return 0;
}

