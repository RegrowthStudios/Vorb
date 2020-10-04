#include "Vorb/stdafx.h"
#include "Vorb/graphics/FontCache.h"

#include "Vorb/graphics/SpriteFont.h"
#include "Vorb/io/IOManager.h"

vg::FontCache::FontCache() :
    m_ioManager(nullptr) {

}

vg::FontCache::~FontCache() {
    dispose();
}

void vg::FontCache::init(vio::IOManagerBase* ioManager, bool ownsManager /*= false*/) {
    m_ioManager = ioManager;
    m_ownsIOManager = ownsManager;
}

void vg::FontCache::dispose() {
    for (auto& font : m_fontMap) {
        font.second->dispose();
        delete font.second;
    }
    FontMap().swap(m_fontMap);

    if (m_ownsIOManager) {
        delete m_ioManager;
    }
    m_ioManager = nullptr;
}

vg::SpriteFont* vg::FontCache::getFont(const vio::Path& filepath, ui32 size, char cs, char ce) {
    nString key = makeKey(filepath, size, cs, ce);

    SpriteFont* res = nullptr;
    try {
        res = m_fontMap.at(key);
    } catch (std::out_of_range& e) {
        res = new SpriteFont();
        res->init(filepath.getCString(), size, cs, ce);

        m_fontMap[key] = res;
    }

    return res;
}

vg::SpriteFont* vg::FontCache::getFont(const vio::Path& filepath, ui32 size) {
    return getFont(filepath, size, FIRST_PRINTABLE_CHAR, LAST_PRINTABLE_CHAR);
}

vg::SpriteFont* vg::FontCache::tryGetFont(const vio::Path& filepath, ui32 size, char cs, char ce) {
    nString key = makeKey(filepath, size, cs, ce);

    SpriteFont* res = nullptr;
    try {
        res = m_fontMap.at(key);
    } catch (std::out_of_range& e) {
        return nullptr;
    }

    return res;
}

vg::SpriteFont* vg::FontCache::tryGetFont(const vio::Path& filepath, ui32 size) {
    return tryGetFont(filepath, size, FIRST_PRINTABLE_CHAR, LAST_PRINTABLE_CHAR);
}

bool vg::FontCache::freeFont(const vio::Path& filepath, ui32 size, char cs, char ce) {
    nString key = makeKey(filepath, size, cs, ce);

    auto res = m_fontMap.find(key);

    if (res == m_fontMap.end()) return false;

    (*res).second->dispose();
    delete (*res).second;

    m_fontMap.erase(res);

    return true;
}

bool vg::FontCache::freeFont(const vio::Path& filepath, ui32 size) {
    return freeFont(filepath, size, FIRST_PRINTABLE_CHAR, LAST_PRINTABLE_CHAR);
}

nString vg::FontCache::makeKey(const vio::Path& filepath, ui32 size, char cs, char ce) {
    // Goal is to make a string that couldn't possibly mistake each section for part of another,
    // on both Windows and Linux a null-byte + asterix should cover this.
    return filepath.getString() + "\0*" + std::to_string(size) + "\0*" + cs + "\0*" + ce;
}
