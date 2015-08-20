#include "stdafx.h"
#include "io/YAML.h"

#include "YAMLImpl.h"

void keg::YAMLReader::init(const cString data) {
    m_first = new YAMLNode;
    m_allocated.insert(m_first);
    m_first->data = YAML::Load(data);
}
void keg::YAMLReader::dispose() {
    for (auto node : m_allocated) delete node;
    std::unordered_set<YAMLNode*>().swap(m_allocated);
    m_first = nullptr;
}

void keg::YAMLReader::free(Node& node) {
    if (!node) return;
    auto place = m_allocated.find(node);
    if (place == m_allocated.end()) return;
    m_allocated.erase(place);
    delete node;
    node = nullptr;
}

void keg::YAMLReader::forAllInMap(Node node, Delegate<Sender, const nString&, Node>* f) {
    for (auto iter : node->data) {
        Node value = new YAMLNode;
        m_allocated.insert(value);

        value->data = iter.second;
        f->invoke(this, iter.first.as<nString>(), value);
    }
}
void keg::YAMLReader::forAllInSequence(Node node, Delegate<Sender, size_t, Node>* f) {
    size_t l = node->data.size();
    for (size_t i = 0; i < l; i++) {
        Node value = new YAMLNode;
        m_allocated.insert(value);

        value->data = node->data[i];
        f->invoke(this, i, value);
    }
}

CALLEE_DELETE keg::Node keg::YAMLReader::getInterior(Node node, const cString value) {
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
    return (bool)node->data[value];
}
size_t keg::getSequenceSize(Node node) {
    return node->data.size();
}

keg::YAMLWriter::YAMLWriter() :
m_emitter(new YAMLEmitter) {
    // Empty
}
keg::YAMLWriter::~YAMLWriter() {
    delete m_emitter;
}
keg::YAMLWriter& keg::YAMLWriter::push(const WriterParam& param) {
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
const cString keg::YAMLWriter::c_str() const {
    return m_emitter->emitter.c_str();
}
