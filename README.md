* Testgame

To configure:
cmake --no-warn-unused-cli -DCMAKE_C_COMPILER="C:/llvm-mos/bin/mos-cx16-clang.bat" -DCMAKE_C_COMPILER_FORCED=TRUE -DCMAKE_PREFIX_PATH=C:\llvm-mos -DCMAKE_BUILD_TYPE:STRING=MinSizeRel -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -S ./ -B./build -G Ninja

Make sure you select the CX-16 kit for CMake tools extension.
