///
/// ImageIO.h
/// Vorb Engine
///
/// Created by Cristian Zaloj on 8 Dec 2014
/// Copyright 2014 Regrowth Studios
/// All Rights Reserved
///
/// Summary:
/// 
///

#pragma once

#ifndef ImageIO_h__
#define ImageIO_h__

#include "../Events.hpp"

namespace vorb {
    namespace io {
        class ImageIO {
        public:
            bool loadPng(const std::vector<ui8>& inData, std::vector<ui8>& outData, ui32& w, ui32& h);
            bool loadPng(const nString& path, std::vector<ui8>& outData, ui32& w, ui32& h);
            bool savePng(const std::vector<ui8>& inData, std::vector<ui8>& outData, const ui32& w, const ui32& h);
            bool savePng(const std::vector<ui8>& inData, const nString& path, const ui32& w, const ui32& h);

            Event<nString> onError;
        };
    }
}
namespace vio = vorb::io;

#endif // ImageIO_h__