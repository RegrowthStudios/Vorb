#include "stdafx.h"
#include "ImageLoader.h"
#include "lodepng.h"
#include "IOManager.h"

#include <sys/types.h>
#include <sys/stat.h>

bool vg::ImageLoader::loadPng(const cString imagepath, std::vector<ui8>& pixelStore, ui32& rWidth, ui32& rHeight, vio::IOManager* iom, bool printError /* = true */) {
   
    std::vector <ui8> imgData;

    if (!iom->readFileToData(imagepath, imgData)) {
        std::cerr << "Failed to open PNG " << imagepath << std::endl;
        return false;
    }

    unsigned error = lodepng::decode(pixelStore, rWidth, rHeight, imgData);

    //if there's an error, display it
    if (error) {
        std::cerr << "png decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
        return false;
    }
    return true;
}
