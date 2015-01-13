#include "stdafx.h"
#include "graphics/ImageIO.h"

#include <lodepng/lodepng.h>

bool vio::ImageIO::loadPng(const std::vector<ui8>& inData, std::vector<ui8>& outData, ui32& w, ui32& h) {
    auto error = lodepng::decode(outData, w, h, inData);
    if (error) {
        onError(lodepng_error_text(error));
        return false;
    }
    return true;
}
bool vio::ImageIO::loadPng(const nString& path, std::vector<ui8>& outData, ui32& w, ui32& h) {
    auto error = lodepng::decode(outData, w, h, path);
    if (error) {
        onError(lodepng_error_text(error));
        return false;
    }
    return true;
}
bool vio::ImageIO::savePng(const std::vector<ui8>& inData, std::vector<ui8>& outData, const ui32& w, const ui32& h) {
    auto error = lodepng::encode(outData, inData, w, h);
    if (error) {
        onError(lodepng_error_text(error));
        return false;
    }
    return true;
}
bool vio::ImageIO::savePng(const std::vector<ui8>& inData, const nString& path, const ui32& w, const ui32& h) {
    auto error = lodepng::encode(path, inData, w, h);
    if (error) {
        onError(lodepng_error_text(error));
        return false;
    }
    return true;
}
