cmake_minimum_required(VERSION 2.8.4 FATAL_ERROR)

set(name vorb)
# ----------------------------------------------------------------------
# VORB
# ----------------------------------------------------------------------
project(${name})
# include available make files
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_files")

include(vorb.cmake)

add_library(${name}
  ${LIBRARY_FILES}
  )
