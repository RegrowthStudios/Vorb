///
/// SoundResource.h
/// Vorb Engine
///
/// Created by Cristian Zaloj on 3 Jan 2015
/// Copyright 2014 Regrowth Studios
/// All Rights Reserved
///
/// Summary:
/// 
///

#pragma once

#ifndef SoundResource_h__
#define SoundResource_h__

#include "SoundImpl.h"

namespace vorb {
    namespace sound {
        class Resource {
            friend class Engine;
        public:

        private:
            impl::ResourceData* m_data = nullptr;
        };
    }
}
namespace vsound = vorb::sound;

#endif // SoundResource_h__
