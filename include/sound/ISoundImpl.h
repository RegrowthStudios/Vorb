///
/// ISoundImpl.h
/// Vorb Engine
///
/// Created by Cristian Zaloj on 3 Jan 2015
/// Copyright 2014 Regrowth Studios
/// All Rights Reserved
///
/// Summary:
/// Sound data implementation hiding
///

#pragma once

#ifndef ISoundImpl_h__
#define ISoundImpl_h__

namespace vorb {
    namespace sound {
        /// Sound engine generalized pre-declarations
        namespace impl {
            struct EngineData;
            struct InstanceData;
            struct ResourceData;
        }
    }
}
namespace vsound = vorb::sound;

#endif // ISoundImpl_h__