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

// Create empty namespaces for aliasing purposes
namespace vorb {
    namespace core {
        namespace graphics {
        }
    }
    namespace voxel {
    }
    namespace ui {
    }
    namespace io {
    }
    namespace sound {
    }
    namespace net {
    }
}

// Namespace aliases
namespace vcore = vorb::core; ///< Namespace alias vorb::core
namespace vg = vorb::core::graphics; ///< Namespace alias vorb::core::graphics
namespace vvox = vorb::voxel; ///< Namespace alias vorb::voxel
namespace vui = vorb::ui; ///< Namespace alias vorb::ui
namespace vio = vorb::io; ///< Namespace alias vorb::ui
namespace vsound = vorb::sound; ///< Namespace alias vorb::sound
namespace vnet = vorb::net; ///< Namespace alias vorb::net

#define DECL_VORB(...) namespace vorb { __VA_ARGS__; }
#define DECL_VCORE(...) namespace vorb { namespace core { __VA_ARGS__; }  }
#define DECL_VG(...) namespace vorb { namespace core { namespace graphics { __VA_ARGS__; }  }  }
#define DECL_VVOX(...) namespace vorb { namespace voxel { __VA_ARGS__; }  }
#define DECL_VUI(...) namespace vorb { namespace ui { __VA_ARGS__; }  }
#define DECL_VIO(...) namespace vorb { namespace io { __VA_ARGS__; }  }
#define DECL_VSOUND(...) namespace vorb { namespace sound { __VA_ARGS__; }  }
#define DECL_VECS(...) namespace vorb { namespace ecs { __VA_ARGS__; }  }
#define DECL_VNET(...) namespace vorb { namespace net { __VA_ARGS__; }  }

#endif // VorbPreDecl_inl__
