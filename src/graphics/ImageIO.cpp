#include "stdafx.h"
#include "graphics/ImageIO.h"

#include <lodepng/lodepng.h>
#include <FreeImage.h>

#include "ImageIOConv.inl"

namespace vorb {
    namespace graphics {
        namespace impl {
            struct ImageBitFormat {
            public:
                ui32 bitsPerPixel;
                ui32 maskShift;
            };

            ImageBitFormat formats[] = {
                {},
                { 24, 8 },
                { 32, 8 },
                { 48, 16 },
                { 64, 16 }
            };
            template<typename T>
            struct RGBMask {
            public:
                union {
                    T bits[3];
                    ui32 data;
                };
            };
            template<typename T>
            struct RGBAMask {
            public:
                union {
                    T bits[4];
                    ui32 data;
                };
            };
            template<typename T>
            ui32 redMask() {
                RGBMask<T> mask = {};
                mask.bits[0] = (T)(~0u);
                return mask.data;
            }
            template<typename T>
            ui32 greenMask() {
                RGBMask<T> mask = {};
                mask.bits[1] = (T)(~0u);
                return mask.data;
            }
            template<typename T>
            ui32 blueMask() {
                RGBMask<T> mask = {};
                mask.bits[2] = (T)(~0u);
                return mask.data;
            }
            template<typename T>
            ui32 redMaskA() {
                RGBAMask<T> mask = {};
                mask.bits[2] = (T)(~0u);
                return mask.data;
            }
            template<typename T>
            ui32 greenMaskA() {
                RGBAMask<T> mask = {};
                mask.bits[1] = (T)(~0u);
                return mask.data;
            }
            template<typename T>
            ui32 blueMaskA() {
                RGBAMask<T> mask = {};
                mask.bits[0] = (T)(~0u);
                return mask.data;
            }

            FIBITMAP* makeRGB(FIBITMAP* bmp) {
                FIBITMAP* tmp = bmp;
                bmp = FreeImage_ConvertTo24Bits(bmp);
                FreeImage_Unload(tmp);
                return bmp;
            }
            FIBITMAP* makeRGBA(FIBITMAP* bmp) {
                FIBITMAP* tmp = bmp;
                bmp = FreeImage_ConvertTo32Bits(bmp);
                FreeImage_Unload(tmp);
                return bmp;
            }
        }
    }
}



bool vg::ImageIO::loadPng(const std::vector<ui8>& inData, std::vector<ui8>& outData, ui32& w, ui32& h) {
    auto error = lodepng::decode(outData, w, h, inData);
    if (error) {
        onError(lodepng_error_text(error));
        return false;
    }
    return true;
}
bool vg::ImageIO::loadPng(const nString& path, std::vector<ui8>& outData, ui32& w, ui32& h) {
    auto error = lodepng::decode(outData, w, h, path);
    if (error) {
        onError(lodepng_error_text(error));
        return false;
    }
    return true;
}
bool vg::ImageIO::savePng(const std::vector<ui8>& inData, std::vector<ui8>& outData, const ui32& w, const ui32& h) {
    auto error = lodepng::encode(outData, inData, w, h);
    if (error) {
        onError(lodepng_error_text(error));
        return false;
    }
    return true;
}
bool vg::ImageIO::savePng(const std::vector<ui8>& inData, const nString& path, const ui32& w, const ui32& h) {
    auto error = lodepng::encode(path, inData, w, h);
    if (error) {
        onError(lodepng_error_text(error));
        return false;
    }
    return true;
}

vg::BitmapResource vg::ImageIO::alloc(const ui32& w, const ui32& h, const ImageIOFormat& format) {
    BitmapResource res = {};
    res.data = nullptr;
    res.width = w;
    res.height = h;
    size_t size = 0;
    switch (format) {
    case ImageIOFormat::RGB_UI8:
        size = sizeof(ui8)* 3;
        break;
    case ImageIOFormat::RGBA_UI8:
        size = sizeof(ui8)* 4;
        break;
    case ImageIOFormat::RGB_UI16:
        size = sizeof(ui16)* 3;
        break;
    case ImageIOFormat::RGBA_UI16:
        size = sizeof(ui16)* 4;
        break;
    case ImageIOFormat::RGB_F32:
        size = sizeof(f32)* 3;
        break;
    case ImageIOFormat::RGBA_F32:
        size = sizeof(f32)* 4;
        break;
    case ImageIOFormat::RGB_F64:
        size = sizeof(f64)* 3;
        break;
    case ImageIOFormat::RGBA_F64:
        size = sizeof(f64)* 4;
        break;
    default:
        return res;
    }
    res.bytesUI8 = new ui8[size * res.width * res.height]();
    return res;
}
void vg::ImageIO::free(const BitmapResource& res) {
    delete[] res.bytesUI8;
}

vg::BitmapResource vg::ImageIO::load(const vio::Path& path, const ImageIOFormat& format) {
    BitmapResource res = {};
    res.data = nullptr;

    FREE_IMAGE_FORMAT fif = FreeImage_GetFIFFromFilename(path.getCString());
    if (fif == FIF_UNKNOWN) {
        onError("File type not recognized");
        return res;
    }

    FIBITMAP* bmp = FreeImage_Load(fif, path.getCString());
    FREE_IMAGE_TYPE bmpType = FreeImage_GetImageType(bmp);
    if (bmpType == FIT_UNKNOWN) {
        onError("Image type not recognized");
        FreeImage_Unload(bmp);
        return res;
    }
    auto bpp = FreeImage_GetBPP(bmp);
    res.width = FreeImage_GetWidth(bmp);
    res.height = FreeImage_GetHeight(bmp);

    // Memcpy if user wants no conversion
    if (format == ImageIOFormat::RAW) {
        size_t byteSize = res.height * FreeImage_GetPitch(bmp);
        res.data = new ui8[byteSize];
        memcpy(res.data, FreeImage_GetBits(bmp), byteSize);
        FreeImage_Unload(bmp);
        return res;
    }

    bool error = false;
    res = alloc(res.width, res.height, format);
    impl::BMPConvFunc f;
    switch (bmpType) {
    case FIT_BITMAP:
        switch (format) {
        case ImageIOFormat::RGB_UI8:
        case ImageIOFormat::RGB_UI16:
        case ImageIOFormat::RGB_F32:
        case ImageIOFormat::RGB_F64:
            bmp = impl::makeRGB(bmp);
            f = impl::convRGB8[(size_t)format];
            f(bmp, res);
            break;
        case ImageIOFormat::RGBA_UI8:
        case ImageIOFormat::RGBA_UI16:
        case ImageIOFormat::RGBA_F32:
        case ImageIOFormat::RGBA_F64:
            bmp = impl::makeRGBA(bmp);
            f = impl::convRGBA8[(size_t)format];
            f(bmp, res);
            break;
        default:
            onError("Unknown format specified");
            error = true;
            break;
        }
        break;
    case FIT_UINT16:
        f = impl::convUI16[(size_t)format];
        f(bmp, res);
        break;
    case FIT_INT16:
        f = impl::convI16[(size_t)format];
        f(bmp, res);
        break;
    case FIT_UINT32:
        f = impl::convUI32[(size_t)format];
        f(bmp, res);
        break;
    case FIT_INT32:
        f = impl::convI32[(size_t)format];
        f(bmp, res);
        break;
    case FIT_FLOAT:
        f = impl::convF32[(size_t)format];
        f(bmp, res);
        break;
    case FIT_DOUBLE:
        f = impl::convF64[(size_t)format];
        f(bmp, res);
        break;
    case FIT_RGB16:
        f = impl::convRGB16[(size_t)format];
        f(bmp, res);
        break;
    case FIT_RGBA16:
        f = impl::convRGBA16[(size_t)format];
        f(bmp, res);
        break;
    case FIT_RGBF:
        f = impl::convRGBF[(size_t)format];
        f(bmp, res);
        break;
    case FIT_RGBAF:
        f = impl::convRGBAF[(size_t)format];
        f(bmp, res);
        break;
    case FIT_COMPLEX:
        onError("Cannot load imaginary files: please come back to the real world");
        error = true;
        break;
    default:
        onError("Cannot load imaginary files: please come back to the real world");
        error = true;
        break;
    }
    FreeImage_Unload(bmp);
    if (error) {
        free(res);
        res.data = nullptr;
    }
    return res;
}
bool vg::ImageIO::save(const vio::Path& path, const void* inData, const ui32& w, const ui32& h, const ImageIOFormat& format) {
    return true;
}

