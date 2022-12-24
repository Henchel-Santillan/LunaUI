:: Builds the LunaUI using CMake and the Ninja generator.

:: Variables used for cmake options. These are configurable and are user-dependent.
set PREFIX_PATH=C:\Qt\6.4.1\mingw_64\lib\cmake
set DEBUG_TYPE_STRING=Debug
set CXX_COMPILER_PATH=C:\Qt\Tools\mingw1120_64\bin\g++.exe
set GENERATOR_STRING=Ninja
set MAKE_PROGRAM_PATH=C:\Qt\Tools\Ninja\ninja.exe
set SOURCE_DIR_PATH=C:\Users\hench\projects\LunaUI
set BUILD_DIR_PATH=C:\Users\hench\projects\LunaUI\build

cmake -DCMAKE_PREFIX_PATH=%PREFIX_PATH% -DCMAKE_BUILD_TYPE:STRING=%DEBUG_TYPE_STRING% -DCMAKE_CXX_COMPILER:FILEPATH=%CXX_COMPILER_PATH% -DCMAKE_GENERATOR:STRING=%GENERATOR_STRING% -DCMAKE_MAKE_PROGRAM:FILEPATH=%MAKE_PROGRAM_PATH% -S %SOURCE_DIR_PATH% -B %BUILD_DIR_PATH%
ninja
