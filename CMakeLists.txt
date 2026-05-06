cmake_minimum_required(VERSION 3.25)

project(AgenticWorkspaceOrchestrator
    VERSION 0.1.0
    LANGUAGES CXX
)

if(MSVC)
    set(CMAKE_CXX_STANDARD 23)
else()
    set(CMAKE_CXX_STANDARD 26)
endif()
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

qt_add_executable(AgenticWorkspaceOrchestrator
    src/main.cpp
    src/MainWindow.cpp
    src/MainWindow.h
)

target_link_libraries(AgenticWorkspaceOrchestrator
    PRIVATE
        Qt6::Widgets
)

if(MSVC)
    target_compile_options(AgenticWorkspaceOrchestrator PRIVATE /std:c++latest)
endif()

set_target_properties(AgenticWorkspaceOrchestrator PROPERTIES
    WIN32_EXECUTABLE TRUE
)
