cmake_minimum_required(VERSION 3.6.2)
project(ClockPlus)

#set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM3 REQUIRED gtkmm-3.0)
include_directories(SYSTEM ${GTKMM3_INCLUDE_DIRS})
link_directories(${GTKMM3_LIBRARY_DIRS})
add_definitions(${GTKMM3_CFLAGS_OTHER})
include_directories(AFTER SYSTEM external)

set(SOURCE_FILES src/main.cpp src/Application.cpp src/Application.h src/Common.h src/MainWindow.cpp src/MainWindow.h src/Background/Backgrounds.cpp src/Background/Backgrounds.h src/Background/Period.cpp src/Background/Period.h src/Background/Background.cpp src/Background/Background.h src/Background/Filter.cpp src/Background/Filter.h src/Common.cpp src/Background/Color.cpp src/Background/Color.h src/Background/Palette.cpp src/Background/Palette.h src/Background/PaletteEntry.cpp src/Background/PaletteEntry.h src/BackgroundManager.cpp src/BackgroundManager.h src/Background/JsonLoader.cpp src/Background/JsonLoader.h src/Background/Date.cpp src/Background/Date.h src/DateTimeProvider.cpp src/DateTimeProvider.h src/Background/CssGenerator.cpp src/Background/CssGenerator.h src/OutlinedLabel.cpp src/OutlinedLabel.h src/ClockContainer.cpp src/ClockContainer.h)
add_executable(ClockPlus ${SOURCE_FILES})
target_link_libraries(ClockPlus ${GTKMM3_LIBRARIES})
target_compile_options(ClockPlus PUBLIC -std=c++1z)
target_compile_options(ClockPlus PUBLIC -Weverything -Werror -Wno-c++98-compat
        -Wno-c++98-compat-pedantic -Wno-newline-eof -Wno-padded
        -Wno-exit-time-destructors)