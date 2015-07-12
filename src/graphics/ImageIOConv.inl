#ifndef VORB_USING_PCH
#include "compat.h"
#endif // !VORB_USING_PCH

namespace vorb {
    namespace graphics {
        namespace impl {

#if defined(OS_WINDOWS)
#define R_OFFSET 2
#define G_OFFSET 1
#define B_OFFSET 0
#define A_OFFSET 3
#else
#define R_OFFSET 0
#define G_OFFSET 1
#define B_OFFSET 2
#define A_OFFSET 3
#error Please research and test wtf is FreeImage doing?
#endif


            typedef void(*BMPConvFunc)(FIBITMAP*, vg::BitmapResource&);

            namespace flip {
#define SCAN_Y(Y, H) (Y)
#include "ImageIOConvF.inl"
#undef SCAN_Y
            }
            namespace noflip {
#define SCAN_Y(Y, H) (H - 1 - Y)
#include "ImageIOConvF.inl"
#undef SCAN_Y
            }

            template<typename T>
            void placeRGB(T* dst, T* src, const ui32& w, const ui32& h) {
                size_t end = w * h * 3;
                size_t si = 0, di = 0;
                while (si < end) {
                    dst[di++] = src[si + R_OFFSET];
                    dst[di++] = src[si + G_OFFSET];
                    dst[di++] = src[si + B_OFFSET];
                    si += 3;
                }
            }
            template<typename T>
            void placeRGBA(T* dst, T* src, const ui32& w, const ui32& h) {
                size_t end = w * h * 4;
                size_t si = 0, di = 0;
                while (si < end) {
                    dst[di++] = src[si + R_OFFSET];
                    dst[di++] = src[si + G_OFFSET];
                    dst[di++] = src[si + B_OFFSET];
                    dst[di++] = src[si + A_OFFSET];
                    si += 4;
                }
            }
        }
    }
}
