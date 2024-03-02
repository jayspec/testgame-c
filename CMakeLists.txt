cmake_minimum_required(VERSION 3.18)
set(LLVM_MOS_PLATFORM cx16)
find_package(llvm-mos-sdk REQUIRED)
project(testgame)
add_executable(
    testgame 
    src/main.c 
    src/globals.c
    src/ship_control.c
    src/sprite_load.c
    src/joystick.c
    src/shot.c)