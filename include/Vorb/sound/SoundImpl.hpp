///
/// SoundImpl.hpp
/// Vorb Engine
///
/// Created by Cristian Zaloj on 3 Jan 2015
/// Copyright 2014 Regrowth Studios
/// MIT License
///
/// Summary:
/// Sound implementation switch
///

#pragma once

#ifndef SoundImpl_hpp__
#define SoundImpl_hpp__

#if defined(VORB_IMPL_SOUND_FMOD)
#include "Vorb/sound/SoundImplFMOD.inl"
#else
#error Missing sound implementation
#endif // VORB_IMPL_SOUND

#endif // SoundImpl_hpp__