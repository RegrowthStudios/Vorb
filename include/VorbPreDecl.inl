///
/// VorbPreDecl.h
/// Vorb Engine
///
/// Created by Cristian Zaloj on 2 Jan 2015
/// Copyright 2014 Regrowth Studios
/// All Rights Reserved
///
/// Summary:
/// Pre-declaration helpers
///

#pragma once

#ifndef VorbPreDecl_inl__
#define VorbPreDecl_inl__

/** @brief The root namespace for most Vorb classes.
 * 
 * This holds core submodules as well as various
 * extensions. ADT's, utilities, and common
 * engine functionality is implemented here.
 */
namespace vorb {
    namespace core {
    }
    /** @brief Vorb's graphics submodule.
     * 
     * Utilities for modifying context state as well as imaging, mesh,
     * and debugging renderers can be found here.
     */
    namespace graphics {
    }
    namespace voxel {
    }
    /** @brief Vorb's UI submodule.
     * 
     * It provides an abstraction over the underlying OS to be able to provide
     * a window, graphics context, and input handling. It also contains
     * controller code for a screen system.
     */
    namespace ui {
    }
    /** @brief Vorb's IO submodule.
     * 
     * It simplifies tasks that relate to the filesystem, such as directory
     * enumeration, file reading and writing, and path operations.
     */
    namespace io {
    }
    /** @brief Vorb's sound submodule.
     * 
     * It builds and maintains the audio context necessary to produce sound as
     * well as an engine that controls how resources are loaded and instanced.
     */
    namespace sound {
    }
    /** @brief Vorb's Entity Component System extension.
     * 
     * This provides an implementation template for structuring an ECS system.
     */
    namespace ecs {
    }
    /** @brief Vorb's network module.
     * 
     * It currently initializes and destroys enet.
     */
    namespace net {
    }
}

// Namespace aliases
namespace vcore = vorb::core; ///< Namespace alias vorb::core
namespace vg = vorb::graphics; ///< Namespace alias vorb::graphics
namespace vvox = vorb::voxel; ///< Namespace alias vorb::voxel
namespace vui = vorb::ui; ///< Namespace alias vorb::ui
namespace vio = vorb::io; ///< Namespace alias vorb::io
namespace vsound = vorb::sound; ///< Namespace alias vorb::sound
namespace vecs = vorb::ecs; ///< Namespace alias vorb::ecs
namespace vnet = vorb::net; ///< Namespace alias vorb::net

#define DECL_VORB(...) namespace vorb { __VA_ARGS__; }
#define DECL_VCORE(...) namespace vorb { namespace core { __VA_ARGS__; }  }
#define DECL_VG(...) namespace vorb { namespace graphics { __VA_ARGS__; }  }
#define DECL_VVOX(...) namespace vorb { namespace voxel { __VA_ARGS__; }  }
#define DECL_VUI(...) namespace vorb { namespace ui { __VA_ARGS__; }  }
#define DECL_VIO(...) namespace vorb { namespace io { __VA_ARGS__; }  }
#define DECL_VSOUND(...) namespace vorb { namespace sound { __VA_ARGS__; }  }
#define DECL_VECS(...) namespace vorb { namespace ecs { __VA_ARGS__; }  }
#define DECL_VNET(...) namespace vorb { namespace net { __VA_ARGS__; }  }

#endif // VorbPreDecl_inl__

/** \example "Vorb Module Initialization"
 * 
 * When using Vorb, you must initialize and dispose the submodules that you wish to use
 * at the top level. You can specify any combinations as well as initialize and dispose at
 * any time.
 * \include VorbInit.cpp
 */
