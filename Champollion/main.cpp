#include <iostream>

#include <boost/program_options.hpp>
namespace options = boost::program_options;


#include <fmt/format.h>

#include <filesystem>
namespace fs = std::filesystem;

#include <chrono>
#include <future>
#include <ctime>

#include "Pex/Binary.hpp"
#include "Pex/FileReader.hpp"

#include "Decompiler/AsmCoder.hpp"
#include "Decompiler/PscCoder.hpp"

#include "Decompiler/StreamWriter.hpp"
#include "Decompiler/Version.hpp"
#include "glob.hpp"
#include "CaselessCompare.h"

struct Params
{
    bool outputAssembly;
    bool outputComment;
    bool writeHeader;
    bool parallel;
    bool traceDecompilation;
    bool dumpTree;
    bool recreateDirStructure;
    bool decompileDebugFuncs;
    bool recursive;
    bool verbose;
    bool printInfo;
    bool printCompileTime;
    bool debugLineComment;

    fs::path assemblyDir;
    fs::path papyrusDir;

    fs::path parentDir{};

    std::vector<fs::path> inputs;
};

enum OptionsResult{
    Invalid = -1,
    HelpOrVersion,
    Good
};

OptionsResult getProgramOptions(int argc, char* argv[], Params& params)
{
    params.outputAssembly = false;
    params.outputComment = false;
    params.writeHeader = false;
    params.parallel = false;
    params.traceDecompilation = false;
    params.dumpTree = true;
    params.recreateDirStructure = false;
    params.decompileDebugFuncs = false;
    params.verbose = false;
    params.printInfo = false;
    params.printCompileTime = false;
    params.debugLineComment = true;

    params.assemblyDir = fs::current_path();
    params.papyrusDir = fs::current_path();

    std::string version_string = "Champollion PEX decompiler " + std::string(CHAMPOLLION_VERSION_STRING);
    options::options_description desc(version_string);
    desc.add_options()
            ("help,h", "Display the help message")
            ("asm,a", options::value<std::string>()->implicit_value(""), "If defined, output assembly file(s) to this directory")
            ("psc,p", options::value<std::string>(), "Name of the output dir for psc decompilation")
            ("recursive,r", "Recursively scan directories for pex files")
            ("recreate-subdirs,s", "Recreates directory structure for script in root of output directory (Fallout 4 only, default false)")
            ("comment,c", "Output assembly in comments of the decompiled psc file")
            ("header,e", "Write header to decompiled psc file")
            ("threaded,t", "Run decompilation in parallel mode")
            ("trace,g", "Trace the decompilation and output results to rebuild log")
            ("no-dump-tree", "Do not dump tree for each node during decompilation tracing (requires --trace)")
            ("debug-funcs,d", "Decompile debug and compiler-generated functions (default false)")
            ("no-debug-line", "Do not comment with debug info line numbers on script lines (default false)")
            ("print-info,i", "Print header info from the specified PEX file(s) and exit")
            ("print-compile-time", "Print the compile time of the script in format of {filename}: {time_integer} and exit")
            ("verbose,v", "Verbose output")
            ("version,V", "Output version number")
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
        return Invalid;
    }

    if (args.count("help"))
    {
        std::cout << desc;
        return HelpOrVersion;
    }
    if (args.count("version"))
    {
        std::cout << version_string << std::endl;
        return HelpOrVersion;
    }

    params.outputComment = (args.count("comment") != 0);
    params.writeHeader = (args.count("header") != 0);
    params.parallel = (args.count("threaded") != 0);
    params.traceDecompilation = (args.count("trace") != 0);
    params.dumpTree = params.traceDecompilation && args.count("no-dump-tree") == 0;
    params.recursive = (args.count("recursive") != 0);
    params.recreateDirStructure = (args.count("recreate-subdirs") != 0);
    params.decompileDebugFuncs = (args.count("debug-funcs") != 0);
    params.printInfo = (args.count("print-info") != 0);
    params.printCompileTime = (args.count("print-compile-time") != 0);
    params.debugLineComment = !(args.count("no-debug-line") != 0);
    params.verbose = (args.count("verbose") != 0);
    if (!params.printInfo) {
      try {
        if (args.count("asm")) {
          params.outputAssembly = true;
          auto dir = args["asm"].as<std::string>();
          if (!dir.empty()) {
            params.assemblyDir = fs::path(dir);
            if (!fs::exists(params.assemblyDir)) {
              fs::create_directories(params.assemblyDir);
            } else if (!fs::is_directory(params.assemblyDir)) {
              std::cout << params.assemblyDir << " is not a directory" << std::endl;
              return Invalid;
            }
          }
        }
        if (args.count("psc")) {
          auto dir = args["psc"].as<std::string>();

          params.papyrusDir = fs::path(dir);
          if (!fs::exists(params.papyrusDir)) {
            fs::create_directories(params.papyrusDir);
          } else if (!fs::is_directory(params.papyrusDir)) {
            std::cout << params.papyrusDir << " is not a directory" << std::endl;
            return Invalid;
          }
        }
      }
      catch (const std::exception &ex) {
        std::cout << ex.what();
        return Invalid;
      }
    }

    if(args.count("input"))
    {
        auto input_args = args["input"].as<std::vector<std::string>>();
        auto globbed_files = glob::rglob(input_args);
        for (auto in : globbed_files)
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
        return Invalid;
    }
    return Good;
}

struct _ProcessResults{
    std::vector<std::string> output;
    bool isStarfield = false;
    bool failed = false;
};

typedef _ProcessResults ProcessResults;
ProcessResults processFile(fs::path file, Params params)
{
    ProcessResults result;
    Pex::Binary pex;
    try
    {
        Pex::FileReader reader(file.string());
        reader.read(pex);
        pex.sort();
    }
    catch(std::exception& ex)
    {
       result.output.push_back(fmt::format("ERROR: {} : {}", file.string(), ex.what()));
       result.failed = true;
       return result;
    }
    pex.getGameType() == Pex::Binary::StarfieldScript ? result.isStarfield = true : result.isStarfield = false;
    if (params.printInfo)
    {
        std::string gameType;
        switch(pex.getGameType())
        {
        case Pex::Binary::SkyrimScript:
            gameType = "Skyrim";
            break;
        case Pex::Binary::Fallout4Script:
            gameType = "Fallout 4";
            break;
        case Pex::Binary::StarfieldScript:
            gameType = "Starfield";
            break;
        default:
            gameType = "Unknown";
            break;
        }

        result.output.push_back(fmt::format("Script:             {}", file.string() ));
        // print out all the info contained in the header and exit
        result.output.push_back(fmt::format("  Game:             {}", gameType));
        auto header = pex.getHeader();
        result.output.push_back(fmt::format("  Game Version:     {}.{}", header.getMajorVersion(), header.getMinorVersion()));
        result.output.push_back(fmt::format("  GameID:           {}", header.getGameID()));
        auto time = header.getCompilationTime();
        std::string hrtime = ctime(&time);
        // trim trailing line break
        hrtime.erase(hrtime.find_last_not_of("\n") + 1);
        result.output.push_back(fmt::format("  Compilation Time: {} ({}) ", time, hrtime));
        result.output.push_back(fmt::format("  Source File:      {}", header.getSourceFileName()));
        result.output.push_back(fmt::format("  User Name:        {}", header.getUserName()));
        result.output.push_back(fmt::format("  Computer Name:    {}\n", header.getComputerName()));
        return result;
    }
    if (params.printCompileTime)
    {
        auto header = pex.getHeader();
        auto time = header.getCompilationTime();
        result.output.push_back(fmt::format("{}: {}", file.string(), time));
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
            result.output.push_back(fmt::format("{} dissassembled to {}", file.string(), asmFile.string()));
        }
        catch(std::exception& ex)
        {
            result.output.push_back(fmt::format("ERROR: {} : {}", file.string(), ex.what()));
            result.failed = true;
            fs::remove(asmFile);
        }
    }
    fs::path dir_structure;
    if (params.recreateDirStructure && (pex.getGameType() == Pex::Binary::Fallout4Script || pex.getGameType() == Pex::Binary::StarfieldScript) && pex.getObjects().size() > 0){
        std::string script_path = pex.getObjects()[0].getName().asString();
        std::replace(script_path.begin(), script_path.end(), ':', '/');
        dir_structure = fs::path(script_path).remove_filename();
    } else if (!params.parentDir.empty()) {
      dir_structure = fs::relative(file, params.parentDir).remove_filename();
    }
    fs::path basedir = !dir_structure.empty() ? (params.papyrusDir / dir_structure) : params.papyrusDir;
    if (!dir_structure.empty()){
        fs::create_directories(basedir);
    }
    fs::path fileName = file.filename().replace_extension(".psc");
    fs::path pscFile = basedir / fileName;
    try
    {   
        std::ofstream pscStream(pscFile);
        if (pscStream.fail()){
            throw std::runtime_error(fmt::format("Failed to open {} for writing", pscFile.string()));
        }
        Decompiler::PscCoder pscCoder(
                new Decompiler::StreamWriter(pscStream),
                params.outputComment,
                params.writeHeader,
                params.traceDecompilation,
                params.dumpTree,
                params.decompileDebugFuncs,
                params.debugLineComment,
                params.papyrusDir.string()); // using string instead of path here for C++14 compatability for staticlib targets

        pscCoder.code(pex);
        result.output.push_back(fmt::format("{} decompiled to {}", file.string(), pscFile.string()));
    }
    catch(std::exception& ex)
    {
        result.output.push_back(fmt::format("ERROR: {} : {}", file.string() , ex.what()));
        result.failed = true;
        fs::remove(pscFile);
    }
    return result;

}
size_t countFiles = 0;
size_t failedFiles = 0;
bool printStarfieldWarning = false;

void processResult(const ProcessResults &result, const Params& params)
{
    countFiles++;
    if (!printStarfieldWarning && result.isStarfield){
      printStarfieldWarning = true;
    }
    if (result.failed){
      ++failedFiles;
      for (auto line : result.output)
      {
        std::cerr << line << '\n';
      }
    } else if (params.verbose || params.printInfo || params.printCompileTime) { // only output each individual successful result if `verbose` is on
      for (auto line : result.output)
      {
        std::cout << line << '\n';
      }
    }
}


int main(int argc, char* argv[])
{

    Params args;
    auto result = getProgramOptions(argc, argv, args);
    if (result == Good)
    {
        auto start = std::chrono::steady_clock::now();
        // ignore parallel if we are printing info
        if(!args.parallel || args.printInfo || args.printCompileTime)
        {
            for (auto path : args.inputs)
            {
                if (args.recursive && fs::is_directory(path)){
                    args.parentDir = path;
                    // recursively get all files in the directory
                    for (auto& entry : fs::recursive_directory_iterator(path)){
                        if (fs::is_regular_file(entry) && caselessCompare(entry.path().extension().string().c_str(), ".pex") == 0){
                            processResult(processFile(entry, args), args);
                        }
                    }
                } else if (fs::is_directory(path)){
                    args.parentDir = fs::path();
                    fs::directory_iterator end;
                    fs::directory_iterator entry(path);
                    while(entry != end)
                    {
                        if (caselessCompare(entry->path().extension().string().c_str(), ".pex") == 0)
                        {
                            processResult(processFile(path, args), args);
                        }
                        entry++;
                    }
                }
                else
                {
                  args.parentDir = fs::path();
                  processResult(processFile(path, args), args);
                }
            }
        }
        else
        {
            std::vector<std::future<ProcessResults>> results;
            for (auto& path : args.inputs)
            {

                if (args.recursive && fs::is_directory(path)){
                  args.parentDir = path;
                  // recursively get all files in the directory
                  for (auto& entry : fs::recursive_directory_iterator(path)){
                    if (fs::is_regular_file(entry) && caselessCompare(entry.path().extension().string().c_str(), ".pex") == 0){
                        results.push_back(std::move(std::async(std::launch::async, processFile, fs::path(entry.path()), args)));
                    }
                  }
                }
                else if (fs::is_directory(path))
                {
                    args.parentDir = fs::path();
                    fs::directory_iterator end;
                    fs::directory_iterator entry(path);
                    while(entry != end)
                    {
                        if (caselessCompare(entry->path().extension().string().c_str(), ".pex") == 0)
                        {

                            results.push_back(std::move(std::async(std::launch::async, processFile, fs::path(entry->path()), args)));
                        }
                        entry++;
                    }
                }
                else
                {
                    args.parentDir = fs::path();
                    results.push_back(std::move(std::async(std::launch::async, processFile, path, args)));
                }
            }

            for (auto& result : results)
            {
                auto pResult = result.get();
                processResult(pResult, args);
            }
            countFiles = results.size();

        }
        auto end = std::chrono::steady_clock::now();
        auto diff = end - start;

        std::cout << countFiles << " files processed in " << std::chrono::duration <double> (diff).count() << " s" << std::endl;
        if (failedFiles > 0){
            std::cout << failedFiles << " files failed to decompile." << std::endl;
        }
        if (countFiles > 0 && countFiles != failedFiles){
          if (args.outputAssembly){
            std::cout << "Disassembled scripts written to " << args.assemblyDir.string() << std::endl;
          }
          std::cout << "Decompiled scripts written to " << args.papyrusDir.string() << std::endl << std::endl;
        }

        if (printStarfieldWarning){
          // TODO: Remove this warning when the CK comes out
          std::cout << "********************* STARFIELD PRELIMINARY SYNTAX WARNING *********************" << std::endl;
          std::cout << "The syntax for new features in Starfield (Guard, TryGuard, GetMatchingStructs) is not yet known." << std::endl;
          std::cout << "Decompiled Starfield scripts use guessed-at syntax for these features." << std::endl;
          std::cout << "This syntax should be considered as experimental, unstable, and subject to change." << std::endl << std::endl;
          std::cout << "The proper syntax will only be known when the Creation Kit comes out in early 2024." << std::endl;
          std::cout << "If you are using decompiled scripts as the basis for mods, please be aware of this," << std::endl;
          std::cout << "and be prepared to update your scripts when the final syntax is known." << std::endl << std::endl;
          std::cout << "The lines in the decompiled scripts which contain this guessed-at syntax are" << std::endl;
          std::cout << "marked with a comment beginning with '" << Decompiler::WARNING_COMMENT_PREFIX << "'." << std::endl;
          std::cout << "********************* STARFIELD PRELIMINARY SYNTAX WARNING *********************" << std::endl;
        }
        return 0;
    }
    if (result == HelpOrVersion){
        return 0;
    }
    return 1;
}

