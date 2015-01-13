cmake_minimum_required(VERSION 2.8.4 FATAL_ERROR)

set(name vorb)
# ----------------------------------------------------------------------
# VORB
# ----------------------------------------------------------------------
project(${name})
# include available make files
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_files")
include(cmake_files/vorb.cmake)

add_library(${name}
  ${VORB_LIBRARY_FILES}
  )
