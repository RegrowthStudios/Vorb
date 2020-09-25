//
// Merge.hpp
// Vorb Engine
//
// Created by Matthew Marshall on 25 Sept 2020
// Copyright 2020 Regrowth Studios
// MIT License
//

/*! \file Merge.hpp
 * \brief Provides an interface for merge strategies and an abstract implementation.
 */

#pragma once

#ifndef Vorb_Merge_hpp__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_Merge_hpp__
//! @endcond

#ifndef VORB_USING_PCH
#include "Vorb/types.h"
#endif // !VORB_USING_PCH

#include "Vorb/mod/DataAssetIOManager.h"

namespace vorb {
    namespace mod {
        class IMerge {
        public:
            /*!
             * \brief Reads the given file from each mod & vanilla data in as nStrings
             * and merges them in reverse priority order populating the given data.
             *
             * \param path: The relative path of the file to merge.
             * \param data: The initial data to merge into.
             *
             * \return True if at least one file was found and successfully merged, false otherwise.
             */
            virtual bool mergeFiles(const vio::Path& path, CALLER_DELETE void* data) = 0;

            /*!
             * \brief Merge the given strings in order, populating the given data.
             *
             * \param strings: The collection of strings to merge.
             * \param data: The initial data to merge into.
             *
             * \return True if the strings are successfully merged, false otherwise.
             */
            virtual bool mergeStrings(const std::vector<nString>& strings, CALLER_DELETE void* data) = 0;
            /*!
             * \brief Merge the given strings in order, populating the given data.
             *
             * \param strings: The collection of strings to merge.
             * \param data: The initial data to merge into.
             *
             * \return True if the strings are successfully merged, false otherwise.
             */
            virtual bool mergeStrings(const std::vector<cString>& strings, CALLER_DELETE void* data) = 0;

        protected:
            /*!
             * \brief The merge strategy with which to merge the data points together.
             *
             * \param newData: The new data point to merge.
             * \param existingData: The data so far merged.
             *
             * \return True if the merge completes, false otherwise.
             */
            virtual bool performMerge(const nString& newData, void* existingData) = 0;
        };

        class MergeBase : public IMerge {
        public:
            /*!
             * \brief Initialises the merge instance.
             *
             * \param ioManager: The IO manager to use to get files.
             */
            virtual void init(DataAssetIOManager* ioManager);

            /*!
             * \brief Reads the given file from each mod & vanilla data in as nStrings
             * and merges them in reverse priority order populating the given data.
             *
             * \param path: The relative path of the file to merge.
             * \param data: The initial data to merge into.
             *
             * \return True if at least one file was found and successfully merged, false otherwise.
             */
            virtual bool mergeFiles(const vio::Path& path, CALLER_DELETE void* data) override;

            /*!
             * \brief Merge the given strings in order, populating the given data.
             *
             * \param strings: The collection of strings to merge.
             * \param data: The initial data to merge into.
             *
             * \return True if the strings are successfully merged, false otherwise.
             */
            virtual bool mergeStrings(const std::vector<nString>& strings, CALLER_DELETE void* data) override;
            /*!
             * \brief Merge the given strings in order, populating the given data.
             *
             * \param strings: The collection of strings to merge.
             * \param data: The initial data to merge into.
             *
             * \return True if the strings are successfully merged, false otherwise.
             */
            virtual bool mergeStrings(const std::vector<cString>& strings, CALLER_DELETE void* data) override;

        protected:
            DataAssetIOManager* m_ioManager;
        };
    }
}
namespace vmod = vorb::mod;

#endif // !Vorb_Merge_hpp__
