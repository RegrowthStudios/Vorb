///
/// YAML.h
/// Vorb Engine
///
/// Created by Cristian Zaloj on 19 Jan 2015
/// Copyright 2014 Regrowth Studios
/// All Rights Reserved
///
/// Summary:
/// 
///

#pragma once

#ifndef YAML_h__
#define YAML_h__

#include "../PtrRecycler.hpp"
#include "../Events.hpp"

namespace keg {
    struct YAMLNode;
    typedef YAMLNode* Node;
    template<typename T> struct NodeValueConverter;
    class YAMLEmitter;

    enum class NodeType {
        NONE,
        VALUE,
        MAP,
        SEQUENCE
    };

    class NodeDocument {
    public:
        void init(const cString data);
        void dispose();

        CALLEE_DELETE Node getFirst() const {
            return m_first;
        }
        CALLEE_DELETE Node getInterior(Node node, const cString value);
        CALLEE_DELETE Node getInterior(Node node, const nString& value) {
            return getInterior(node, value.c_str());
        }
        void free(Node& node);

        void forAllInMap(Node node, IDelegate<const nString&, Node>* f);
        void forAllInSequence(Node node, IDelegate<size_t, Node>* f);
    private:
        Node m_first;
        std::unordered_set<Node> m_allocated;
    };

    enum class WriterParam {
        BEGIN_MAP,
        END_MAP,
        BEGIN_SEQUENCE,
        END_SEQUENCE,
        KEY,
        VALUE
    };


    class Writer {
    public:
        Writer();
        ~Writer();

        template<typename T>
        Writer& operator<< (T value) {
            NodeValueConverter<T>::write(m_emitter, value);
            return *this;
        }
        Writer& push(const WriterParam& param);

        const cString c_str() const;
    private:
        YAMLEmitter* m_emitter;
    };

    NodeType getType(Node node);
    bool hasInterior(Node node, const cString value);
    inline bool hasInterior(Node node, const nString& value) {
        return hasInterior(node, value.c_str());
    }
    size_t getSequenceSize(Node node);

    template<typename T>
    T convert(Node node) {
        return NodeValueConverter<T>::convert(node);
    }

#define KEG_NODE_CONVERTER(TYPE) \
    template<> struct NodeValueConverter<TYPE> { \
    public: \
        static TYPE convert(Node node); \
        static void write(YAMLEmitter* emitter, TYPE value); \
    }
#define KEG_NODE_CONVERTER_NUM(TYPE) \
    KEG_NODE_CONVERTER(TYPE ## 8); \
    KEG_NODE_CONVERTER(TYPE ## 16); \
    KEG_NODE_CONVERTER(TYPE ## 32); \
    KEG_NODE_CONVERTER(TYPE ## 64); \
    KEG_NODE_CONVERTER(TYPE ## 8v2); \
    KEG_NODE_CONVERTER(TYPE ## 16v2); \
    KEG_NODE_CONVERTER(TYPE ## 32v2); \
    KEG_NODE_CONVERTER(TYPE ## 64v2); \
    KEG_NODE_CONVERTER(TYPE ## 8v3); \
    KEG_NODE_CONVERTER(TYPE ## 16v3); \
    KEG_NODE_CONVERTER(TYPE ## 32v3); \
    KEG_NODE_CONVERTER(TYPE ## 64v3); \
    KEG_NODE_CONVERTER(TYPE ## 8v4); \
    KEG_NODE_CONVERTER(TYPE ## 16v4); \
    KEG_NODE_CONVERTER(TYPE ## 32v4); \
    KEG_NODE_CONVERTER(TYPE ## 64v4)

    KEG_NODE_CONVERTER_NUM(i);
    KEG_NODE_CONVERTER_NUM(ui);
    KEG_NODE_CONVERTER(f32);
    KEG_NODE_CONVERTER(f32v2);
    KEG_NODE_CONVERTER(f32v3);
    KEG_NODE_CONVERTER(f32v4);
    KEG_NODE_CONVERTER(f64);
    KEG_NODE_CONVERTER(f64v2);
    KEG_NODE_CONVERTER(f64v3);
    KEG_NODE_CONVERTER(f64v4);
    KEG_NODE_CONVERTER(bool);
    KEG_NODE_CONVERTER(nString);
    KEG_NODE_CONVERTER(cString);
}

#endif // YAML_h__
