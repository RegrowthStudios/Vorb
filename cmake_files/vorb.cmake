include(utils)
include(deps)
include(components)

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

macro(run_project_testing)
   add_subdirectory(tests)
endmacro(run_project_testing)