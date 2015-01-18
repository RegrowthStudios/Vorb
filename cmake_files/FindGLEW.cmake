# Sets the following variables

# GLEW_INCLUDE_DIRS - include directories for GLEW
# GLEW_LIBRARIES - libraries to link against GLEW
# GLEW_FOUND - true if GLEW has been found and can be used

SET(GLEW_FOUND "NO")

# default search dirs
SET(_glew_HEADER_SEARCH_DIRS
    "${CMAKE_CURRENT_SOURCE_DIR}/deps/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/deps/glew"
    "/usr/include"
    "/usr/local/include"
    )

# check environment variable
SET(_glew_ENV_ROOT_DIR "$ENV{GLEW_ROOT_DIR}")

# set GLEW ROOT if needed
IF(NOT GLEW_ROOT_DIR AND _glew_ENV_ROOT_DIR)
    SET(GLEW_ROOT_DIR "${_glew_ENV_ROOT_DIR}")
ENDIF(NOT GLEW_ROOT_DIR AND _glew_ENV_ROOT_DIR)

IF(GLEW_ROOT_DIR)
    SET(_glew_HEADER_SEARCH_DIRS "${GLEW_ROOT_DIR}"
                                "${GLEW_ROOT_DIR}/include"
                                 ${_glew_HEADER_SEARCH_DIRS})
ENDIF(GLEW_ROOT_DIR)

FIND_PATH(GLEW_INCLUDE_DIR "glew/glew.hpp"
    PATHS ${_glew_HEADER_SEARCH_DIRS})

message(STATUS "glew.hpp: ${GLEW_INCLUDE_DIR}")

IF (GLEW_INCLUDE_PATH AND GLEW_LIBRARY)
    SET(GLEW_LIBRARIES ${GLEW_LIBRARY})
    SET(GLEW_FOUND "YES")
ENDIF (GLEW_INCLUDE_PATH AND GLEW_LIBRARY)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GLEW DEFAULT_MSG
    GLEW_INCLUDE_DIR)

IF(NOT GLEW_FOUND)
    # add external here
    set(GLEW_INCLUDEDIR "${VENDOR_PREFIX}/src/glew" CACHE PATH "" FORCE)
    # set(GLEW_INCLUDEDIR "${CMAKE_CURRENT_SOURCE_DIR}/deps/glew")
    list(APPEND vendor_args
        "-DGLEW_INCLUDEDIR:PATH=${GLEW_INCLUDEDIR}")
    INCLUDE(ExternalProject)
    ExternalProject_Add(glew
        PREFIX ${VENDOR_PREFIX}
        GIT_REPOSITORY https://github.com/RegrowthStudios/glew
        GIT_TAG 002cb71b24
        INSTALL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/deps/glew"
        # UPDATE_COMMAND ""
        # CONFIGURE_COMMAND ""
        # BUILD_IN_SOURCE 1
        # BUILD_COMMAND ""
        # INSTALL_COMMAND ""
        LOG_DOWNLOAD ON
        LOG_INSTALL ON
    )
    ExternalProject_Get_Property(glew source_dir)
    set(GLEW_INCLUDE_DIR ${source_dir}/glew)
ENDIF(NOT GLEW_FOUND)

IF(GLEW_FOUND)
    SET(GLEW_INCLUDE_DIRS "${GLEW_INCLUDE_DIR}")
    MESSAGE(STATUS "GLEW_INCLUDE_DIR = ${GLEW_INCLUDE_DIR}")
ENDIF(GLEW_FOUND)
