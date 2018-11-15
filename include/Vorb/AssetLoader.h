//
// AssetLoader.h
// Vorb Engine
//
// Created by Cristian Zaloj on 13 Feb 2015
// Ported to Vorb by Benjamin Arnold on 4 Jun 2015
// Copyright 2014 Regrowth Studios
// MIT License
//

/*! \file AssetLoader.h
 * @brief Handles loading assets.
 */

#pragma once

#ifndef Vorb_AssetLoader_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_AssetLoader_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "Vorb/types.h"
#endif // !VORB_USING_PCH

#include "Vorb/vorb_rpc.h"
#include "Vorb/graphics/GLStates.h"
#include "Vorb/graphics/ImageIO.h"
#include "Vorb/io/Path.h"
#include "Vorb/Asset.h"

#include <memory>
#include <mutex>

namespace vorb {
    namespace core {

        template<typename T> struct AssetBuilder;

        class GLRPC : public RPC {
        public:
            GLRPC(void* userData = nullptr) {
                data.userData = userData;
            }

            template<typename F>
            void set(F func) {
                RPCFunction* f = new vcore::RPCFunction(std::move(makeFunctor(func)));
                data.f = f;
                m_func.reset(f);
            }
        private:
            std::unique_ptr<RPCFunction> m_func;
        };

        /// T should derive from vcore::Asset
        template<typename T>
        class AssetLoader {
            friend struct AssetBuilder < T > ;
        public:
            void setContext(AssetBuilder<T>* context);

            CALLEE_DELETE T* get(const nString& name) const;
            CALLEE_DELETE T* load(const nString& name, const vpath& path);
            void free(const nString& name);
            void freeAll();

            void updateGL();
        private:
            std::mutex m_mutex;
            RPCManager m_rpc;
            AssetBuilder<T>* m_context = nullptr;
            std::unordered_map<nString, T*> m_assets;
        };

#define CONTEXTUAL_ASSET_LOADER_HEADER(LOADER_TYPENAME, ASSET_TYPENAME) \
class LOADER_TYPENAME : public vorb::core::AssetLoader<ASSET_TYPENAME>, public vorb::core::AssetBuilder<ASSET_TYPENAME>

#define CONTEXTUAL_ASSET_LOADER(LOADER_TYPENAME, ASSET_TYPENAME) \
CONTEXTUAL_ASSET_LOADER_HEADER(LOADER_TYPENAME, ASSET_TYPENAME) { \
public: \
    LOADER_TYPENAME() { \
        setContext(this); \
    } \
}

#include "AssetLoader.inl"

    }
}
namespace vcore = vorb::core;

#endif // !Vorb_AssetLoader_h__
