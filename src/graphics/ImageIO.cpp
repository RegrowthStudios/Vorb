#include "stdafx.h"
#include "graphics/ImageIO.h"

#include <FreeImage.h>

#include "ImageIOConv.inl"

namespace vorb {
    namespace graphics {
        namespace impl {
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

vg::BitmapResource vg::ImageIO::load(const vio::Path& path,
                                     const ImageIOFormat& format /* = ImageIOFormat::RGBA_UI8 */,
                                     bool flipV /*= false*/) {
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
            f = flipV ? impl::flip::convRGB8[(size_t)format] : impl::noflip::convRGB8[(size_t)format];
            f(bmp, res);
            break;
        case ImageIOFormat::RGBA_UI8:
        case ImageIOFormat::RGBA_UI16:
        case ImageIOFormat::RGBA_F32:
        case ImageIOFormat::RGBA_F64:
            bmp = impl::makeRGBA(bmp);
            f = flipV ? impl::flip::convRGBA8[(size_t)format] : impl::noflip::convRGBA8[(size_t)format];
            f(bmp, res);
            break;
        default:
            onError("Unknown format specified");
            error = true;
            break;
        }
        break;
    case FIT_UINT16:
        f = flipV ? impl::flip::convUI16[(size_t)format] : impl::noflip::convUI16[(size_t)format];
        f(bmp, res);
        break;
    case FIT_INT16:
        f = flipV ? impl::flip::convI16[(size_t)format] : impl::noflip::convI16[(size_t)format];
        f(bmp, res);
        break;
    case FIT_UINT32:
        f = flipV ? impl::flip::convUI32[(size_t)format] : impl::noflip::convUI32[(size_t)format];
        f(bmp, res);
        break;
    case FIT_INT32:
        f = flipV ? impl::flip::convI32[(size_t)format] : impl::noflip::convI32[(size_t)format];
        f(bmp, res);
        break;
    case FIT_FLOAT:
        f = flipV ? impl::flip::convF32[(size_t)format] : impl::noflip::convF32[(size_t)format];
        f(bmp, res);
        break;
    case FIT_DOUBLE:
        f = flipV ? impl::flip::convF64[(size_t)format] : impl::noflip::convF64[(size_t)format];
        f(bmp, res);
        break;
    case FIT_RGB16:
        f = flipV ? impl::flip::convRGB16[(size_t)format] : impl::noflip::convRGB16[(size_t)format];
        f(bmp, res);
        break;
    case FIT_RGBA16:
        f = flipV ? impl::flip::convRGBA16[(size_t)format] : impl::noflip::convRGBA16[(size_t)format];
        f(bmp, res);
        break;
    case FIT_RGBF:
        f = flipV ? impl::flip::convRGBF[(size_t)format] : impl::noflip::convRGBF[(size_t)format];
        f(bmp, res);
        break;
    case FIT_RGBAF:
        f = flipV ? impl::flip::convRGBAF[(size_t)format] : impl::noflip::convRGBAF[(size_t)format];
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
    FREE_IMAGE_FORMAT fif = FreeImage_GetFIFFromFilename(path.getCString());
    if (fif == FIF_UNKNOWN) {
        onError("File type not recognized");
        return false;
    }

    ui32 bytes = 0;
    switch (format) {
    case ImageIOFormat::RGB_UI8: bytes = 3 * sizeof(ui8); break;
    case ImageIOFormat::RGBA_UI8: bytes = 4 * sizeof(ui8); break;
    case ImageIOFormat::RGB_UI16: bytes = 3 * sizeof(ui16); break;
    case ImageIOFormat::RGBA_UI16: bytes = 4 * sizeof(ui16); break;
    case ImageIOFormat::RGB_F32: bytes = 3 * sizeof(f32); break;
    case ImageIOFormat::RGBA_F32: bytes = 4 * sizeof(f32); break;
    case ImageIOFormat::RGB_F64: bytes = 3 * sizeof(f64); break;
    case ImageIOFormat::RGBA_F64: bytes = 4 * sizeof(f64); break;
    default:
        return false;
    }
    FIBITMAP* bmp = FreeImage_Allocate(w, h, bytes << 3);
    switch (format) {
    case ImageIOFormat::RGB_UI8: impl::placeRGB<ui8>((ui8*)FreeImage_GetBits(bmp), (ui8*)inData, w, h); break;
    case ImageIOFormat::RGBA_UI8: impl::placeRGBA<ui8>((ui8*)FreeImage_GetBits(bmp), (ui8*)inData, w, h); break;
    case ImageIOFormat::RGB_UI16: impl::placeRGB<ui16>((ui16*)FreeImage_GetBits(bmp), (ui16*)inData, w, h); break;
    case ImageIOFormat::RGBA_UI16: impl::placeRGBA<ui16>((ui16*)FreeImage_GetBits(bmp), (ui16*)inData, w, h); break;
    case ImageIOFormat::RGB_F32: impl::placeRGB<f32>((f32*)FreeImage_GetBits(bmp), (f32*)inData, w, h); break;
    case ImageIOFormat::RGBA_F32: impl::placeRGBA<f32>((f32*)FreeImage_GetBits(bmp), (f32*)inData, w, h); break;
    case ImageIOFormat::RGB_F64: impl::placeRGB<f64>((f64*)FreeImage_GetBits(bmp), (f64*)inData, w, h); break;
    case ImageIOFormat::RGBA_F64: impl::placeRGBA<f64>((f64*)FreeImage_GetBits(bmp), (f64*)inData, w, h); break;
    default:
        return false;
    }
    FreeImage_Save(fif, bmp, path.getCString());
    FreeImage_Unload(bmp);

    return true;
}
