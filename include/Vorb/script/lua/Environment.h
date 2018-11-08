//
// lua/Environment.h
// Vorb Engine
//
// Created by Matthew Marshall on 8 Nov 2018
// Copyright 2018 Regrowth Studios
// MIT License
//

/*! \file Environment.h
 * \brief Lua implementation of the script environment.
 */

#pragma once

#ifndef Vorb_Lua_Environment_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_Lua_Environment_h__
//! @endcond

#ifdef VORB_USING_SCRIPT

#include <lua.hpp>

#include "../IEnvironment.hpp"

namespace vorb {
    namespace script {
        namespace lua {
            class LFunction;

            using Handle = lua_State*;

            class Environment : public IEnvironment {
                using LFunctionList = std::unordered_map<nString, std::shared_ptr<LFunction>>;
            public:
                Environment();
                Environment(const std::string& filepath);
                ~Environment();

                /*!
                * \brief Load in a script from the provided filepath.
                *
                * \param filepath The filepath from which to load the script.
                */
                virtual bool load(const vio::Path& filepath) override;

                /*!
                 * \brief Set the max length each script may be.
                 *
                 * \param length The max length.
                 */
                void setMaxScriptLength(i32 length) { m_maxScriptLength = length; }

                /*!
                 * \brief Get the max length each script may be.
                 *
                 * \return The max length.
                 */
                i32 getMaxScriptLength() { return m_maxScriptLength; }

                /*!
                * \brief Add a C function as a hook with name "name" for Lua scripts to call.
                *
                * \param name The name of the C++ function to add.
                */
                // 
                void addCFunction(const std::string& name, CFunction::Type);
                // Add a C function that captures upvalueCount many upvalues - variables accessible at every call of the function.
                void addCClosure(const std::string& name, ui8 upvalueCount, CFunction::Type);
                // Add a C delegate as a hook with name "name" for Lua scripts to call.
                template<typename ReturnType, typename ...Parameters>
                void addCDelegate(const std::string& name, Delegate<ReturnType, Parameters...>&& delegate);

                // Add a Lua function to the list of registered Lua functions.
                bool addLFunction(LFunction* lFunction);
                // Subscribe a Lua callback to the named event.
                bool subscribeLFunction(std::string eventName, LFunction* lFunction, ui32 priority /*= 0*/);

                // Retrieves the named Lua function from the registered list.
                LFunction* getLFunction(std::string name);

                // Adds the given value to the top of the lua stack.
                template<typename Type>
                void addValue(const std::string& name, Type value);

                // Sets the global lua table to the top of the lua stack.
                void setGlobalNamespace();
                // Sets a series of embedded lua tables to the top of the lua stack.
                template<typename ...Namespaces>
                void setNamespaces(Namespaces... namespaces);

                // Dumps the lua stack to cout.
                void dumpStack();
            protected:
                // Recursively pushes tables onto the top of the lua stack.
                template<typename Namespace, typename ...Namespaces>
                void pushNamespaces(Namespace namespace_, Namespaces... namespaces);
                void pushNamespace(const std::string& namespace_);
                inline void pushNamespaces() {
                    // Empty
                }

                Handle        m_state;
                i32           m_namespaceDepth;
                i32           m_maxScriptLength;
                LFunctionList m_lFunctions;
            };
        }
    }
}
namespace vscript = vorb::script;

#endif // VORB_USING_SCRIPT

#endif // !Vorb_Lua_Environment_h__
