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
    }
    namespace graphics {
    }
    namespace voxel {
    }
    namespace ui {
    }
    namespace io {
    }
    namespace sound {
    }
    namespace ecs {
    }
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

#define DECL_VORB(DECL) namespace vorb { DECL; }
#define DECL_VCORE(DECL) namespace vorb { namespace core { DECL; }  }
#define DECL_VG(DECL) namespace vorb { namespace graphics { DECL; }  }
#define DECL_VVOX(DECL) namespace vorb { namespace voxel { DECL; }  }
#define DECL_VUI(DECL) namespace vorb { namespace ui { DECL; }  }
#define DECL_VIO(DECL) namespace vorb { namespace io { DECL; }  }
#define DECL_VSOUND(DECL) namespace vorb { namespace sound { DECL; }  }
#define DECL_VECS(DECL) namespace vorb { namespace ecs { DECL; }  }
#define DECL_VNET(DECL) namespace vorb { namespace net { DECL; }  }

#endif // VorbPreDecl_inl__
