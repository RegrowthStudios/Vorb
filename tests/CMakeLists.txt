add_executable(maintest main.cpp)
target_link_libraries(maintest vorb)

#include(cotire)

#cotire(maintest)

# cotire sets the following properties
#get_target_property(_unitySource maintest COTIRE_CXX_UNITY_SOURCE)
#get_target_property(_prefixHeader maintest COTIRE_CXX_PREFIX_HEADER)
#get_target_property(_precompiledHeader maintest COTIRE_CXX_PRECOMPILED_HEADER)
#get_target_property(_unityTargetName maintest COTIRE_UNITY_TARGET_NAME)

if (_unitySource)
    message(STATUS "maintest unity source: ${_unitySource}")
endif()
if (_prefixHeader)
    message(STATUS "maintest prefix header: ${_prefixHeader}")
endif()
if (_precompiledHeader)
    message(STATUS "maintest precompiled header: ${_precompiledHeader}")
endif()
if (TARGET ${_unityTargetName})
    message(STATUS "maintest unity target: ${_unityTargetName}")
endif()