include(cmake_files/utils.cmake)
include(cmake_files/deps.cmake)
include(cmake_files/components.cmake)

macro(setup_project)
    verify_folder_structure()
    initialize_for_platform()
    setup_components()
    setup_dependencies()

    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
    include_directories(SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/deps)
endmacro(setup_project)

macro(create_project_library)
    add_library(${name}
      ${PROJECT_LIBRARY_FILES}
      )
endmacro(create_project_library)
