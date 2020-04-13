#include "Vorb/stdafx.h"
#include "Vorb/mod/Mod.h"

KEG_TYPE_DEF(ModMetadata, vmod::ModMetadata, kt) {
    using namespace keg;
    kt.addValue("name",   Value::basic(offsetof(vmod::ModMetadata, name), BasicType::STRING));
    kt.addValue("author", Value::basic(offsetof(vmod::ModMetadata, author), BasicType::STRING));
    kt.addValue("version", Value::basic(offsetof(vmod::ModMetadata, version), BasicType::STRING));
    // TODO(Matthew): More metadata info.
}

KEG_TYPE_DEF(ModEntryPoints, vmod::ModEntryPoints, kt) {
    using namespace keg;
    kt.addValue("blockFiles", Value::array(offsetof(vmod::ModEntryPoints, blockFiles), BasicType::STRING));
    kt.addValue("caFiles",    Value::array(offsetof(vmod::ModEntryPoints, caFiles),    BasicType::STRING));
    kt.addValue("musicFiles", Value::array(offsetof(vmod::ModEntryPoints, musicFiles), BasicType::STRING));
    // TODO(Matthew): More entry point info.
}

bool vmod::ModBase::init(const vio::Path& installDir, const vio::Path& referenceDir){
    m_activeIOManager.setModDirectory(installDir);

    // If the active directory exists now, then the mod is installed,
    // thus it is considered to be in an active state.
    m_isInstalled = !installDir.isDirectory();

    m_installIOManager.setSearchDirectory(referenceDir);
    m_installIOManager.setSearchOnly(true);

    // We successfully init if we can load metadata of the mod.
    return loadMetadata();
}

void vmod::ModBase::dispose() {
    m_activeIOManager = ModIOManager();
    m_installIOManager = vio::IOManager();
    m_metadata = NULL_METADATA;
}

bool vmod::ModBase::loadMetadata() {
    cString modMetadata = nullptr;

    // Read the metadata in the expected location.
    if (m_isInstalled) {
        modMetadata = m_activeIOManager.readFileToString(METADATA_FILENAME);
    } else {
        modMetadata = m_installIOManager.readFileToString(METADATA_FILENAME);
    }

    // Check we got something from reading the metadata.
    if (modMetadata == nullptr) return false;

    // Attempt to parse metadata.
    if (keg::parse(&m_metadata, modMetadata, "ModMetadata") != keg::Error::NONE) return false;

    return true;
}
