# Sets the following variables

# LODE_PNG_INCLUDE_DIRS - include directories for LODE_PNG
# LODE_PNG_LIBRARIES - libraries to link against LODE_PNG
# LODE_PNG_FOUND - true if LODE_PNG has been found and can be used

SET(LODE_PNG_FOUND "NO")

# default search dirs
SET(_lode_png_HEADER_SEARCH_DIRS
    "${CMAKE_CURRENT_SOURCE_DIR}/deps/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/deps/lodepng"
    "/usr/include"
    "/usr/local/include"
    )

# check environment variable
SET(_lode_png_ENV_ROOT_DIR "$ENV{LODE_PNG_ROOT_DIR}")

# set LODE_PNG ROOT if needed
IF(NOT LODE_PNG_ROOT_DIR AND _lode_png_ENV_ROOT_DIR)
    SET(LODE_PNG_ROOT_DIR "${_lode_png_ENV_ROOT_DIR}")
ENDIF(NOT LODE_PNG_ROOT_DIR AND _lode_png_ENV_ROOT_DIR)

IF(LODE_PNG_ROOT_DIR)
    SET(_lode_png_HEADER_SEARCH_DIRS "${LODE_PNG_ROOT_DIR}"
                                "${LODE_PNG_ROOT_DIR}/include"
                                 ${_lode_png_HEADER_SEARCH_DIRS})
ENDIF(LODE_PNG_ROOT_DIR)

FIND_PATH(LODE_PNG_INCLUDE_DIR "lode_png/lode_png.hpp"
    PATHS ${_lode_png_HEADER_SEARCH_DIRS})

message(STATUS "lode_png.hpp: ${LODE_PNG_INCLUDE_DIR}")

IF (LODE_PNG_INCLUDE_PATH AND LODE_PNG_LIBRARY)
    SET(LODE_PNG_LIBRARIES ${LODE_PNG_LIBRARY})
    SET(LODE_PNG_FOUND "YES")
ENDIF (LODE_PNG_INCLUDE_PATH AND LODE_PNG_LIBRARY)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(LODE_PNG DEFAULT_MSG
    LODE_PNG_INCLUDE_DIR)

IF(NOT LODE_PNG_FOUND)
    # add external here
    set(LODE_PNG_INCLUDEDIR "${VENDOR_PREFIX}/src/lode_png" CACHE PATH "" FORCE)
    # set(LODE_PNG_INCLUDEDIR "${CMAKE_CURRENT_SOURCE_DIR}/deps/lode_png")
    list(APPEND vendor_args
        "-DLODE_PNG_INCLUDEDIR:PATH=${LODE_PNG_INCLUDEDIR}")
    INCLUDE(ExternalProject)
    ExternalProject_Add(lode_png
        PREFIX ${VENDOR_PREFIX}
        GIT_REPOSITORY https://github.com/RegrowthStudios/lodepng
        GIT_TAG d6dcafc0cd
        INSTALL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/deps/lodepng"
        # UPDATE_COMMAND ""
        # CONFIGURE_COMMAND ""
        # BUILD_IN_SOURCE 1
        # BUILD_COMMAND ""
        # INSTALL_COMMAND ""
        LOG_DOWNLODE ON
        LOG_INSTALL ON
    )
    ExternalProject_Get_Property(lode_png source_dir)
    set(LODE_PNG_INCLUDE_DIR ${source_dir}/lode_png)
ENDIF(NOT LODE_PNG_FOUND)

IF(LODE_PNG_FOUND)
    SET(LODE_PNG_INCLUDE_DIRS "${LODE_PNG_INCLUDE_DIR}")
    MESSAGE(STATUS "LODE_PNG_INCLUDE_DIR = ${LODE_PNG_INCLUDE_DIR}")
ENDIF(LODE_PNG_FOUND)
