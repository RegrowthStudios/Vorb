#include "Vorb/stdafx.h"
#include "Vorb/mod/Merge.h"

#include <algorithm>

void vmod::MergeBase::init(DataAssetIOManager* ioManager) {
    m_ioManager = ioManager;
}

bool vmod::MergeBase::mergeFiles(const vio::Path& path, CALLER_DELETE void* data) {
    // We don't want to be provided a nullptr for initial data.
    assert(data != nullptr);

    std::vector<nString> strings;
    if (m_ioManager->readEachFileToString(path, strings)) {
        // Nothing to do if we received no data.
        if (strings.size() == 0) return false;

        // Need to make sure we merge in reverse-priority order.
        std::reverse(strings.begin(), strings.end());

        return mergeStrings(strings, data);
    }
    return false;
}

bool vmod::MergeBase::mergeStrings(const std::vector<nString>& strings, CALLER_DELETE void* data) {
    // We don't want to be provided a nullptr for initial data.
    assert(data != nullptr);

    bool hasSucceeded = false;
    for (auto& string : strings) {
        hasSucceeded = performMerge(string, data) || hasSucceeded;
    }

    return hasSucceeded;
}

bool vmod::MergeBase::mergeStrings(const std::vector<cString>& strings, CALLER_DELETE void* data) {
    // We don't want to be provided a nullptr for initial data.
    assert(data != nullptr);

    bool hasSucceeded = false;
    for (auto& string : strings) {
        hasSucceeded = performMerge(string, data) || hasSucceeded;
    }

    return hasSucceeded;
}
