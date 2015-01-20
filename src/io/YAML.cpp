#include "stdafx.h"
#include "io/YAML.h"

#include <yaml-cpp/yaml.h>

#include "YAMLInternal.inl"

namespace keg {
    struct YAMLNode {
    public:
        YAML::Node data;
    };
    class YAMLEmitter {
    public:
        YAML::Emitter emitter;
    };
}

void keg::NodeDocument::init(const cString data) {
    m_first = new YAMLNode;
    m_allocated.insert(m_first);
    m_first->data = YAML::Load(data);
}
void keg::NodeDocument::dispose() {
    for (auto node : m_allocated) delete node;
    std::unordered_set<YAMLNode*>().swap(m_allocated);
    m_first = nullptr;
}

void keg::NodeDocument::free(Node& node) {
    if (!node) return;
    auto place = m_allocated.find(node);
    if (place == m_allocated.end()) return;
    m_allocated.erase(place);
    delete node;
    node = nullptr;
}

void keg::NodeDocument::forAllInMap(Node node, IDelegate<const nString&, Node>* f) {
    for (auto iter : node->data) {
        Node value = new YAMLNode;
        m_allocated.insert(value);

        value->data = iter.second;
        f->invoke(this, iter.first.as<nString>(), value);
    }
}
void keg::NodeDocument::forAllInSequence(Node node, IDelegate<size_t, Node>* f) {
    size_t l = node->data.size();
    for (size_t i = 0; i < l; i++) {
        Node value = new YAMLNode;
        m_allocated.insert(value);

        value->data = node->data[i];
        f->invoke(this, i, value);
    }
}

CALLEE_DELETE keg::Node keg::NodeDocument::getInterior(Node node, const cString value) {
    Node interior = new YAMLNode;
    m_allocated.insert(interior);

    interior->data = node->data[value];
    return interior;
}

keg::NodeType keg::getType(Node node) {
    switch (node->data.Type()) {
    case YAML::NodeType::Scalar:
        return NodeType::VALUE;
    case YAML::NodeType::Map:
        return NodeType::MAP;
    case YAML::NodeType::Sequence:
        return NodeType::SEQUENCE;
    default:
        return NodeType::NONE;
    }
}
bool keg::hasInterior(Node node, const cString value) {
    return node->data[value];
}
size_t keg::getSequenceSize(Node node) {
    return node->data.size();
}

keg::Writer::Writer() :
m_emitter(new YAMLEmitter) {
    // Empty
}
keg::Writer::~Writer() {
    delete m_emitter;
}
keg::Writer& keg::Writer::push(const WriterParam& param) {
    switch (param) {
    case WriterParam::BEGIN_MAP:
        m_emitter->emitter << YAML::BeginMap;
        break;
    case WriterParam::END_MAP:
        m_emitter->emitter << YAML::EndMap;
        break;
    case WriterParam::BEGIN_SEQUENCE:
        m_emitter->emitter << YAML::BeginSeq;
        break;
    case WriterParam::END_SEQUENCE:
        m_emitter->emitter << YAML::EndSeq;
        break;
    case WriterParam::KEY:
        m_emitter->emitter << YAML::Key;
        break;
    case WriterParam::VALUE:
        m_emitter->emitter << YAML::Value;
        break;
    default:
        break;
    }
    return *this;
}
const cString keg::Writer::c_str() const {
    return m_emitter->emitter.c_str();
}

#define KEG_CONVERT_YAML_AS(TYPE) \
TYPE keg::NodeValueConverter<TYPE>::convert(Node node) { \
    return node->data.as<TYPE>(); \
} \
void keg::NodeValueConverter<TYPE>::write(YAMLEmitter* emitter, TYPE value) { \
    emitter->emitter << value; \
}
#define KEG_CONVERT_YAML_AS_NUM(TYPE) \
    KEG_CONVERT_YAML_AS(TYPE ## 8) \
    KEG_CONVERT_YAML_AS(TYPE ## 16) \
    KEG_CONVERT_YAML_AS(TYPE ## 32) \
    KEG_CONVERT_YAML_AS(TYPE ## 64) \
    KEG_CONVERT_YAML_AS(TYPE ## 8v2) \
    KEG_CONVERT_YAML_AS(TYPE ## 16v2) \
    KEG_CONVERT_YAML_AS(TYPE ## 32v2) \
    KEG_CONVERT_YAML_AS(TYPE ## 64v2) \
    KEG_CONVERT_YAML_AS(TYPE ## 8v3) \
    KEG_CONVERT_YAML_AS(TYPE ## 16v3) \
    KEG_CONVERT_YAML_AS(TYPE ## 32v3) \
    KEG_CONVERT_YAML_AS(TYPE ## 64v3) \
    KEG_CONVERT_YAML_AS(TYPE ## 8v4) \
    KEG_CONVERT_YAML_AS(TYPE ## 16v4) \
    KEG_CONVERT_YAML_AS(TYPE ## 32v4) \
    KEG_CONVERT_YAML_AS(TYPE ## 64v4)

KEG_CONVERT_YAML_AS(bool)
KEG_CONVERT_YAML_AS(cString)
KEG_CONVERT_YAML_AS(nString)
KEG_CONVERT_YAML_AS_NUM(i)
KEG_CONVERT_YAML_AS_NUM(ui)
KEG_CONVERT_YAML_AS(f32)
KEG_CONVERT_YAML_AS(f32v2)
KEG_CONVERT_YAML_AS(f32v3)
KEG_CONVERT_YAML_AS(f32v4)
KEG_CONVERT_YAML_AS(f64)
KEG_CONVERT_YAML_AS(f64v2)
KEG_CONVERT_YAML_AS(f64v3)
KEG_CONVERT_YAML_AS(f64v4)
