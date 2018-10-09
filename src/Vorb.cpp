#include "Vorb/stdafx.h"
#include "Vorb/Vorb.h"

#include <boost/filesystem.hpp>
#include <enet/enet.h>
//#include <FreeImage.h>
#if defined(VORB_IMPL_FONT_SDL)
//#if defined(VORB_OS_WINDOWS)
//#include <TTF/SDL_ttf.h>
//#else
#include <SDL_ttf/SDL_ttf.h>
//#endif
#else
// TODO: FreeType?
#endif

#include "Vorb/graphics/ConnectedTextures.h"
#include "Vorb/graphics/SpriteBatch.h"
#include "Vorb/io/IOManager.h"
#include "Vorb/sound/SoundEngine.h"
#include "Vorb/utils.h"
#include "Vorb/VorbLibs.h"
#include "Vorb/Events.hpp"

void doNothing(void*) { 
    // Empty
}
std::vector<DelegateBase::Deleter> DelegateBase::m_deleters(1, { doNothing });

namespace vorb {
    // Current system settings
    InitParam currentSettings = InitParam::NONE;
    bool isSystemInitialized(const InitParam& p) {
        return (currentSettings & p) != InitParam::NONE;
    }

    /************************************************************************/
    /* Initializers                                                         */
    /************************************************************************/
    InitParam initGraphics() {
        // Check for previous initialization
        if (isSystemInitialized(InitParam::GRAPHICS)) {
            return InitParam::GRAPHICS;
        }

#if defined(VORB_IMPL_FONT_SDL)
        if (TTF_Init() == -1) {
            printf("TTF_Init Error: %s\n", TTF_GetError());
            return InitParam::NONE;
        }
#else
        // TODO(Cristian): FreeType
#endif
//        FreeImage_Initialise();
        vg::ConnectedTextureHelper::init();
        return InitParam::GRAPHICS;
    }
    InitParam initIO() {
        // Check for previous initialization
        if (isSystemInitialized(InitParam::IO)) {
            return InitParam::IO;
        }

        // Correctly retrieve initial path
        vio::Path path = boost::filesystem::initial_path().string();

        // Set the executable directory
#ifdef VORB_OS_WINDOWS
        {
            nString buf(1024, 0);
            GetModuleFileName(nullptr, &buf[0], 1024 * sizeof(TCHAR));
            path = buf;
            path--;
        }
#else
        // TODO: Investigate options

#endif // VORB_OS_WINDOWS
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
        // Check for previous initialization
        if (isSystemInitialized(InitParam::SOUND)) {
            return InitParam::SOUND;
        }

        if (!vsound::impl::initSystem()) return InitParam::NONE;

        return InitParam::SOUND;
    }
    InitParam initNet() {
        // Check for previous initialization
        if (isSystemInitialized(InitParam::NET)) {
            return InitParam::NET;
        }

        auto err = enet_initialize();
        if (err != 0) return InitParam::NONE;
        return InitParam::NET;
    }
    
    /************************************************************************/
    /* Disposers                                                            */
    /************************************************************************/
    InitParam disposeGraphics() {
        // Check for existence
        if (!isSystemInitialized(InitParam::GRAPHICS)) {
            return InitParam::GRAPHICS;
        }

#if defined(VORB_IMPL_FONT_SDL)
        TTF_Quit();
#else
        // TODO(Cristian): FreeType
#endif
//        FreeImage_DeInitialise();
        vg::SpriteBatch::disposeProgram();

        return InitParam::GRAPHICS;
    }
    InitParam disposeIO() {
        // Check for existence
        if (!isSystemInitialized(InitParam::IO)) {
            return InitParam::IO;
        }

        return InitParam::IO;
    }
    InitParam disposeSound() {
        // Check for existence
        if (!isSystemInitialized(InitParam::SOUND)) {
            return InitParam::SOUND;
        }
        
        if (!vsound::impl::disposeSystem()) return InitParam::NONE;

        return InitParam::SOUND;
    }
    InitParam disposeNet() {
        // Check for existence
        if (!isSystemInitialized(InitParam::NET)) {
            return InitParam::NET;
        }

        enet_deinitialize();

        return InitParam::NET;
    }
}

vorb::InitParam vorb::init(const InitParam& p) {
#define HAS(v, b) ((v & b) != InitParam::NONE)

    vorb::InitParam succeeded = InitParam::NONE;
    if (HAS(p, InitParam::SOUND)) succeeded |= initSound();
    if (HAS(p, InitParam::GRAPHICS)) succeeded |= initGraphics();
    if (HAS(p, InitParam::IO)) succeeded |= initIO();
    if (HAS(p, InitParam::NET)) succeeded |= initNet();

    // Add system flags
    currentSettings |= succeeded;

    return succeeded;

#undef HAS
}
vorb::InitParam vorb::dispose(const InitParam& p) {
#define HAS(v, b) ((v & b) != InitParam::NONE)

    vorb::InitParam succeeded = InitParam::NONE;
    if (HAS(p, InitParam::SOUND)) succeeded |= disposeSound();
    if (HAS(p, InitParam::GRAPHICS)) succeeded |= disposeGraphics();
    if (HAS(p, InitParam::IO)) succeeded |= disposeIO();
    if (HAS(p, InitParam::NET)) succeeded |= disposeNet();

    // Remove system flags
    currentSettings &= (InitParam)(~(ui64)succeeded);

    return succeeded;

#undef HAS
}
