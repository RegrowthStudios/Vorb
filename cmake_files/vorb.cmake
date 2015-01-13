include(cmake_files/utils.cmake)
include(cmake_files/deps.cmake)
include(cmake_files/components.cmake)

verify_folder_structure()
initialize_for_platform()
setup_components()
setup_dependencies()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

