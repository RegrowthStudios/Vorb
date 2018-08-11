#include "Vorb/stdafx.h"
#include "Vorb/graphics/ImageIO.h"

//#include <FreeImage.h>

#include <png.h>
#include "Vorb/graphics/ImageIOConv.inl"

namespace vorb {
    namespace graphics {
        namespace impl {
//            FIBITMAP* makeRGB(FIBITMAP* bmp) {
//                FIBITMAP* tmp = bmp;
//                bmp = FreeImage_ConvertTo24Bits(bmp);
//                FreeImage_Unload(tmp);
//                return bmp;
//            }
//            FIBITMAP* makeRGBA(FIBITMAP* bmp) {
//                FIBITMAP* tmp = bmp;
//                bmp = FreeImage_ConvertTo32Bits(bmp);
//                FreeImage_Unload(tmp);
//                return bmp;
//            }
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
void vg::ImageIO::free(BitmapResource& res) {
    delete[] res.bytesUI8;
    res.bytesUI8 = nullptr;
}

vg::ImageIOFormat pngToImageIoFormat(png_byte color_type, png_byte bit_depth)
{
    switch(color_type)
    {
    case PNG_COLOR_TYPE_RGB:
        switch(bit_depth)
        {
        case 8:
            return vg::ImageIOFormat::RGB_UI8;
            break;
        case 16:
            return vg::ImageIOFormat::RGB_UI16;
            break;
        }
        break;
    case PNG_COLOR_TYPE_RGB_ALPHA:
        switch(bit_depth)
        {
        case 8:
            return vg::ImageIOFormat::RGBA_UI8;
            break;
        case 16:
            return vg::ImageIOFormat::RGBA_UI16;
            break;
        }
        break;
    }

    return vg::ImageIOFormat::RAW;
}

std::pair<png_byte,png_byte> ImageIoFormatToPng(const vg::ImageIOFormat &format)
{
    std::pair<png_byte, png_byte> value;

    switch(format)
    {
    case vg::ImageIOFormat::RGB_UI8:
        value.first=PNG_COLOR_TYPE_RGB;
        value.second=8;
        break;
    case vg::ImageIOFormat::RGBA_UI8:
        value.first=PNG_COLOR_TYPE_RGB_ALPHA;
        value.second=8;
        break;
    case vg::ImageIOFormat::RGB_UI16:
        value.first=PNG_COLOR_TYPE_RGB;
        value.second=16;
        break;
    case vg::ImageIOFormat::RGBA_UI16:
        value.first=PNG_COLOR_TYPE_RGB_ALPHA;
        value.second=16;
        break;
    default:
        value.first=PNG_COLOR_TYPE_GRAY;
        value.second=0;
        break;
    }

    return value;
}

// TODO: Get this in working order. Reevaluate parameter attribute once done.
vg::BitmapResource vg::ImageIO::load(const vio::Path& path,
                                     const ImageIOFormat& requestedformat /* = ImageIOFormat::RGBA_UI8 */,
                                     bool flipV /*= false*/ VORB_UNUSED) {
    BitmapResource res = {};
    res.data = nullptr;

    FILE *file=fopen(path.getCString(), "rb");

    if(!file)
    {
        onError("Unable to open file");
        return res;
    }

    char header[8];

    fread(header, 1, 8, file);

    if(png_sig_cmp((png_const_bytep)header, 0, 8))
    {
        onError("File type not recognized");
        return res;
    }

    png_structp png_ptr;

    /* initialize stuff */
    png_ptr=png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if(!png_ptr)
    {
        onError("Format not recognized");
        return res;
    }

    png_infop info_ptr;

    info_ptr=png_create_info_struct(png_ptr);
    
    if(!info_ptr)
    {
        onError("Unable to retrieve image information");
        return res;
    }

    if(setjmp(png_jmpbuf(png_ptr)))
    {
        onError("File corrupt");
        return res;
    }

    int width, height;
    png_byte color_type;
    //png_byte bit_depth;
    //int number_of_passes;

    png_init_io(png_ptr, file);
    png_set_sig_bytes(png_ptr, 8);
    png_read_info(png_ptr, info_ptr);

    width=png_get_image_width(png_ptr, info_ptr);
    height=png_get_image_height(png_ptr, info_ptr);
    color_type=png_get_color_type(png_ptr, info_ptr);
    //bit_depth=png_get_bit_depth(png_ptr, info_ptr);

    //number_of_passes=png_set_interlace_handling(png_ptr);

    if(color_type==PNG_COLOR_TYPE_PALETTE)
    {
        png_set_palette_to_rgb(png_ptr);
        color_type=PNG_COLOR_TYPE_RGB;
    }

    if((color_type==PNG_COLOR_TYPE_RGB)&&(requestedformat==ImageIOFormat::RGBA_UI8))
        png_set_add_alpha(png_ptr, 0xff, PNG_FILLER_AFTER);
    else if((color_type==PNG_COLOR_TYPE_RGBA)&&(requestedformat==ImageIOFormat::RGB_UI8))
        png_set_strip_alpha(png_ptr);

    png_read_update_info(png_ptr, info_ptr);
    color_type=png_get_color_type(png_ptr, info_ptr);

    if(setjmp(png_jmpbuf(png_ptr)))
    {
        onError("File corrupt");
        return res;
    }

    int channels = 0;
    int depth = 0;

//    if(color_type==PNG_COLOR_TYPE_GRAY)
//        channels=1;
//    else if(color_type==PNG_COLOR_TYPE_GRAY_ALPHA)
//        channels=2;
//    else if(color_type==PNG_COLOR_TYPE_RGB)
//        channels=3;
//    else if(color_type==PNG_COLOR_TYPE_RGB_ALPHA)
//        channels=4;
//
//    if(bit_depth==8)
//        depth=1;
//    else if(bit_depth==16)
//        depth=2;

    if(requestedformat==ImageIOFormat::RGB_UI8)
    {
        channels=3;
        depth=1;
    }
    else if(requestedformat==ImageIOFormat::RGBA_UI8)
    {
        channels=4;
        depth=1;
    }
    else if(requestedformat==ImageIOFormat::RGB_UI16)
    {
        channels=4;
        depth=2;
    }
    else if(requestedformat==ImageIOFormat::RGBA_UI16)
    {
        channels=4;
        depth=2;
    }

    std::vector<png_bytep> row_pointers(height);
    //ImageIOFormat imageIoFormat=pngToImageIoFormat(color_type, bit_depth);

    res=alloc(width, height, requestedformat);
    png_byte *imageData=(png_byte *)res.data;

    size_t pos=0;
    size_t stride=width*channels*depth;

    for(int y=0; y<height; y++)
    {
        row_pointers[y]=&imageData[pos];
        pos+=stride;
    }

    png_read_image(png_ptr, row_pointers.data());

    fclose(file);

//    if((requestedformat!=imageIoFormat) || flipV)
//    {
//        switch(requestedformat)
//        {
//        case ImageIOFormat::RGB_UI8:
//        case ImageIOFormat::RGB_UI16:
//        case ImageIOFormat::RGB_F32:
//        case ImageIOFormat::RGB_F64:
//            bmp=impl::makeRGB(bmp);
//            f=flipV?impl::flip::convRGB8[(size_t)format]:impl::noflip::convRGB8[(size_t)format];
//            f(bmp, res);
//            break;
//        case ImageIOFormat::RGBA_UI8:
//        case ImageIOFormat::RGBA_UI16:
//        case ImageIOFormat::RGBA_F32:
//        case ImageIOFormat::RGBA_F64:
//            bmp=impl::makeRGBA(bmp);
//            f=flipV?impl::flip::convRGBA8[(size_t)format]:impl::noflip::convRGBA8[(size_t)format];
//            f(bmp, res);
//            break;
//        }
//    }
    
//    FREE_IMAGE_FORMAT fif = FreeImage_GetFIFFromFilename(path.getCString());
//    if (fif == FIF_UNKNOWN) {
//        onError("File type not recognized");
//        return res;
//    }
//
//    FIBITMAP* bmp = FreeImage_Load(fif, path.getCString());
//    FREE_IMAGE_TYPE bmpType = FreeImage_GetImageType(bmp);
//    if (bmpType == FIT_UNKNOWN) {
//        onError("Image type not recognized");
//        FreeImage_Unload(bmp);
//        return res;
//    }
//    auto bpp = FreeImage_GetBPP(bmp);
//    res.width = FreeImage_GetWidth(bmp);
//    res.height = FreeImage_GetHeight(bmp);
//
//    // Memcpy if user wants no conversion
//    if (format == ImageIOFormat::RAW) {
//        size_t byteSize = res.height * FreeImage_GetPitch(bmp);
//        res.data = new ui8[byteSize];
//        memcpy(res.data, FreeImage_GetBits(bmp), byteSize);
//        FreeImage_Unload(bmp);
//        return res;
//    }
//
//    bool error = false;
//    res = alloc(res.width, res.height, format);
//    impl::BMPConvFunc f;
//    switch (bmpType) {
//    case FIT_BITMAP:
//        switch (format) {
//        case ImageIOFormat::RGB_UI8:
//        case ImageIOFormat::RGB_UI16:
//        case ImageIOFormat::RGB_F32:
//        case ImageIOFormat::RGB_F64:
//            bmp = impl::makeRGB(bmp);
//            f = flipV ? impl::flip::convRGB8[(size_t)format] : impl::noflip::convRGB8[(size_t)format];
//            f(bmp, res);
//            break;
//        case ImageIOFormat::RGBA_UI8:
//        case ImageIOFormat::RGBA_UI16:
//        case ImageIOFormat::RGBA_F32:
//        case ImageIOFormat::RGBA_F64:
//            bmp = impl::makeRGBA(bmp);
//            f = flipV ? impl::flip::convRGBA8[(size_t)format] : impl::noflip::convRGBA8[(size_t)format];
//            f(bmp, res);
//            break;
//        default:
//            onError("Unknown format specified");
//            error = true;
//            break;
//        }
//        break;
//    case FIT_UINT16:
//        f = flipV ? impl::flip::convUI16[(size_t)format] : impl::noflip::convUI16[(size_t)format];
//        f(bmp, res);
//        break;
//    case FIT_INT16:
//        f = flipV ? impl::flip::convI16[(size_t)format] : impl::noflip::convI16[(size_t)format];
//        f(bmp, res);
//        break;
//    case FIT_UINT32:
//        f = flipV ? impl::flip::convUI32[(size_t)format] : impl::noflip::convUI32[(size_t)format];
//        f(bmp, res);
//        break;
//    case FIT_INT32:
//        f = flipV ? impl::flip::convI32[(size_t)format] : impl::noflip::convI32[(size_t)format];
//        f(bmp, res);
//        break;
//    case FIT_FLOAT:
//        f = flipV ? impl::flip::convF32[(size_t)format] : impl::noflip::convF32[(size_t)format];
//        f(bmp, res);
//        break;
//    case FIT_DOUBLE:
//        f = flipV ? impl::flip::convF64[(size_t)format] : impl::noflip::convF64[(size_t)format];
//        f(bmp, res);
//        break;
//    case FIT_RGB16:
//        f = flipV ? impl::flip::convRGB16[(size_t)format] : impl::noflip::convRGB16[(size_t)format];
//        f(bmp, res);
//        break;
//    case FIT_RGBA16:
//        f = flipV ? impl::flip::convRGBA16[(size_t)format] : impl::noflip::convRGBA16[(size_t)format];
//        f(bmp, res);
//        break;
//    case FIT_RGBF:
//        f = flipV ? impl::flip::convRGBF[(size_t)format] : impl::noflip::convRGBF[(size_t)format];
//        f(bmp, res);
//        break;
//    case FIT_RGBAF:
//        f = flipV ? impl::flip::convRGBAF[(size_t)format] : impl::noflip::convRGBAF[(size_t)format];
//        f(bmp, res);
//        break;
//    case FIT_COMPLEX:
//        onError("Cannot load imaginary files: please come back to the real world");
//        error = true;
//        break;
//    default:
//        onError("Cannot load imaginary files: please come back to the real world");
//        error = true;
//        break;
//    }
//    FreeImage_Unload(bmp);
//    if (error) {
//        free(res);
//        res.data = nullptr;
//    }
    return res;
}
bool vg::ImageIO::save(const vio::Path& path, const void* inData, const ui32& w, const ui32& h, const ImageIOFormat& format) {

    FILE *file=fopen(path.getCString(), "wb");
    
    if(!file)
    {
        onError("Unable to open file");
        return false;
    }

    png_structp png=png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    
    if(!png)
    {
        onError("Failed to open png handler");
        return false;
    }

    png_infop info=png_create_info_struct(png);
    
    if(!info)
    {
        onError("Failed to create png struct");
        return false;
    }

    if(setjmp(png_jmpbuf(png))) 
    {
        onError("Failed png formating");
        return false;
    }

    png_init_io(png, file);

    auto value=ImageIoFormatToPng(format);

    png_byte color_type=value.first;
    png_byte bit_depth=value.second;

    // Output is 8bit depth, RGBA format.
    png_set_IHDR(
        png,
        info,
        w, h,
        bit_depth,
        color_type,
        PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT,
        PNG_FILTER_TYPE_DEFAULT
    );
    png_write_info(png, info);

    int channels = 0;
    int depth = 0;

    if(color_type==PNG_COLOR_TYPE_GRAY)
        channels=1;
    else if(color_type==PNG_COLOR_TYPE_GRAY_ALPHA)
        channels=2;
    else if(color_type==PNG_COLOR_TYPE_RGB)
        channels=3;
    else if(color_type==PNG_COLOR_TYPE_RGB_ALPHA)
        channels=4;

    if(bit_depth==8)
        depth=1;
    else if(bit_depth==16)
        depth=2;

    // To remove the alpha channel for PNG_COLOR_TYPE_RGB format,
    // Use png_set_filler().
    //png_set_filler(png, 0, PNG_FILLER_AFTER);
    std::vector<png_bytep> row_pointers(h);
    png_byte *imageData=(png_byte *)inData;

    size_t pos=0;
    size_t stride=w*channels*depth;

    for(size_t y = 0; y < h; y++)
    {
        row_pointers[y]=&imageData[pos];
        pos+=stride;
    }

    png_write_image(png, row_pointers.data());
    png_write_end(png, NULL);

    fclose(file);
    return true;

//    assert(false);
//    return false;

//    FREE_IMAGE_FORMAT fif = FreeImage_GetFIFFromFilename(path.getCString());
//    if (fif == FIF_UNKNOWN) {
//        onError("File type not recognized");
//        return false;
//    }

//    ui32 bytes = 0;
//    switch (format) {
//    case ImageIOFormat::RGB_UI8: bytes = 3 * sizeof(ui8); break;
//    case ImageIOFormat::RGBA_UI8: bytes = 4 * sizeof(ui8); break;
//    case ImageIOFormat::RGB_UI16: bytes = 3 * sizeof(ui16); break;
//    case ImageIOFormat::RGBA_UI16: bytes = 4 * sizeof(ui16); break;
//    case ImageIOFormat::RGB_F32: bytes = 3 * sizeof(f32); break;
//    case ImageIOFormat::RGBA_F32: bytes = 4 * sizeof(f32); break;
//    case ImageIOFormat::RGB_F64: bytes = 3 * sizeof(f64); break;
//    case ImageIOFormat::RGBA_F64: bytes = 4 * sizeof(f64); break;
//    default:
//        return false;
//    }
//    FIBITMAP* bmp = FreeImage_Allocate(w, h, bytes << 3);
//    switch (format) {
//    case ImageIOFormat::RGB_UI8: impl::placeRGB<ui8>((ui8*)FreeImage_GetBits(bmp), (ui8*)inData, w, h); break;
//    case ImageIOFormat::RGBA_UI8: impl::placeRGBA<ui8>((ui8*)FreeImage_GetBits(bmp), (ui8*)inData, w, h); break;
//    case ImageIOFormat::RGB_UI16: impl::placeRGB<ui16>((ui16*)FreeImage_GetBits(bmp), (ui16*)inData, w, h); break;
//    case ImageIOFormat::RGBA_UI16: impl::placeRGBA<ui16>((ui16*)FreeImage_GetBits(bmp), (ui16*)inData, w, h); break;
//    case ImageIOFormat::RGB_F32: impl::placeRGB<f32>((f32*)FreeImage_GetBits(bmp), (f32*)inData, w, h); break;
//    case ImageIOFormat::RGBA_F32: impl::placeRGBA<f32>((f32*)FreeImage_GetBits(bmp), (f32*)inData, w, h); break;
//    case ImageIOFormat::RGB_F64: impl::placeRGB<f64>((f64*)FreeImage_GetBits(bmp), (f64*)inData, w, h); break;
//    case ImageIOFormat::RGBA_F64: impl::placeRGBA<f64>((f64*)FreeImage_GetBits(bmp), (f64*)inData, w, h); break;
//    default:
//        return false;
//    }
//    FreeImage_Save(fif, bmp, path.getCString());
//    FreeImage_Unload(bmp);
//
//    return true;
}
