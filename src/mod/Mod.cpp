#include "Vorb/stdafx.h"
#include "Vorb/mod/Mod.h"

KEG_TYPE_DEF(ModMetadata, vmod::ModMetadata, kt) {
    using namespace keg;
    kt.addValue("name",   Value::basic(offsetof(vmod::ModMetadata, name), BasicType::STRING));
    kt.addValue("author", Value::basic(offsetof(vmod::ModMetadata, name), BasicType::STRING));
    // TODO(Matthew): More metadata info.
}

vmod::Mod::Mod() :
    m_isStarted(false) {
    // Empty.
}

bool vmod::Mod::init(const nString& dir) {
    vio::Path modDir = ModIOManager::getGlobalModDirectory() / vio::Path(dir);

    // Ensure proposed mod directory exists.
    if (!modDir.isDirectory()) return false;

    m_iomanager.setModDirectory(modDir);

    // Grab mod metadata from file.
    cString modMetadata = m_iomanager.readFileToString("mod.yaml");
    if (modMetadata == nullptr) return false;

    // Attempt to parse metadata.
    if (keg::parse(&m_metadata, modMetadata, "ModMetadata") != keg::Error::NONE) return false;

    // TODO(Matthew): More initialisation.

    return true;
}

void vmod::Mod::dispose() {
    if (m_isStarted) shutdown();

    m_metadata = NULL_METADATA;
    m_iomanager.setModDirectory("");
    m_iomanager.setVanillaAssetDirectory("");
}

bool vmod::Mod::startup() {
    if (m_isStarted) return false;

    // TODO(Matthew): Implement.
}

bool vmod::Mod::shutdown() {
    if (!m_isStarted) return false;

    // TODO(Matthew): Implement.
}

void vmod::Mod::update(f32 dt/* = 0.0f*/) {
    if (!m_isStarted) return;

    // TODO(Matthew): Implement.
}
