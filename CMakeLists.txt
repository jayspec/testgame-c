cmake_minimum_required(VERSION 3.18)
set(LLVM_MOS_PLATFORM cx16)
set(CMAKE_C_FLAGS "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON")
find_package(llvm-mos-sdk REQUIRED)
project(testgame)
add_executable(
    testgame 
    src/main.c 
    src/ship.c
    src/sprite_load.c
    src/joystick.c
    src/shot.c
    src/sprite_update.c)