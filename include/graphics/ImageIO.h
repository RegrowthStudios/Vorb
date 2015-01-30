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
#include "../io/Path.h"

namespace vorb {
    namespace graphics {
        enum class ImageIOFormat {
            RAW = 0,
            RGB_UI8,
            RGBA_UI8,
            RGB_UI16,
            RGBA_UI16,
            RGB_F32,
            RGBA_F32,
            RGB_F64,
            RGBA_F64
        };
        class ImageIO;

        class BitmapResource {
            friend class ImageIO;
        public:
            ui32 width;
            ui32 height;
            union {
                void* data;
                ui8* bytesUI8;
                ui16* bytesUI16;
                f32* bytesF32;
                f64* bytesF64;
            };
        };

        class ImageIO {
        public:
            static BitmapResource alloc(const ui32& w, const ui32& h, const ImageIOFormat& format);
            static void free(const BitmapResource& res);

            BitmapResource load(const vio::Path& path, const ImageIOFormat& format);
            bool save(const vio::Path& path, const void* inData, const ui32& w, const ui32& h, const ImageIOFormat& format);

            Event<nString> onError;
        };
    }
}
namespace vg = vorb::graphics;

#endif // ImageIO_h__