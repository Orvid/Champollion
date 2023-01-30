# Champollion

Champollion is a decompiler for the Papyrus script language used in Skyrim and Fallout 4. It aims to produce a Papyrus Script file (.psc) from a .pex binary file. The decompiled script should recompile to a functionally equivalent PEX binary.

## Usage

Champollion is a CLI-only program.

### Parameters

`Champollion <files or directories> [-p <output directory>] [-a [<assembly directory>]] [-c] [-t]`

| Short                     | Long                         | Description                                                  |
| ------------------------- | ---------------------------- | ------------------------------------------------------------ |
| -p *output directory*     | --psc *output directory*     | Set the output directory, where Champollion will write the decompiled files |
| -a [*assembly directory*] | --asm [*assembly directory*] | Champollion will write an assembly version of the PEX file in the given directory, if one. The assembly file is an human readable version of the content of the PEX file |
| -c                        | --comment                    | The decompiled file will be annotated with the assembly instruction corresponding to the decompiled code lines. |
| -t                        | --threaded                   | Champollion will parallelize the decompilation. It is useful when decompiling a directory containing many PEX files. |
| -s                        | --recreate-subdirs           | Recreates directory structure for script in root of output directory (Fallout 4 only, default false) |
| -e                        | --header                     | Write header to decompiled psc file                          |
| -g                        | --trace                      | Trace the decompilation and output results to rebuild log    |
|                           | --no-dump-tree               | Do not dump tree for each node during decompilation tracing (requires --trace) |
|                           | --version                    | Output version number                                        |
| -h                        | --help                       | Print help message                                           |

## Build Dependencies

* Boost (installable through vcpkg)
* CMake
* A C++17 compiler (for Windows you need at least Visual Studio 2019)

## Copyright

Copyright (c) 2022 Nikita Lita

Copyright (c) 2015 Orvid King

Copyright (c) 2013 Paul-Henry Perrin

See LICENSE for the LGPL V3 license.
