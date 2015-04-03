#include "stdafx.h"
#include "graphics/ShaderManager.h"
#include "graphics/GLProgram.h"
#include "graphics/ShaderParser.h"
#include "io/IOManager.h"

#include <errno.h>

Event<const nString&> vg::ShaderManager::onFileIOFailure;
Event<const nString&> vg::ShaderManager::onShaderCompilationError;
Event<const nString&> vg::ShaderManager::onProgramLinkError;
vg::GLProgramMap vg::ShaderManager::m_programMap;

CALLER_DELETE vg::GLProgram* vg::ShaderManager::createProgram(const cString vertSrc, const cString fragSrc,
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
    GLProgram* program = new GLProgram(true);
    program->onShaderCompilationError += makeDelegate(triggerShaderCompilationError);
    program->onProgramLinkError += makeDelegate(triggerProgramLinkError);
   
    // Parse vertex shader code
    ShaderParser::parseVertexShader(vertSrc, parsedVertSrc,
                                    attributeNames, semantics, vertIOM);

    // Create vertex shader
    ShaderSource srcVert;
    srcVert.stage = vg::ShaderType::VERTEX_SHADER;
    if (defines) srcVert.sources.push_back(defines);
    srcVert.sources.push_back(parsedVertSrc.c_str());
    if (!program->addShader(srcVert)) {
        program->dispose();
        delete program;
        return nullptr;
    }

    // Parse fragment shader code
    ShaderParser::parseFragmentShader(fragSrc, parsedFragSrc, fragIOM);

    // Create the fragment shader
    ShaderSource srcFrag;
    srcFrag.stage = vg::ShaderType::FRAGMENT_SHADER;
    if (defines) srcFrag.sources.push_back(defines);
    srcFrag.sources.push_back(parsedFragSrc.c_str());
    if (!program->addShader(srcFrag)) {
        program->dispose();
        delete program;
        return nullptr;
    }

    // Set the attributes
    program->setAttributes(attributeNames, semantics);
    // Link the program
    if (!program->link()) {
        program->dispose();
        delete program;
        return nullptr;
    }
    // Set uniforms
    program->initUniforms();

    program->onShaderCompilationError -= makeDelegate(triggerShaderCompilationError);
    program->onProgramLinkError -= makeDelegate(triggerProgramLinkError);
    return program;
}

CALLER_DELETE vg::GLProgram* vg::ShaderManager::createProgramFromFile(const vio::Path& vertPath, const vio::Path& fragPath,
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
        return nullptr;
    }
    if (!fragIOM.readFileToString(fragPath, fragSrc)) {
        onFileIOFailure(nString(strerror(errno)) + " : " + fragPath.getString());
        return nullptr;
    }

    return createProgram(vertSrc.c_str(), fragSrc.c_str(), &vertIOM, &fragIOM, defines);
}

void vg::ShaderManager::destroyProgram(CALLEE_DELETE GLProgram** program) {
    (*program)->dispose();
    delete (*program);
    (*program) = nullptr;
}

void vg::ShaderManager::destroyAllPrograms() {
    for (auto& it : m_programMap) {
        auto& program = it.second;
        program->dispose();
        delete program;
    }
    GLProgramMap().swap(m_programMap);
}

bool vg::ShaderManager::registerProgram(const nString& name, GLProgram* program) {
    auto& it = m_programMap.find(name);
    if (it != m_programMap.end()) return false;
    m_programMap[name] = program;
    return true;
}

CALLER_DELETE vg::GLProgram* vg::ShaderManager::unregisterProgram(const nString& name) {
    auto& it = m_programMap.find(name);
    GLProgram* rv = it->second;
    m_programMap.erase(it);
    return rv;
}

bool vg::ShaderManager::unregisterProgram(const GLProgram* program) {
    for (auto& it = m_programMap.begin(); it != m_programMap.end(); it++) {
        if (it->second == program) {
            m_programMap.erase(it);
            return true;
        }
    }
    return false;
}

void vg::ShaderManager::triggerShaderCompilationError(Sender s, const nString& n) {
    onShaderCompilationError(n);
}

void vg::ShaderManager::triggerProgramLinkError(Sender s, const nString& n) {
    onProgramLinkError(n);
}
