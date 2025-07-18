cmake_minimum_required(VERSION 3.8)

project(Sinistar)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)

FetchContent_MakeAvailable(SFML)

set(ECS_BACKEND "Archetype" CACHE STRING "" FORCE)

FetchContent_Declare(WeaveECS
    GIT_REPOSITORY https://github.com/LouisCox14/Weave-ECS.git
    GIT_TAG main
    GIT_SHALLOW ON
)

FetchContent_MakeAvailable(WeaveECS)

add_executable(Sinistar src/main.cpp)

file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
target_sources(Sinistar PRIVATE ${MY_SOURCES})
target_include_directories(Sinistar PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")

target_link_libraries(Sinistar PRIVATE SFML::Graphics SFML::Audio WeaveECS)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Sinistar PROPERTY CXX_STANDARD 20)
endif()