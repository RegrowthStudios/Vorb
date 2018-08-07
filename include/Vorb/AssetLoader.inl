template<typename T>
void AssetLoader<T>::setContext(AssetBuilder<T>* context) {
    m_context = context;
}

template<typename T>
CALLEE_DELETE T* AssetLoader<T>::load(const nString& name, const vpath& path) {
    T* asset = nullptr;
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        auto kvp = m_assets.find(name);
        if (kvp != m_assets.end()) return kvp->second;

        asset = new T;
        m_assets[name] = asset;
        asset->name = name;
        asset->isLoaded = false;
        asset->shouldFree = false;
    }


    auto f = [=](AssetLoader<T>* loader, OUT T* asset, vpath path) {
        // Perform loading logic
        (loader->m_context)->create(path, asset, loader->m_rpc);

        {
            std::unique_lock<std::mutex> lock(loader->m_mutex);

            // Inform listeners of loading status
            asset->isLoaded = true;

            // Free if called before loaded
            if (asset->shouldFree) {
                loader->m_assets.erase(asset->name);
                delete asset;
            }
        }
    };

    { std::thread(f, this, asset, path).detach(); }

    return asset;
}

template<typename T>
void AssetLoader<T>::free(const nString& name) {
    std::unique_lock<std::mutex> lock(m_mutex);
    auto kvp = m_assets.find(name);
    if (kvp == m_assets.end()) return;

    T* asset = kvp->second;
    if (asset->isLoaded) {
        m_assets.erase(kvp);
        m_context->destroy(asset);
        delete asset;
    } else {
        asset->shouldFree = true;
    }
}
template<typename T>
void AssetLoader<T>::freeAll() {
    std::unique_lock<std::mutex> lock(m_mutex);
    for (auto kvp : m_assets) {
        m_context->destroy(kvp.second);
        delete kvp.second;
    }
    std::unordered_map<nString, T*>().swap(m_assets);
    // TODO: Make sure this is safe while objects are loading
}

template<typename T>
void AssetLoader<T>::updateGL() {
    m_rpc.processRequests(1);
}

template<typename T>
CALLEE_DELETE T* AssetLoader<T>::get(const nString& name) const {
    std::unique_lock<std::mutex> lock(m_mutex);
    auto kvp = m_assets.find(name);
    return kvp == m_assets.end() ? nullptr : kvp->second;
}
