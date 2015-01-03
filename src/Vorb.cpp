#include "stdafx.h"
#include "Vorb.h"

#include <boost/filesystem.hpp>

#include "ConnectedTextures.h"
#include "IOManager.h"
#include "utils.h"

namespace vorb {
    InitParam initGraphics() {
        vg::ConnectedTextureHelper::init();
        return InitParam::GRAPHICS;
    }
    InitParam initIO() {
        // Correctly retrieve initial path
        vpath path = boost::filesystem::initial_path().string();

        // Set the executable directory
#ifdef OS_WINDOWS
        {
            nString buf;
            buf.resize(1024);
            GetModuleFileName(nullptr, &buf[0], 1024 * sizeof(TCHAR));
            path = buf;
            path--;
        }
#else
        // TODO: Investigate options

#endif // OS_WINDOWS
        if (!path.isValid()) path = "."; // No other option
        vio::IOManager::setExecutableDirectory(path.asCanonical());

        // Set the current working directory
        path = boost::filesystem::current_path().string();
        if (!path.isValid()) path = "."; // No other option
        if (path.isValid()) vio::IOManager::setCurrentWorkingDirectory(path.asCanonical());

#ifdef DEBUG
        printf("Executable Directory:\n    %s\n", vio::IOManager::getExecutableDirectory().getCString());
        printf("Current Working Directory:\n    %s\n\n\n", !vio::IOManager::getCurrentWorkingDirectory().isNull()
            ? vio::IOManager::getCurrentWorkingDirectory().getCString()
            : "None Specified");
#endif // DEBUG

        return InitParam::IO;
    }
    InitParam initSound() {
        return InitParam::SOUND;
    }
}

vorb::InitParam vorb::init(const InitParam& p) {
#define HAS(v, b) ((v & b) != InitParam::NONE)

    vorb::InitParam succeeded = InitParam::NONE;
    if (HAS(p, InitParam::SOUND)) succeeded |= initSound();
    if (HAS(p, InitParam::GRAPHICS)) succeeded |= initGraphics();
    if (HAS(p, InitParam::IO)) succeeded |= initIO();
    return succeeded;

#undef HAS
}
