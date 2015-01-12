include(utils.cmake)
include(deps.cmake)
include(components.cmake)

verify_folder_structure()
initialize_for_platform()
setup_components()
setup_dependencies()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

