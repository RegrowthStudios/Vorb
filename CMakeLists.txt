cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)
cmake_policy(SET CMP0054 NEW)

set(HUNTER_STATUS_DEBUG ON)
#setup cache server and suppot upload
set(
    HUNTER_CACHE_SERVERS
    "https://github.com/huntercache/SoA"
    CACHE
    STRING
    "Default cache server"
)

string(COMPARE EQUAL "$ENV{TRAVIS}" "true" is_travis)
string(COMPARE EQUAL "$ENV{APPVEYOR}" "True" is_appveyor)
string(COMPARE EQUAL "$ENV{GITHUB_USER_PASSWORD}" "" password_is_empty)

if((is_travis OR is_appveyor) AND NOT password_is_empty)
  option(HUNTER_RUN_UPLOAD "Upload cache binaries" ON)
endif()
message(STATUS "Hunter upload: ${HUNTER_RUN_UPLOAD}")

set(
    HUNTER_PASSWORDS_PATH
    "${CMAKE_CURRENT_LIST_DIR}/cmake/hunter/passwords.cmake"
    CACHE
    FILEPATH
    "Hunter passwords"
)

include(${CMAKE_CURRENT_LIST_DIR}/cmake/hunter/HunterGate.cmake)

HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.23.16.tar.gz"
    SHA1 "dbe5b1c966414c4a055983c11d5251a2d8f2f16d"
)

set(name vorb)
enable_testing()

# ----------------------------------------------------------------------
# VORB
# ----------------------------------------------------------------------
project(${name})
# include available make files
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_files")
#include(${name})

option(VORB_USE_SCRIPTING "Support for Lua scripting" ON)

# TODO(Matthew): Support MSVC with these?
option(USING_GDB "Are we using gdb to debug?" ON)
option(EXTRA_DEBUG "Should we add extra debug symbols?" ON)
option(OPTIMISE_ON_DEBUG "Should we optimise a debug target?" ON)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
    "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
    if (${USING_GDB})
        if (${EXTRA_DEBUG})
            set(CMAKE_CXX_FLAGS_DEBUG "-ggdb3")
        else()
            set(CMAKE_CXX_FLAGS_DEBUG "-ggdb")
        endif()
    else()
        if (${EXTRA_DEBUG})
            set(CMAKE_CXX_FLAGS_DEBUG "-g3")
        else()
            set(CMAKE_CXX_FLAGS_DEBUG "-g")
        endif()
    endif ()
    if (${OPTIMISE_ON_DEBUG})
        # This is supported by both Clang and GCC as long as we are up-to-date.
        # Should be fine going back as far as Ubuntu 17.10, and deffo fine on Arch.
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Og")
    endif()
endif()

#setup_project()
#create_project_library()
#run_project_testing()

include(utils)
#include(deps)
include(components)

if(VORB_USE_SCRIPTING)
    hunter_add_package(Lua)
    find_package(Lua CONFIG REQUIRED)
endif()

hunter_add_package(glm)
find_package(glm CONFIG REQUIRED)
hunter_add_package(glew)
find_package(glew CONFIG REQUIRED)
hunter_add_package(Boost COMPONENTS filesystem system)
find_package(Boost REQUIRED COMPONENTS filesystem system)
hunter_add_package(SDL2)
find_package(SDL2 CONFIG REQUIRED)
hunter_add_package(SDL_ttf)
find_package(SDL_ttf CONFIG REQUIRED)
hunter_add_package(enet)
find_package(enet CONFIG REQUIRED)
hunter_add_package(yaml-cpp)
find_package(yaml-cpp CONFIG REQUIRED)
#hunter_add_package(OpenSSL)
#find_package(OpenSSL REQUIRED)
hunter_add_package(PNG)
find_package(PNG CONFIG REQUIRED)

verify_folder_structure()
initialize_for_platform()
#setup_components()
#setup_dependencies()

#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/Vorb)
include_directories(SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/deps)

set(vorb_ecs
    include/Vorb/ecs/BitTable.hpp
    include/Vorb/ecs/ComponentTable.hpp
    include/Vorb/ecs/ComponentTableBase.h
    include/Vorb/ecs/ECS.h
    include/Vorb/ecs/Entity.h
    include/Vorb/ecs/MultiComponentTracker.hpp
    include/Vorb/ecs/MultipleComponentSet.h
#source
    src/ecs/ComponentTableBase.cpp
    src/ecs/ECS.cpp
    src/ecs/MultipleComponentSet.cpp
)

set(vorb_graphics
    include/Vorb/graphics/AnimationData.h
    include/Vorb/graphics/ConnectedTextures.h
    include/Vorb/graphics/DeferredShaders.h
    include/Vorb/graphics/DepthState.h
    include/Vorb/graphics/Frustum.h
    include/Vorb/graphics/FullQuadVBO.h
    include/Vorb/graphics/GBuffer.h
    include/Vorb/graphics/GLEnums.h
    include/Vorb/graphics/GLProgram.h
    include/Vorb/graphics/GLRenderTarget.h
    include/Vorb/graphics/GLStates.h
    include/Vorb/graphics/GpuMemory.h
    include/Vorb/graphics/GraphicsDevice.h
    include/Vorb/graphics/gtypes.h
    include/Vorb/graphics/ImageIO.h
    include/Vorb/graphics/ImageIOConv.inl
    include/Vorb/graphics/ImageIOConvF.inl
    include/Vorb/graphics/MeshData.h
    include/Vorb/graphics/ModelIO.h
    include/Vorb/graphics/RasterizerState.h
    include/Vorb/graphics/RTSwapChain.hpp
    include/Vorb/graphics/SamplerState.h
    include/Vorb/graphics/ShaderInterface.h
    include/Vorb/graphics/ShaderManager.h
    include/Vorb/graphics/ShaderParser.h
    include/Vorb/graphics/SpriteBatch.h
    include/Vorb/graphics/SpriteBatchShader.inl
    include/Vorb/graphics/SpriteFont.h
    include/Vorb/graphics/Texture.h
    include/Vorb/graphics/TextureCache.h
#source
    src/graphics/ConnectedTextures.cpp
    src/graphics/DeferredShaders.cpp
    src/graphics/DepthState.cpp
    src/graphics/Frustum.cpp
    src/graphics/FullQuadVBO.cpp
    src/graphics/GBuffer.cpp
    src/graphics/GLProgram.cpp
    src/graphics/GLRenderTarget.cpp
    src/graphics/GpuMemory.cpp
    src/graphics/GraphicsDevice.cpp
    src/graphics/ImageIO.cpp
    src/graphics/ModelIO.cpp
    src/graphics/RasterizerState.cpp
    src/graphics/SamplerState.cpp
    src/graphics/ShaderInterface.cpp
    src/graphics/ShaderManager.cpp
    src/graphics/ShaderParser.cpp
    src/graphics/SpriteBatch.cpp
    src/graphics/SpriteFont.cpp
    src/graphics/Texture.cpp
    src/graphics/TextureCache.cpp
)

if(VORB_USE_SCRIPTING)
set(vorb_graphics ${vorb_graphics}
    include/Vorb/graphics/script/GLEnumsScriptFuncs.h
    include/Vorb/graphics/script/SamplerStateScriptFuncs.h
    include/Vorb/graphics/script/SpriteBatchScriptFuncs.h
    include/Vorb/graphics/script/TextureCacheScriptFuncs.h
#source
    src/graphics/script/TextureCacheScriptFuncs.cpp
)
endif()

set(vorb_io
    include/Vorb/io/Directory.h
    include/Vorb/io/File.h
    include/Vorb/io/FileOps.h
    include/Vorb/io/FileStream.h
    include/Vorb/io/IOManager.h
    include/Vorb/io/Keg.h
    include/Vorb/io/KegBasic.h
    include/Vorb/io/KegEnum.h
    include/Vorb/io/KegEnvironment.h
    include/Vorb/io/KegFuncs.h
    include/Vorb/io/KegType.h
    include/Vorb/io/KegTypes.h
    include/Vorb/io/KegValue.h
    include/Vorb/io/MemFile.h
    include/Vorb/io/Path.h
    include/Vorb/io/YAML.h
    include/Vorb/io/YAMLConverters.h
    include/Vorb/io/YAMLImpl.h
    include/Vorb/io/YAMLInternal.inl
    include/Vorb/io/YAMLNode.h
    include/Vorb/io/YAMLReader.h
    include/Vorb/io/YAMLWriter.h
#source
    src/io/Directory.cpp
    src/io/File.cpp
    src/io/FileOps.cpp
    src/io/IOManager.cpp
    src/io/Keg.cpp
    src/io/KegEnum.cpp
    src/io/KegEnvironment.cpp
    src/io/KegRead.cpp
    src/io/KegType.cpp
    src/io/KegValue.cpp
    src/io/KegWrite.cpp
    src/io/MemFile.cpp
    src/io/Path.cpp
    src/io/YAML.cpp
    src/io/YAMLConverters.cpp
)

if(VORB_USE_SCRIPTING)
    set(vorb_script
        include/Vorb/script/IEnvironment.hpp
        include/Vorb/script/ConsoleBackend.hpp
        include/Vorb/script/lua/Environment.h
        include/Vorb/script/lua/LFunction.h
        include/Vorb/script/lua/CFunction.hpp
        include/Vorb/script/lua/ValueMediator.h
    #source
        src/script/lua/Environment.cpp
        src/script/lua/LFunction.cpp
        src/script/lua/ValueMediator.cpp
    )
endif()

set(vorb_sound
    include/Vorb/sound/ISoundImpl.h
    include/Vorb/sound/SoundEngine.h
    include/Vorb/sound/SoundImpl.hpp
    include/Vorb/sound/SoundImplFMOD.inl
    include/Vorb/sound/SoundInstance.h
    include/Vorb/sound/SoundListener.h
    include/Vorb/sound/SoundResource.h
#source
    src/sound/SoundEngine.cpp
    src/sound/SoundInstance.cpp
)

set(vorb_ui
    include/Vorb/ui/Drawables.h
    include/Vorb/ui/FocusController.h
    include/Vorb/ui/GameWindow.h
    include/Vorb/ui/IGameScreen.h
    include/Vorb/ui/InputDispatcher.h
    include/Vorb/ui/InputDispatcherEventCatcher.h
    include/Vorb/ui/KeyboardEventDispatcher.h
    include/Vorb/ui/KeyMappings.inl
    include/Vorb/ui/Keys.inl
    include/Vorb/ui/KeyStrings.h
    include/Vorb/ui/MainGame.h
    include/Vorb/ui/MouseInputDispatcher.h
    include/Vorb/ui/ScreenList.h
    include/Vorb/ui/TextInputListener.hpp
    include/Vorb/ui/UIRenderer.h
    include/Vorb/ui/WindowEventDispatcher.h
    include/Vorb/ui/widgets/Button.h
    include/Vorb/ui/widgets/CheckBox.h
    include/Vorb/ui/widgets/ComboBox.h
    include/Vorb/ui/widgets/IWidget.h
    include/Vorb/ui/widgets/Label.h
    include/Vorb/ui/widgets/Panel.h
    include/Vorb/ui/widgets/Slider.h
    include/Vorb/ui/widgets/TextWidget.h
    include/Vorb/ui/widgets/Viewport.h
    include/Vorb/ui/widgets/Widget.h
    include/Vorb/ui/widgets/WidgetList.h
    include/Vorb/ui/widgets/yaml/Button.h
    include/Vorb/ui/widgets/yaml/CheckBox.h
    include/Vorb/ui/widgets/yaml/helper.hpp
    include/Vorb/ui/widgets/yaml/IWidget.h
    include/Vorb/ui/widgets/yaml/Label.h
    include/Vorb/ui/widgets/yaml/Panel.h
    include/Vorb/ui/widgets/yaml/Slider.h
    include/Vorb/ui/widgets/yaml/TextWidget.h
    include/Vorb/ui/widgets/yaml/Widget.h
#source
    src/ui/Drawables.cpp
    src/ui/FocusController.cpp
    src/ui/GameWindow.cpp
    src/ui/InputDispatcher.cpp
    src/ui/KeyboardEventDispatcher.cpp
    src/ui/MainGame.cpp
    src/ui/MouseInputDispatcher.cpp
    src/ui/ScreenList.cpp
    src/ui/UIRenderer.cpp
    src/ui/widgets/Button.cpp
    src/ui/widgets/CheckBox.cpp
    src/ui/widgets/ComboBox.cpp
    src/ui/widgets/IWidget.cpp
    src/ui/widgets/Label.cpp
    src/ui/widgets/Panel.cpp
    src/ui/widgets/Slider.cpp
    src/ui/widgets/TextWidget.cpp
    src/ui/widgets/Viewport.cpp
    src/ui/widgets/Widget.cpp
    src/ui/widgets/WidgetList.cpp
    src/ui/widgets/yaml/Button.cpp
    src/ui/widgets/yaml/CheckBox.cpp
    src/ui/widgets/yaml/IWidget.cpp
    src/ui/widgets/yaml/Label.cpp
    src/ui/widgets/yaml/Panel.cpp
    src/ui/widgets/yaml/Slider.cpp
    src/ui/widgets/yaml/TextWidget.cpp
    src/ui/widgets/yaml/Widget.cpp
)

if(VORB_USE_SCRIPTING)
set(vorb_ui ${vorb_ui}
    include/Vorb/ui/ScriptedUI.h
    include/Vorb/ui/script/ButtonScriptFuncs.h
    include/Vorb/ui/script/CheckBoxScriptFuncs.h
    include/Vorb/ui/script/ComboBoxScriptFuncs.h
    include/Vorb/ui/script/GameWindowScriptFuncs.h
    include/Vorb/ui/script/GraphicsScriptFuncs.h
    include/Vorb/ui/script/LabelScriptFuncs.h
    include/Vorb/ui/script/PanelScriptFuncs.h
    include/Vorb/ui/script/SliderScriptFuncs.h
    include/Vorb/ui/script/TextWidgetScriptFuncs.h
    include/Vorb/ui/script/ViewportScriptFuncs.h
    include/Vorb/ui/script/ViewScriptEnvironment.h
    include/Vorb/ui/script/WidgetListScriptFuncs.h
    include/Vorb/ui/script/WidgetScriptFuncs.h
    include/Vorb/ui/script/lua/ValueMediator.h
#source
    src/ui/script/ButtonScriptFuncs.cpp
    src/ui/script/CheckBoxScriptFuncs.cpp
    src/ui/script/ComboBoxScriptFuncs.cpp
    src/ui/script/GameWindowScriptFuncs.cpp
    src/ui/script/LabelScriptFuncs.cpp
    src/ui/script/PanelScriptFuncs.cpp
    src/ui/script/SliderScriptFuncs.cpp
    src/ui/script/TextWidgetScriptFuncs.cpp
    src/ui/script/WidgetListScriptFuncs.cpp
    src/ui/script/WidgetScriptFuncs.cpp
    src/ui/script/lua/ValueMediator.cpp
)
endif()

set(vorb_voxel
    include/Vorb/voxel/IntervalTree.h
    include/Vorb/voxel/IntervalTree.inl
    include/Vorb/voxel/VoxCommon.h
    include/Vorb/voxel/VoxelMeshAlg.h
    include/Vorb/voxel/VoxelMesherCulled.h
    include/Vorb/voxel/VoxelTextureStitcher.h
#source
    src/voxel/VoxCommon.cpp
    src/voxel/VoxelTetureStitcher.cpp
)

set(vorb_headers
    include/Vorb/Asset.h
    include/Vorb/AssetLoader.h
    include/Vorb/AssetLoader.inl
    include/Vorb/blockingconcurrentqueue.h
    include/Vorb/BufferUtils.inl
    include/Vorb/colors.h
    include/Vorb/compat.h
    include/Vorb/concurrentqueue.h
    include/Vorb/Constants.h
    include/Vorb/decorators.h
    include/Vorb/Delegate.hpp
    include/Vorb/Event.hpp
    include/Vorb/FastConversion.inl
    include/Vorb/FixedSizeArrayRecycler.hpp
    include/Vorb/Graphics.h
    include/Vorb/IDGenerator.h
    include/Vorb/ImplGraphicsH.inl
    include/Vorb/IntersectionUtils.hpp
    include/Vorb/IO.h
    include/Vorb/IThreadPoolTask.h
    include/Vorb/Matrix.hpp
    include/Vorb/Matrix.inl
    include/Vorb/MeshGenerators.h
    include/Vorb/PtrRecycler.hpp
    include/Vorb/Quaternion.hpp
    include/Vorb/Quaternion.inl
    include/Vorb/Radix.inl
    include/Vorb/Random.h
    include/Vorb/RingBuffer.hpp
    include/Vorb/RingBuffer.inl
    include/Vorb/vorb_rpc.h
    include/Vorb/ScopedTiming.hpp
    include/Vorb/stdafx.h
    include/Vorb/TextureRecycler.hpp
    include/Vorb/ThreadPool.h
    include/Vorb/ThreadPool.inl
    include/Vorb/ThreadSync.h
    include/Vorb/Timing.h
    include/Vorb/TypeManip.hpp
    include/Vorb/types.h
    include/Vorb/typesArray.inl
    include/Vorb/typesColor.inl
    include/Vorb/utils.h
    include/Vorb/Vector.hpp
    include/Vorb/Vector.inl
    include/Vorb/Vorb.h
    include/Vorb/VorbAssert.hpp
    include/Vorb/VorbLibs.h
    include/Vorb/VorbMemory.h
    include/Vorb/VorbPreDecl.inl
)


set(vorb_source
    src/colors.cpp
    src/MeshGenIcosphere.cpp
    src/Random.cpp
    src/vorb_rpc.cpp
    src/stdafx.cpp
    src/Timing.cpp
    src/Vorb.cpp

)

add_library(vorb
    ${vorb_ecs}
    ${vorb_graphics}
    ${vorb_io}
    ${vorb_script}
    ${vorb_sound}
    ${vorb_ui}
    ${vorb_voxel}
    ${vorb_headers}
    ${vorb_source}
)

source_group("ecs" FILES ${vorb_ecs})
source_group("graphics" FILES ${vorb_graphics})
source_group("io" FILES ${vorb_io})
source_group("script" FILES ${vorb_script})
source_group("sound" FILES ${vorb_sound})
source_group("ui" FILES ${vorb_ui})
source_group("voxel" FILES ${vorb_voxel})

source_group("include" FILES ${vorb_headers})
source_group("source" FILES ${vorb_source})

target_link_libraries(vorb
    SDL2::SDL2main
    SDL2::SDL2
    SDL_ttf::SDL_ttf
    glew::glew
    Boost::filesystem
    Boost::system
    glm
    enet::enet
    yaml-cpp::yaml-cpp
#    OpenSSL::SSL
#    OpenSSL::Crypto
    Lua::lua_lib
    PNG::png
)

target_include_directories(vorb PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_definitions(vorb PUBLIC -DVORB_IMPL_GRAPHICS_OPENGL -DVORB_IMPL_UI_SDL -DVORB_IMPL_FONT_SDL -DVORB_IMPL_SOUND_FMOD)

if(VORB_USE_SCRIPTING)
    target_compile_definitions(vorb PUBLIC -DVORB_USING_SCRIPT)
endif()

add_subdirectory(tests)

