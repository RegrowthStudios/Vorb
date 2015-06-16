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
            virtual ~BitmapResource() {};
            ui32 width = 0;
            ui32 height = 0;
            union {
                void* data = nullptr;
                ui8* bytesUI8;
                ui8v2* bytesUI8v2;
                ui8v3* bytesUI8v3;
                ui8v4* bytesUI8v4;
                ui16* bytesUI16;
                ui16v2* bytesUI16v2;
                ui16v3* bytesUI16v3;
                ui16v4* bytesUI16v4;
                f32* bytesF32;
                f32v2* bytesF32v2;
                f32v3* bytesF32v3;
                f32v4* bytesF32v4;
                f64* bytesF64;
                f64v2* bytesF64v2;
                f64v3* bytesF64v3;
                f64v4* bytesF64v4;
            };
        };

        class ImageIO {
        public:
            static BitmapResource alloc(const ui32& w, const ui32& h, const ImageIOFormat& format);
            static void free(BitmapResource& res);

            BitmapResource load(const vio::Path& path,
                                const ImageIOFormat& format = ImageIOFormat::RGBA_UI8,
                                bool flipV = false);
            bool save(const vio::Path& path, const void* inData, const ui32& w,
                      const ui32& h, const ImageIOFormat& format);

            Event<nString> onError;
        };

        /// Destroys the resource in the destructor
        class ScopedBitmapResource : public BitmapResource {
        public:
            ScopedBitmapResource() {};
            ScopedBitmapResource(ScopedBitmapResource& other) = delete;
            ScopedBitmapResource(const BitmapResource& rs) {
                memcpy(this, &rs, sizeof(BitmapResource));
            }
            virtual ~ScopedBitmapResource() {
                ImageIO::free(*this);
            }
            ScopedBitmapResource& operator=(const BitmapResource& rs) {
                memcpy(this, &rs, sizeof(BitmapResource));
                return *this;
            }
        private:
            VORB_NON_COPYABLE(ScopedBitmapResource);
        };
    }
}
namespace vg = vorb::graphics;

#endif // ImageIO_h__