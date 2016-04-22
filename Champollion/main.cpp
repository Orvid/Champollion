#include <iostream>

#include <boost/program_options.hpp>
namespace options = boost::program_options;

#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

#include <boost/format.hpp>

#include <chrono>
#include <future>

#include "Pex/Binary.hpp"
#include "Pex/FileReader.hpp"

#include "Decompiler/AsmCoder.hpp"
#include "Decompiler/PscCoder.hpp"

#include "Decompiler/StreamWriter.hpp"


struct Params
{
    bool outputAssembly;
    bool outputComment;
    bool parallel;

    fs::path assemblyDir;
    fs::path papyrusDir;

    std::vector<fs::path> inputs;
};

bool getProgramOptions(int argc, char* argv[], Params& params)
{
    params.outputAssembly = false;
    params.outputComment = false;
    params.parallel = false;
    params.assemblyDir = fs::path(".");
    params.papyrusDir = fs::path(".");


    options::options_description desc("Champollion PEX decompiler V1.0.6");
    desc.add_options()
            ("help,h", "Display the help message")
            ("asm,a", options::value<std::string>()->implicit_value(""), "Output assembly file")
            ("psc,p", options::value<std::string>(), "Name of the output dir for psc decompilation")
            ("comment,c", "Output assembly in comments of the decompiled psc file")
            ("threaded,t", "Run decompilation in parallel mode")
    ;
    options::options_description files;
    files.add_options()
            ("input", options::value< std::vector<std::string> >(), "Name of the input file");

    options::positional_options_description pdesc;
    pdesc.add("input", -1);

    options::variables_map args;

    try
    {
        options::store(options::basic_command_line_parser<char>(argc, argv).options(
                           options::options_description().add(desc).add(files)
                      ).positional(pdesc).run(), args);
        options::notify(args);
    }
    catch (const std::exception& ex)
    {
        std::cout << ex.what() << std::endl;
        std::cout << desc << std::endl;
        return false;
    }

    if (args.count("help"))
    {
        std::cout << desc;
        return false;
    }

    params.outputComment = (args.count("comment") != 0);
    params.parallel = (args.count("threaded") != 0);

    try
    {
        if (args.count("asm"))
        {
            params.outputAssembly = true;
            auto dir = args["asm"].as<std::string>();
            if (! dir.empty())
            {
                params.assemblyDir = fs::path(dir);
                if (!fs::exists(params.assemblyDir))
                {
                    fs::create_directories(params.assemblyDir);
                }
                else if (!fs::is_directory(params.assemblyDir))
                {
                    std::cout << params.assemblyDir << " is not a directory" << std::endl;
                    return false;
                }
            }
        }
        if (args.count("psc"))
        {
            auto dir = args["psc"].as<std::string>();

            params.papyrusDir = fs::path(dir);
            if (!fs::exists(params.papyrusDir))
            {
                fs::create_directories(params.papyrusDir);
            }
            else if (!fs::is_directory(params.papyrusDir))
            {
                std::cout << params.papyrusDir << " is not a directory" << std::endl;
                return false;
            }
        }
    }
    catch(const std::exception& ex)
    {
        std::cout << ex.what();
        return false;
    }

    if(args.count("input"))
    {
        for (auto in : args["input"].as<std::vector<std::string>>())
        {
            fs::path file(in);
            if (fs::exists(file))
            {
                params.inputs.push_back(file);
            }
            else
            {
                std::cout << file << " doesn't exists, skipping" << std::endl;
            }
        }
    }
    if (params.inputs.empty())
    {
        std::cout << "No input file given" << std::endl;
        return false;
    }
    return true;
}

typedef std::vector<std::string> ProcessResults;
ProcessResults processFile(fs::path file, Params& params)
{
    ProcessResults result;
    Pex::Binary pex;
    try
    {
        Pex::FileReader reader(file.string());
        reader.read(pex);
    }
    catch(std::exception& ex)
    {
       result.push_back(boost::str(boost::format("ERROR: %1% : %2%") % file.string() % ex.what()));
       return result;
    }
    if (params.outputAssembly)
    {
        fs::path asmFile = params.assemblyDir / file.filename().replace_extension(".pas");
        try
        {
            std::ofstream asmStream(asmFile.string());
            Decompiler::AsmCoder asmCoder(new Decompiler::StreamWriter(asmStream));

            asmCoder.code(pex);
            result.push_back(boost::str(boost::format("%1% dissassembled to %2%") % file.string() % asmFile.string()));
        }
        catch(std::exception& ex)
        {
            result.push_back(boost::str(boost::format("ERROR: %1% : %2%") % file.string() % ex.what()));
            fs::remove(asmFile);
        }
    }

    fs::path pscFile = params.papyrusDir / file.filename().replace_extension(".psc");
    try
    {
        std::ofstream pscStream(pscFile.string());
        Decompiler::PscCoder pscCoder(new Decompiler::StreamWriter(pscStream));

        pscCoder.outputAsmComment(params.outputComment).code(pex);
        result.push_back(boost::str(boost::format("%1% decompiled to %2%") % file.string() % pscFile.string()));
    }
    catch(std::exception& ex)
    {
        result.push_back(boost::str(boost::format("ERROR: %1% : %2%") % file.string() % ex.what()));
        fs::remove(pscFile);
    }
    return result;

}

int main(int argc, char* argv[])
{

    Params args;
    size_t countFiles = 0;
    if (getProgramOptions(argc, argv, args))
    {
        auto start = std::chrono::steady_clock::now();
        if(!args.parallel)
        {
            for (auto path : args.inputs)
            {
                if (fs::is_directory(path))
                {
                    fs::directory_iterator end;
                    fs::directory_iterator entry(path);
                    while(entry != end)
                    {
                        if (_stricmp(entry->path().extension().string().c_str(), ".pex") == 0)
                        {
                            for (auto line : processFile(entry->path(), args))
                            {
                                std::cout << line << '\n';
                            }
                            ++countFiles;
                        }
                        entry++;
                    }
                }
                else
                {
                    ++countFiles;
                    for (auto line : processFile(path, args))
                    {
                        std::cout << line << '\n';
                    }
                }
            }
        }
        else
        {
            std::vector<std::future<ProcessResults>> results;
            for (auto& path : args.inputs)
            {
                if (fs::is_directory(path))
                {
                    fs::directory_iterator end;
                    fs::directory_iterator entry(path);
                    while(entry != end)
                    {
                        if (_stricmp(entry->path().extension().string().c_str(), ".pex") == 0)
                        {

                            results.push_back(std::async(std::launch::async, processFile, fs::path(entry->path()), args));
                        }
                        entry++;
                    }
                }
                else
                {
                    results.push_back(std::async(std::launch::async, processFile, path, args));
                }
            }

            for (auto& result : results)
            {
                for(auto& line : result.get())
                {
                    std::cout << line << '\n';
                }
            }
            countFiles = results.size();

        }
        auto end = std::chrono::steady_clock::now();
        auto diff = end - start;

        std::cout << countFiles << " files processed in " << std::chrono::duration <double> (diff).count() << " s" << std::endl;
        return 0;
    }
    return 1;
}

