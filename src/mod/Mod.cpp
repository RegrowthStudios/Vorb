#include "Vorb/stdafx.h"
#include "Vorb/mod/Mod.h"

KEG_TYPE_DEF(ModMetadata, vmod::ModMetadata, kt) {
    using namespace keg;
    kt.addValue("name",   Value::basic(offsetof(vmod::ModMetadata, name), BasicType::STRING));
    kt.addValue("author", Value::basic(offsetof(vmod::ModMetadata, author), BasicType::STRING));
    // TODO(Matthew): More metadata info.
}

KEG_TYPE_DEF(ModEntryPoints, vmod::ModEntryPoints, kt) {
    using namespace keg;
    kt.addValue("blockFiles", Value::array(offsetof(vmod::ModEntryPoints, blockFiles), BasicType::STRING));
    kt.addValue("caFiles",    Value::array(offsetof(vmod::ModEntryPoints, caFiles),    BasicType::STRING));
    kt.addValue("musicFiles", Value::array(offsetof(vmod::ModEntryPoints, musicFiles), BasicType::STRING));
    // TODO(Matthew): More entry point info.
}


vmod::ActiveMod::ActiveMod() :
    m_isStarted(false) {
    // Empty.
}

bool vmod::ActiveMod::init(const nString& dir) {
    vio::Path modDir = ModIOManager::getGlobalModDirectory() / vio::Path(dir);

    // Ensure proposed mod directory exists.
    if (!modDir.isDirectory()) return false;

    m_iomanager.setModDirectory(modDir);

    if (!loadMetadata()) return false;

    // TODO(Matthew): More initialisation.

    return true;
}

void vmod::ActiveMod::dispose() {
    if (m_isStarted) shutdown();

    m_metadata = NULL_METADATA;
    m_iomanager.setModDirectory("");
    m_iomanager.setVanillaAssetDirectory("");
}

bool vmod::ActiveMod::startup() {
    if (m_isStarted) return false;

    // TODO(Matthew): Implement.
}

bool vmod::ActiveMod::shutdown() {
    if (!m_isStarted) return false;

    // TODO(Matthew): Implement.
}

void vmod::ActiveMod::update(f32 dt/* = 0.0f*/) {
    if (!m_isStarted) return;

    // TODO(Matthew): Implement.
}

nString vmod::ActiveMod::generateModDirName(const nString& modName) {
    return vio::Path(modName).makeNice().getString();
}

bool vmod::ActiveMod::loadMetadata() {
    // Grab mod metadata from file.
    cString modMetadata = m_iomanager.readFileToString(METADATA_FILENAME);
    if (modMetadata == nullptr) return false;

    // Attempt to parse metadata.
    if (keg::parse(&m_metadata, modMetadata, "ModMetadata") != keg::Error::NONE) return false;

    return true;
}
