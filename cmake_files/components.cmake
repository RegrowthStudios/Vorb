macro(setup_components)
    # Setup defines for all other vorb components
    get_components_to_build()

    # Core and root source always will be built
    setup_root()
    setup_core()
    SET(PROJECT_LIBRARY_FILES ${VORB_ROOT_FILES} ${VORB_CORE_FILES})

    # Setup Other Components
    IF (VORB_BUILD_GRAPHICS)
        setup_graphics()
        LIST(APPEND PROJECT_LIBRARY_FILES ${VORB_GRAPHICS_FILES})
    ENDIF (VORB_BUILD_GRAPHICS)
    IF (VORB_BUILD_IO)
        setup_io()
        LIST(APPEND PROJECT_LIBRARY_FILES ${VORB_IO_FILES})
    ENDIF (VORB_BUILD_IO)
    IF (VORB_BUILD_PLATFORM)
        setup_platform()
        LIST(APPEND PROJECT_LIBRARY_FILES ${VORB_PLATFORM_FILES})
    ENDIF (VORB_BUILD_PLATFORM)
    IF (VORB_BUILD_SOUND)
        setup_sound()
        LIST(APPEND PROJECT_LIBRARY_FILES ${VORB_SOUND_FILES})
    ENDIF (VORB_BUILD_SOUND)
    IF (VORB_BUILD_UI)
        setup_ui()
        LIST(APPEND PROJECT_LIBRARY_FILES ${VORB_UI_FILES})
    ENDIF (VORB_BUILD_UI)
    IF (VORB_BUILD_VOXELS)
        setup_voxels()
        LIST(APPEND PROJECT_LIBRARY_FILES ${VORB_VOXELS_FILES})
    ENDIF (VORB_BUILD_VOXELS)
endmacro(setup_components)

macro(setup_core)
    file(GLOB core_source ${CMAKE_CURRENT_LIST_DIR}/src/core/*.cpp)
    file(GLOB core_inline ${CMAKE_CURRENT_LIST_DIR}/src/core/*.inl)
    file(GLOB core_header ${CMAKE_CURRENT_LIST_DIR}/src/core/*.hpp)
    set(VORB_CORE_FILES ${core_source} ${core_inline} ${core_header})
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/core)
endmacro(setup_core)

macro(setup_graphics)
    file(GLOB graphics_source ${CMAKE_CURRENT_LIST_DIR}/src/graphics/*.cpp)
    file(GLOB graphics_inline ${CMAKE_CURRENT_LIST_DIR}/src/graphics/*.inl)
    file(GLOB graphics_header ${CMAKE_CURRENT_LIST_DIR}/src/graphics/*.hpp)
    set(VORB_GRAPHICS_FILES ${graphics_source} ${graphics_inline} ${graphics_header})
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/graphics)
endmacro(setup_graphics)

macro(setup_io)
    file(GLOB io_source ${CMAKE_CURRENT_LIST_DIR}/src/io/*.cpp)
    file(GLOB io_inline ${CMAKE_CURRENT_LIST_DIR}/src/io/*.inl)
    file(GLOB io_header ${CMAKE_CURRENT_LIST_DIR}/src/io/*.hpp)
    set(VORB_IO_FILES ${io_source} ${io_inline} ${io_header})
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/io)
endmacro(setup_io)

macro(setup_platform)
    file(GLOB platform_source ${CMAKE_CURRENT_LIST_DIR}/src/platform/*.cpp)
    file(GLOB platform_inline ${CMAKE_CURRENT_LIST_DIR}/src/platform/*.inl)
    file(GLOB platform_header ${CMAKE_CURRENT_LIST_DIR}/src/platform/*.hpp)
    set(VORB_PLATFORM_FILES ${platform_source} ${platform_inline} ${platform_header})
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/platform)
endmacro(setup_platform)

macro(setup_sound)
    file(GLOB sound_source ${CMAKE_CURRENT_LIST_DIR}/src/sound/*.cpp)
    file(GLOB sound_inline ${CMAKE_CURRENT_LIST_DIR}/src/sound/*.inl)
    file(GLOB sound_header ${CMAKE_CURRENT_LIST_DIR}/src/sound/*.hpp)
    set(VORB_SOUND_FILES ${sound_source} ${sound_inline} ${sound_header})
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/sound)
endmacro(setup_sound)

macro(setup_ui)
    file(GLOB ui_source ${CMAKE_CURRENT_LIST_DIR}/src/ui/*.cpp)
    file(GLOB ui_inline ${CMAKE_CURRENT_LIST_DIR}/src/ui/*.inl)
    file(GLOB ui_header ${CMAKE_CURRENT_LIST_DIR}/src/ui/*.hpp)
    set(VORB_UI_FILES ${ui_source} ${ui_inline} ${ui_header})
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/ui)
endmacro(setup_ui)

macro(setup_voxels)
    file(GLOB voxels_source ${CMAKE_CURRENT_LIST_DIR}/src/voxels/*.cpp)
    file(GLOB voxels_inline ${CMAKE_CURRENT_LIST_DIR}/src/voxels/*.inl)
    file(GLOB voxels_header ${CMAKE_CURRENT_LIST_DIR}/src/voxels/*.hpp)
    set(VORB_VOXELS_FILES ${voxels_source} ${voxels_inline} ${voxels_header})
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/voxels)
endmacro(setup_voxels)

macro(setup_root)
    # Setup root VORB files
    file(GLOB root_source ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
    file(GLOB root_inline ${CMAKE_CURRENT_LIST_DIR}/src/*.inl)
    file(GLOB root_header ${CMAKE_CURRENT_LIST_DIR}/src/*.hpp)
    set(VORB_ROOT_FILES ${ui_source} ${ui_inline} ${ui_header})
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
endmacro(setup_root)

macro(get_components_to_build)

    SET(_vorb_env_BUILD_ALL_COMPONENTS "$ENV{VORB_BUILD_ALL_COMPONENTS}")
    SET(_vorb_env_BUILD_GRAPHICS "$ENV{VORB_BUILD_GRAPHICS}")
    SET(_vorb_env_BUILD_IO "$ENV{VORB_BUILD_IO}")
    SET(_vorb_env_BUILD_PLATFORM "$ENV{VORB_BUILD_PLATFORM}")
    SET(_vorb_env_BUILD_SOUND "$ENV{VORB_BUILD_SOUND}")
    SET(_vorb_env_BUILD_UI "$ENV{VORB_BUILD_UI}")
    SET(_vorb_env_BUILD_VOXELS "$ENV{VORB_BUILD_VOXELS}")

    IF(_vorb_env_BUILD_ALL_COMPONENTS)
        SET(VORB_BUILD_GRAPHICS True)
        SET(VORB_BUILD_IO True)
        SET(VORB_BUILD_PLATFORM True)
        SET(VORB_BUILD_SOUND True)
        SET(VORB_BUILD_UI True)
        SET(VORB_BUILD_VOXELS True)
    ENDIF(_vorb_env_BUILD_ALL_COMPONENTS)

    IF(NOT ${VORB_BUILD_GRAPHICS} OR ${_vorb_env_BUILD_GRAPHICS})
        SET(VORB_BUILD_GRAPHICS True)
    ENDIF(NOT ${VORB_BUILD_GRAPHICS} OR ${_vorb_env_BUILD_GRAPHICS})

    IF(NOT ${VORB_BUILD_IO} OR ${_vorb_env_BUILD_IO})
        SET(VORB_BUILD_IO True)
    ENDIF(NOT ${VORB_BUILD_IO} OR ${_vorb_env_BUILD_IO})

    IF(NOT ${VORB_BUILD_PLATFORM} and ${_vorb_env_BUILD_PLATFORM})
        SET(VORB_BUILD_PLATFORM True)
    ENDIF(NOT ${VORB_BUILD_PLATFORM} and ${_vorb_env_BUILD_PLATFORM})

    IF(NOT ${VORB_BUILD_SOUND} and ${_vorb_env_BUILD_SOUND})
        SET(VORB_BUILD_SOUND True)
    ENDIF(NOT ${VORB_BUILD_SOUND} and ${_vorb_env_BUILD_SOUND})

    IF(NOT ${VORB_BUILD_UI} and ${_vorb_env_BUILD_UI})
        SET(VORB_BUILD_UI True)
    ENDIF(NOT ${VORB_BUILD_UI} and ${_vorb_env_BUILD_UI})

    IF(NOT ${VORB_BUILD_VOXELS} and ${_vorb_env_BUILD_VOXELS})
        SET(VORB_BUILD_VOXELS True)
    ENDIF(NOT ${VORB_BUILD_VOXELS} and ${_vorb_env_BUILD_VOXELS})
endmacro(get_components_to_build)