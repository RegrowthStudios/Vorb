//
// ImageIO.h
// Vorb Engine
//
// Created by Cristian Zaloj on 8 Dec 2014
// Copyright 2014 Regrowth Studios
// MIT License
//

/*! \file ImageIO.h
 * @brief 
 */

#pragma once

#ifndef Vorb_ImageIO_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_ImageIO_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "Vorb/types.h"
#endif // !VORB_USING_PCH

#include "../Event.hpp"
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
            BitmapResource():width(0), height(0), data(nullptr){}
            virtual ~BitmapResource() {};

            ui32 width;
            ui32 height;
            union {
                void* data;
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
            ScopedBitmapResource(const BitmapResource& rs) {
//                memcpy(this, &rs, sizeof(BitmapResource));
                this->width=rs.width;
                this->height=rs.height;
                this->data=rs.data;
            }
            virtual ~ScopedBitmapResource() {
                ImageIO::free(*this);
            }
            ScopedBitmapResource& operator=(const BitmapResource& rs) {
//                memcpy(this, &rs, sizeof(BitmapResource));
                this->width=rs.width;
                this->height=rs.height;
                this->data=rs.data;
                return *this;
            }
        private:
            VORB_NON_COPYABLE(ScopedBitmapResource);
        };
    }
}
namespace vg = vorb::graphics;

#endif // !Vorb_ImageIO_h__
