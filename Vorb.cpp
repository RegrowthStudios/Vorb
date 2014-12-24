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
        boost::filesystem::path cwP = boost::filesystem::initial_path();

        // Set the current working directory
        nString cwPath, cwDir;
        convertWToMBString((cwString)boost::filesystem::system_complete(cwP).c_str(), cwPath);
        IOManager::getDirectory(cwPath.c_str(), cwDir);
        IOManager::setCurrentWorkingDirectory(cwDir);

        // Set the executable directory
#ifdef OS_WINDOWS
        {
            TCHAR filebuf[1024];
            GetModuleFileName(nullptr, filebuf, 1024 * sizeof(TCHAR));

            nString execDir;
            IOManager::getDirectory(filebuf, execDir);
            IOManager::setExecutableDirectory(execDir);
        }
#else
        // TODO: Investigate options
        IOManager::setExecutableDirectory(cwDir);
#endif // OS_WINDOWS

#ifdef DEBUG
        printf("Executable Directory:\n    %s\n", IOManager::getExecutableDirectory());
        printf("Current Working Directory:\n    %s\n\n\n", IOManager::getCurrentWorkingDirectory()
            ? IOManager::getCurrentWorkingDirectory()
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

    vorb::InitParam succeeded;
    if (HAS(p, InitParam::SOUND)) succeeded |= initSound();
    if (HAS(p, InitParam::GRAPHICS)) succeeded |= initGraphics();
    if (HAS(p, InitParam::IO)) succeeded |= initIO();
    return succeeded;

#undef HAS
}
