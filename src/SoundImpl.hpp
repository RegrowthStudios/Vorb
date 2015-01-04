///
/// SoundImpl.hpp
/// Vorb Engine
///
/// Created by Cristian Zaloj on 3 Jan 2015
/// Copyright 2014 Regrowth Studios
/// All Rights Reserved
///
/// Summary:
/// Sound implementation switch
///

#pragma once

#ifndef SoundImpl_hpp__
#define SoundImpl_hpp__

// TODO: Pass this into the preprocessor
#define VORB_SOUND_IMPL_FMOD

#ifdef VORB_SOUND_IMPL_FMOD
#include "SoundImplFMOD.inl"
#else
// No other sound implementation yet
#endif // VORB_SOUND_IMPL_FMOD

#endif // SoundImpl_hpp__