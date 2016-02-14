#include "stdafx.h"
#include "macros.h"

#undef UNIT_TEST_BATCH
#define UNIT_TEST_BATCH Vorb_Graphics_Devices_

#include <include/Vorb.h>
#include <include/graphics/IAdapter.h>
#include <include/graphics/IContext.h>
#include <include/graphics/IDevice.h>

TEST(EnumerateAdapters) {
    vorb::init(vorb::InitParam::GRAPHICS);

    // Obtain a list of adapters
    std::vector<vg::APIVersion> adapterVersions;
    vg::IAdapter::listAdapterTypes(adapterVersions);

    // Print out all recognized adapters
    puts("Available adapters:\n");
    for (vg::APIVersion version : adapterVersions) {
        switch (version.api) {
        case vg::API::DIRECT_3D:
            printf("D3D       %d.%d\n", version.version.major, version.version.minor);
            break;
        case vg::API::OPENGL:
            printf("OpenGL    %d.%d\n", version.version.major, version.version.minor);
            break;
        default:
            testAssert(false, "Unknown API type: %d\n", version.api);
            break;
        }
    }

    vorb::dispose(vorb::InitParam::GRAPHICS);

    return true;
}

TEST(CreateAllAdapters) {
    vorb::init(vorb::InitParam::GRAPHICS);

    // Obtain a list of adapters
    std::vector<vg::APIVersion> adapterVersions;
    vg::IAdapter::listAdapterTypes(adapterVersions);

    // Create all recognized adapters
    puts("Available adapters:\n");
    for (vg::APIVersion version : adapterVersions) {
        // Print adapter type
        switch (version.api) {
        case vg::API::DIRECT_3D:
            printf("D3D       %d.%d\n", version.version.major, version.version.minor);
            break;
        case vg::API::OPENGL:
            printf("OpenGL    %d.%d\n", version.version.major, version.version.minor);
            break;
        default:
            testAssert(false, "Unknown API type: %d\n", version.api);
            break;
        }

        // Create the adapter
        vg::IAdapter* adapter = vg::getAdapter(version.api, version.version.major, version.version.minor);
        testAssert(adapter != nullptr, "Adapter could not be created");
        
        // Make sure the adapter actually works by creating a context
        vg::IDevice* testDefaultDevice = nullptr;
        vg::IContext* testContext = adapter->createContext(&testDefaultDevice);
        testAssert(testContext != nullptr, "Adapter could not create a context");
        testAssert(testDefaultDevice != nullptr, "Adapter does not make a default context");

        // Destroy all pieces
        testDefaultDevice->dispose();
        delete testDefaultDevice;
        testContext->dispose();
        delete testContext;
        delete adapter;
    }

    vorb::dispose(vorb::InitParam::GRAPHICS);

    return true;
}
