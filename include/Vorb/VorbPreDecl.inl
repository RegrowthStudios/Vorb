//
// VorbPreDecl.inl
// Vorb Engine
//
// Created by Cristian Zaloj on 16 Feb 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file VorbPreDecl.inl
 * @brief Pre-declaration helpers for classes in Vorb namespaces
 */

#pragma once

#ifndef Vorb_VorbPreDecl_inl__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_VorbPreDecl_inl__
//! @endcond

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
    /** @brief Vorb's voxel extension.
     * 
     * The voxel extension provides a suite of utilities for voxel
     * code that wishes to adhere a set of interfaces.
     */
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
    /** @brief Vorb's networking submodule.
     * 
     * It currently initializes and destroys enet.
     */
    namespace net {
    }
    namespace script {
    }
}

// Namespace aliases
namespace vcore = vorb::core;
namespace vg = vorb::graphics;
namespace vvox = vorb::voxel;
namespace vui = vorb::ui;
namespace vio = vorb::io;
namespace vsound = vorb::sound;
namespace vecs = vorb::ecs;
namespace vnet = vorb::net;
namespace vscript = vorb::script;

#define DECL_VORB(...) namespace vorb { __VA_ARGS__; }
#define DECL_VCORE(...) namespace vorb { namespace core { __VA_ARGS__; }  }
#define DECL_VG(...) namespace vorb { namespace graphics { __VA_ARGS__; }  }
#define DECL_VVOX(...) namespace vorb { namespace voxel { __VA_ARGS__; }  }
#define DECL_VUI(...) namespace vorb { namespace ui { __VA_ARGS__; }  }
#define DECL_VIO(...) namespace vorb { namespace io { __VA_ARGS__; }  }
#define DECL_VSOUND(...) namespace vorb { namespace sound { __VA_ARGS__; }  }
#define DECL_VECS(...) namespace vorb { namespace ecs { __VA_ARGS__; }  }
#define DECL_VNET(...) namespace vorb { namespace net { __VA_ARGS__; }  }
#define DECL_VSCRIPT(...) namespace vorb { namespace script { __VA_ARGS__; }  }

#endif // !Vorb_VorbPreDecl_inl__
