#include "Vorb/stdafx.h"
#include "Vorb/graphics/ShaderManager.h"
#include "Vorb/graphics/GLProgram.h"
#include "Vorb/graphics/ShaderParser.h"
#include "Vorb/io/IOManager.h"

#include <errno.h>

Event<const nString&> vg::ShaderManager::onFileIOFailure;
Event<const nString&> vg::ShaderManager::onShaderCompilationError;
Event<const nString&> vg::ShaderManager::onProgramLinkError;
vg::GLProgramMap vg::ShaderManager::m_programMap;
vg::GLProgram vg::ShaderManager::m_nilProgram;

vg::GLProgram vg::ShaderManager::createProgram(const cString vertSrc, const cString fragSrc,
                                               vio::IOManager* vertIOM /*= nullptr*/,
                                               vio::IOManager* fragIOM /*= nullptr*/,
                                               cString defines /*= nullptr*/) {
    vio::IOManager ioManager;
    // Use default ioManager
    if (!vertIOM) vertIOM = &ioManager;
    if (!fragIOM) fragIOM = &ioManager;

    std::vector<nString> attributeNames;
    std::vector<VGSemantic> semantics;
    nString parsedVertSrc;
    nString parsedFragSrc;

    // Allocate program object
    GLProgram program(true);
    program.onShaderCompilationError += makeDelegate(triggerShaderCompilationError);
    program.onProgramLinkError += makeDelegate(triggerProgramLinkError);
   
    // Parse vertex shader code
    ShaderParser::parseVertexShader(vertSrc, parsedVertSrc,
                                    attributeNames, semantics, vertIOM);

    // Create vertex shader
    ShaderSource srcVert;
    srcVert.stage = vg::ShaderType::VERTEX_SHADER;
    if (defines) srcVert.sources.push_back(defines);
    srcVert.sources.push_back(parsedVertSrc.c_str());
    if (!program.addShader(srcVert)) {
        program.dispose();
        return m_nilProgram;
    }

    // Parse fragment shader code
    ShaderParser::parseFragmentShader(fragSrc, parsedFragSrc, fragIOM);

    // Create the fragment shader
    ShaderSource srcFrag;
    srcFrag.stage = vg::ShaderType::FRAGMENT_SHADER;
    if (defines) srcFrag.sources.push_back(defines);
    srcFrag.sources.push_back(parsedFragSrc.c_str());
    if (!program.addShader(srcFrag)) {
        program.dispose();
        return m_nilProgram;
    }

    // Set the attributes
    program.setAttributes(attributeNames, semantics);
    // Link the program
    if (!program.link()) {
        program.dispose();
        return m_nilProgram;
    }
    // Set uniforms
    program.initUniforms();

    program.onShaderCompilationError -= makeDelegate(triggerShaderCompilationError);
    program.onProgramLinkError -= makeDelegate(triggerProgramLinkError);
    return program;
}

vg::GLProgram vg::ShaderManager::createProgramFromFile(const vio::Path& vertPath, const vio::Path& fragPath,
                                                       vio::IOManager* iom /*= nullptr*/, cString defines /*= nullptr*/) {
    vio::IOManager ioManager;
    vio::Path vertSearchDir;
    vio::Path fragSearchDir;

    vio::IOManager vertIOM;
    vio::IOManager fragIOM;
    if (iom) {
        vertIOM = *iom;
        fragIOM = *iom;
    } else {
        vertIOM = ioManager;
        fragIOM = ioManager;
    }

    // Set search dir to same dir as the files
    vertSearchDir = vertPath;
    fragSearchDir = fragPath;
    vertSearchDir--;
    fragSearchDir--;
    vertIOM.setSearchDirectory(vertSearchDir);
    fragIOM.setSearchDirectory(fragSearchDir);

    nString vertSrc;
    nString fragSrc;
    
    // Load in the files with error checking
    if (!vertIOM.readFileToString(vertPath, vertSrc)) {
        onFileIOFailure(nString(strerror(errno)) + " : " + vertPath.getString());
        return m_nilProgram;
    }
    if (!fragIOM.readFileToString(fragPath, fragSrc)) {
        onFileIOFailure(nString(strerror(errno)) + " : " + fragPath.getString());
        return m_nilProgram;
    }

    return createProgram(vertSrc.c_str(), fragSrc.c_str(), &vertIOM, &fragIOM, defines);
}

void vg::ShaderManager::disposeAllPrograms() {
    for (auto& it : m_programMap) {
        it.second.dispose();
    }
    GLProgramMap().swap(m_programMap);
}

bool vg::ShaderManager::registerProgram(const nString& name, const GLProgram& program) {
    auto& it = m_programMap.find(name);
    if (it != m_programMap.end()) return false;
    m_programMap[name] = program;
    return true;
}

CALLER_DELETE vg::GLProgram vg::ShaderManager::unregisterProgram(const nString& name) {
    auto& it = m_programMap.find(name);
    GLProgram rv = it->second;
    m_programMap.erase(it);
    return rv;
}

bool vg::ShaderManager::unregisterProgram(const GLProgram& program) {
    for (auto& it = m_programMap.begin(); it != m_programMap.end(); it++) {
        if (it->second.getID() == program.getID()) {
            m_programMap.erase(it);
            return true;
        }
    }
    return false;
}

vg::GLProgram& vg::ShaderManager::getProgram(const nString& name) {
    auto& it = m_programMap.find(name);
    if (it == m_programMap.end()) return m_nilProgram;
    return it->second;
}

void vg::ShaderManager::triggerShaderCompilationError(Sender s, const nString& n) {
    onShaderCompilationError(n);
}

void vg::ShaderManager::triggerProgramLinkError(Sender s, const nString& n) {
    onProgramLinkError(n);
}
