# Setup VORB Dependencies
macro(setup_dependencies)
    # GLM
    find_package(GLM)
    if (GLM_FOUND)
      add_definitions(-DTYPES_GLM)
      include_directories(SYSTEM ${GLM_INCLUDE_DIRS})
      link_directories(${GLM_LIBRARY_DIRS})
      add_definitions(${GLM_DEFINITIONS})
    else()
      remove_definitions(-DTYPES_GLM)
    endif (GLM_FOUND)

endmacro(setup_dependencies)