cmake_minimum_required(VERSION 2.8.4 FATAL_ERROR)

#include(${CMAKE_CURRENT_LIST_DIR}/cmake_files/hunter/HunterGate.cmake)
#
#HunterGate(
#    URL "https://github.com/ruslo/hunter/archive/v0.19.208.tar.gz"
#    SHA1 "4128ac8c79c21b250bf825e8119ce0bff05e5132"
#)

set(name vorb)

enable_testing()

# ----------------------------------------------------------------------
# VORB
# ----------------------------------------------------------------------
#project(${name})
# include available make files
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_files")
#include(${name})

#setup_project()
#create_project_library()
#run_project_testing()

include(utils)
#include(deps)
include(components)

hunter_add_package(glm)
find_package(glm CONFIG REQUIRED)
hunter_add_package(glew)
find_package(glew CONFIG REQUIRED)
hunter_add_package(Boost COMPONENTS filesystem system)
find_package(Boost REQUIRED COMPONENTS filesystem system)
hunter_add_package(SDL2)
find_package(SDL2 CONFIG REQUIRED)
hunter_add_package(SDL_ttf)
find_package(SDL_ttf CONFIG REQUIRED)
hunter_add_package(enet)
find_package(enet CONFIG REQUIRED)
hunter_add_package(yaml-cpp)
find_package(yaml-cpp CONFIG REQUIRED)
hunter_add_package(OpenSSL)
find_package(OpenSSL REQUIRED)

verify_folder_structure()
initialize_for_platform()
setup_components()
#setup_dependencies()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/deps)

add_library(vorb
    ${PROJECT_LIBRARY_FILES}
    )

target_link_libraries(vorb
    SDL2::SDL2main
    SDL2::SDL2
    SDL_ttf::SDL_ttf
    glew::glew
    Boost::filesystem
    Boost::system
    glm
    enet::enet
    yaml-cpp::yaml-cpp
    OpenSSL::SSL
    OpenSSL::Crypto
)

add_subdirectory(tests)

add_definitions(-DVORB_IMPL_GRAPHICS_OPENGL -DVORB_IMPL_UI_SDL -DVORB_IMPL_FONT_SDL -DVORB_USING_SCRIPT)
