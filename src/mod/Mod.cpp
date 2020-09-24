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

vmod::ModBase::ModBase() :
    m_metadata(NULL_METADATA),
    m_isStarted(false) {
    // Empty.
}

void vmod::ModBase::init(ModMetadata metadata, const vio::Path& modDir, const DataAssetIOManager* dataAssetIOManager) {
    m_modDir = modDir;
    m_ioManager.setModDirectory(modDir);
    m_ioManager.setDataAssetIOManager(dataAssetIOManager);
    m_metadata = metadata;
}

void vmod::ModBase::dispose() {
    m_ioManager = ModIOManager();
    m_isStarted = false;
    m_metadata = NULL_METADATA;
}
