#include "Vorb/stdafx.h"
#include "Vorb/graphics/ShaderParser.h"

#include <sstream>

#include "Vorb/io/IOManager.h"

// Static definitions
Event<nString> vg::ShaderParser::onParseError;
std::map<nString, vg::Semantic> vg::ShaderParser::m_semantics;
std::set<nString> vg::ShaderParser::m_parsedIncludes;
bool vg::ShaderParser::isNormalComment = false;
bool vg::ShaderParser::isBlockComment = false;
vio::IOManager* vg::ShaderParser::ioManager;

// Checks if c is a whitespace char
inline bool isWhitespace(char c) {
    return (c == '\n' || c == '\0' || c == '\t' || c == '\r' || c == ' ');
}
// Skips all whitespace by incrementing i accordingly
inline void skipWhitespace(const nString& s, size_t& i) {
    while (isWhitespace(s[i]) && i < s.size()) i++;
}
// Checks if c is a number
inline bool isNumeric(char c) {
    return (c >= '0' && c <= '9');
}

void vg::ShaderParser::parseVertexShader(const cString inputCode, OUT nString& resultCode,
                                         OUT std::vector<nString>& attributeNames,
                                         OUT std::vector<VGSemantic>& semantics,
                                         vio::IOManager* iom /*= nullptr*/) {
    if (m_semantics.empty()) initSemantics();
    isNormalComment = false;
    isBlockComment = false;
    m_parsedIncludes.clear();

    // Convert to nString for easy include replacements
    nString input(inputCode);

    vio::IOManager tmpIoManager;
    if (!iom) iom = &tmpIoManager;
    ioManager = iom;

    nString data;

    // Anticipate final code size
    resultCode = "";
    resultCode.reserve(input.size());

    for (size_t i = 0; i < input.size(); i++) {
        char c = input[i];
        checkForComment(input.c_str(), i);
        if (!isComment()) {
            if (c == '#') {
                if (tryParseInclude(input, i)) {
                    i--;
                    continue;
                }
            } else if (c == 'i') {
                // Attempt to parse as an attribute
                VGSemantic semantic;
                nString attribute = tryParseAttribute(input.c_str(), i, semantic);
                if (attribute.size()) {
                    attributeNames.push_back(attribute);
                    semantics.push_back(semantic);
                }
            }
        }
        resultCode += c;
    }
}

void vg::ShaderParser::parseFragmentShader(const cString inputCode, OUT nString& resultCode, vio::IOManager* iom /*= nullptr*/) {
    if (m_semantics.empty()) initSemantics();
    isNormalComment = false;
    isBlockComment = false;
    m_parsedIncludes.clear();

    // Convert to nString for easy include replacements
    nString input(inputCode);

    vio::IOManager tmpIoManager;
    if (!iom) iom = &tmpIoManager;
    ioManager = iom;

    nString data;

    // Anticipate final code size
    resultCode = "";
    resultCode.reserve(input.size());

    for (size_t i = 0; i < input.size(); i++) {
        char c = input[i];
        checkForComment(input.c_str(), i);
        if (!isComment() && c == '#') {
            if (tryParseInclude(input, i)) {
                i--;
                continue;
            }
        } 
        resultCode += c;
    }
}

void vorb::graphics::ShaderParser::initSemantics() {
    m_semantics["COLOR"] = SEM_COLOR;
    m_semantics["POSITION"] = SEM_POSITION;
    m_semantics["TEXCOORD"] = SEM_TEXCOORD;
    m_semantics["NORMAL"] = SEM_NORMAL;
    m_semantics["BINORMAL"] = SEM_BINORMAL;
    m_semantics["TANGENT"] = SEM_TANGENT;
    m_semantics["FOG"] = SEM_FOG;
    m_semantics["BLENDINDICES"] = SEM_BLENDINDICES;
    m_semantics["BLENDWEIGHT"] = SEM_BLENDWEIGHT;
    m_semantics["PSIZE"] = SEM_PSIZE;
    m_semantics["TESSFACTOR"] = SEM_TESSFACTOR;
}

bool vg::ShaderParser::checkForComment(const cString s, size_t i) {
    if (s[i] == '/' && s[i + 1] == '/') {
        isNormalComment = true;
        return true;
    } else if (s[i] == '\n') {
        isNormalComment = false;
    } else if (s[i] == '/' && s[i + 1] == '*') {
        isBlockComment = true;
        return true;
    } else if (s[i] == '*' && s[i + 1] == '/') {
        isBlockComment = false;
    }
    return false;
}

bool vg::ShaderParser::tryParseInclude(nString& s, size_t i) {
    size_t startI = i;
    static const char INCLUDE_STR[10] = "#include";
    // Check that #include is correct
    for (int j = 0; INCLUDE_STR[j] != '\0'; j++) {
        if (s[i] == '\0') { return false; }
        if (s[i++] != INCLUDE_STR[j]) { return false; }
    }

    skipWhitespace(s, i);
    if (s[i] == '\0') { return false; }

    if (s[i++] != '\"') { return false; }
    // Grab the include string
    nString include = "";
    while (s[i] != '\"' && s[i] != '\n') {
        // Check for invalid characters in path
        if (isWhitespace(s[i])) { return false; }
        include += s[i++];
    }
    if (s[i] != '\"') return false;

    if (include.size()) {

        if (m_parsedIncludes.find(include) != m_parsedIncludes.end()) {
            onParseError("Circular include detected: " + include);
            return false;
        }
        m_parsedIncludes.insert(include);
        // Replace the include with the file contents
        nString data = "";
        if (ioManager->readFileToString(include, data)) {
            if (data.empty()) {
                s.erase(startI, i + 1 - startI);
            } else {
                while (data.back() == '\0') data.pop_back();
                // Replace by erasing and inserting
                s.erase(startI, i + 1 - startI);
                if (data.length()) s.insert(startI, data.c_str());
            }
            return true;
        } else {
            onParseError("Failed to open file " + include);
            return false;
        }
    }
    return false;
}

nString vg::ShaderParser::tryParseAttribute(const cString s, size_t i, OUT VGSemantic& semantic) {
    static const char IN_STR[4] = "in ";
    static const char SEM_STR[5] = "SEM ";
    semantic = vg::Semantic::SEM_INVALID;
    // Check that in is correct
    for (int j = 0; IN_STR[j] != '\0'; j++) {
        if (s[i] == '\0') return "";
        if (s[i++] != IN_STR[j]) return "";
    }

    skipWhitespace(s, i);
    if (s[i] == '\0') return "";

    // Skip the type
    while (!isWhitespace(s[i])) {
        // Check for null character
        if (s[i] == '\0') return "";
        i++;
    }

    skipWhitespace(s, i);
    if (s[i] == '\0') return "";

    // Read the name
    nString name = "";
    while (s[i] != ';' && !isWhitespace(s[i])) {
        // Check for null character
        if (s[i] == '\0') return "";
        name += s[i++];
    }

    // Now to look for semantic comment

    skipWhitespace(s, i);
    if (s[i] == '\0') return name;

    // Check for comment
    if (s[i++] != '/') return name;
    if (s[i] == '\0') return name;
    if (s[i++] != '/') return name;
    if (s[i] == '\0') return name;

    skipWhitespace(s, i);
    if (s[i] == '\0') return name;

    // Check for SEM
    for (int j = 0; SEM_STR[j] != '\0'; j++) {
        if (s[i] == '\0') return name;
        if (s[i++] != SEM_STR[j]) return name;
    }

    skipWhitespace(s, i);
    if (s[i] == '\0') return name;

    // Read the semantic
    nString semanticName = "";
    while (!isWhitespace(s[i]) && !isNumeric(s[i])) {
        // Check for null character
        if (s[i] == '\0') return name;
        semanticName += s[i++];
    }
    auto& it = m_semantics.find(semanticName);
    if (it == m_semantics.end()) return name;

    // Get the number
    nString numberString = "";
    for (int j = 0; s[i] != '\0' && isNumeric(s[i]); j++) {
        numberString += s[i];
        i++;
    }
    int number = 0;
    if (numberString.size()) {
        std::istringstream(numberString) >> number;
    }

    // Calculate the semantic ID
    semantic = (VGSemantic)it->second + number * Semantic::SEM_NUM_SEMANTICS + 1;
    return name;
}
