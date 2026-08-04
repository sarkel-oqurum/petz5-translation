cmake_minimum_required(VERSION 3.20)

project(PetzChuvash)

set(CMAKE_CXX_STANDARD 17)

#
# Common
#

add_library(common
    src/common/path.cpp
)

target_include_directories(common PUBLIC
    src/common
)

#
# DLL
#

add_library(Chuvash SHARED
    src/dll/dll.cpp
    src/dll/hooks.cpp
    src/dll/CreateFontA.cpp
    src/dll/CreateFontIndirectA.cpp
    src/dll/DoWMDrawItemGotoMenu.cpp
)

target_include_directories(Chuvash PRIVATE
    dll
    petz
)

target_link_libraries(Chuvash
    common
    user32
    gdi32
    kernel32
)

#
# Launcher
#

add_executable(Petz_cv
    src/launcher/main.cpp
    src/launcher/injector.cpp
    src/launcher/launcher.cpp
    src/launcher/launcher.rc
)

target_include_directories(Petz_cv PRIVATE
    launcher
    petz
)

target_link_libraries(Petz_cv
    common
    user32
    kernel32
)