#include "stdafx.h"
#include "graphics/GLProgram.h"

#include "io/IOManager.h"

// Used for querying attribute and uniforms variables within a program
#define PROGRAM_VARIABLE_IGNORED_PREFIX "gl_"
#define PROGRAM_VARIABLE_IGNORED_PREFIX_LEN 3
#define PROGRAM_VARIABLE_MAX_LENGTH 1024

const vg::ShaderLanguageVersion vg::DEFAULT_SHADING_LANGUAGE_VERSION = vg::ShaderLanguageVersion(
    GL_PROGRAM_DEFAULT_SHADER_VERSION_MAJOR,
    GL_PROGRAM_DEFAULT_SHADER_VERSION_MINOR,
    GL_PROGRAM_DEFAULT_SHADER_VERSION_REVISION
);

VGProgram vg::GLProgram::m_programInUse = 0;

vg::GLProgram::GLProgram(bool init /*= false*/) {
    if (init) this->init();
}

void vg::GLProgram::init() {
    if (isCreated()) return;
    m_isLinked = false;
    m_id = glCreateProgram();
}
void vg::GLProgram::dispose() {
    // Delete the shaders
    if (m_idVS) {
        glDeleteShader(m_idVS);
        m_idVS = 0;
    }
    if (m_idFS) {
        glDeleteShader(m_idFS);
        m_idFS = 0;
    }

    // Delete the program
    if (m_id) {
     
        glDeleteProgram(m_id);
        m_id = 0;
        m_isLinked = false;
    }
    AttributeMap().swap(m_attributes);
    UniformMap().swap(m_uniforms);
    AttributeSemBinding().swap(m_semanticBinding);
}

bool vg::GLProgram::addShader(const ShaderSource& data) {
    // Check current state
    if (isLinked() || !isCreated()) {
        onShaderCompilationError("Cannot add a shader to a fully created or non-existent program");
        return false;
    }

    // Check for preexisting stages
    switch (data.stage) {
        case ShaderType::VERTEX_SHADER:
            if (m_idVS != 0) {
                onShaderCompilationError("Attempting to add another vertex shader");
                return false;
            }
            break;
        case ShaderType::FRAGMENT_SHADER:
            if (m_idFS != 0) {
                onShaderCompilationError("Attempting to add another fragment shader");
                return false;
            }
            break;
        default:
            onShaderCompilationError("Shader stage is not supported");
            return false;
    }

    // List of shader code
    const cString* sources = new const cString[data.sources.size() + 1];

    // Version information
    char bufVersion[32];
    sprintf(bufVersion, "#version %d%d%d\n", data.version.major, data.version.minor, data.version.revision);
    sources[0] = bufVersion;

    // Append rest of shader code
    for (size_t i = 0; i < data.sources.size(); i++) {
        sources[i + 1] = data.sources[i];
    }

    // Compile shader
    VGShader idS = glCreateShader((VGEnum)data.stage);
    glShaderSource(idS, (GLsizei)data.sources.size() + 1, sources, 0);
    glCompileShader(idS);
    delete[] sources;

    // Check status
    i32 status;
    glGetShaderiv(idS, GL_COMPILE_STATUS, &status);
    if (status != 1) {
        int infoLogLength;
        glGetShaderiv(idS, GL_INFO_LOG_LENGTH, &infoLogLength);
        std::vector<char> FragmentShaderErrorMessage(infoLogLength);
        glGetShaderInfoLog(idS, infoLogLength, NULL, FragmentShaderErrorMessage.data());
        onShaderCompilationError(FragmentShaderErrorMessage.data());
        glDeleteShader(idS);
        return false;
    }

    // Add shader to stage
    switch (data.stage) {
        case ShaderType::VERTEX_SHADER:
            m_idVS = idS;
            break;
        case ShaderType::FRAGMENT_SHADER:
            m_idFS = idS;
            break;
        default:
            break;
    }
    return true;
}
bool vg::GLProgram::addShader(const ShaderType& type, const cString code, const ShaderLanguageVersion& version /*= DEFAULT_SHADING_LANGUAGE_VERSION*/) {
    ShaderSource src;
    src.stage = type;
    src.sources.push_back(code);
    src.version = version;
    return addShader(src);
}

void vg::GLProgram::setAttribute(nString name, VGAttribute index) {
    // Adding attributes to a linked program does nothing
    if (isLinked() || !isCreated()) return;

    // Set the custom attribute
    glBindAttribLocation(m_id, index, name.c_str());
    m_attributes[name] = index;
}
void vg::GLProgram::setAttributes(const std::map<nString, VGAttribute>& attr) {
    // Adding attributes to a linked program does nothing
    if (isLinked() || !isCreated()) return;

    // Set the custom attributes
    for (auto& binding : attr) {
        glBindAttribLocation(m_id, binding.second, binding.first.c_str());
        m_attributes[binding.first] = binding.second;
    }
}
void vg::GLProgram::setAttributes(const std::vector<AttributeBinding>& attr) {
    // Adding attributes to a linked program does nothing
    if (isLinked() || !isCreated()) return;

    // Set the custom attributes
    for (auto& binding : attr) {
        glBindAttribLocation(m_id, binding.second, binding.first.c_str());
        m_attributes[binding.first] = binding.second;
    }
}
void vg::GLProgram::setAttributes(const std::vector<nString>& attr) {
    // Adding attributes to a linked program does nothing
    if (isLinked() || !isCreated()) return;

    // Set the custom attributes
    for (ui32 i = 0; i < attr.size(); i++) {
        glBindAttribLocation(m_id, i, attr[i].c_str());
        m_attributes[attr[i]] = i;
    }
}

void vg::GLProgram::setAttributes(const std::vector<nString>& attr, const std::vector<VGSemantic>& sem) {
    setAttributes(attr);
    for (int i = 0; i < (int)sem.size(); i++) {
        VGSemantic s = sem[i];
        if (s != Semantic::SEM_INVALID) {
            m_semanticBinding[s] = static_cast<VGAttribute>(i);
        }
    }
}

bool vg::GLProgram::link() {
    // Check internal state
    if (isLinked() || !isCreated()) {
        linkError("Cannot link a fully created or non-existent program");
        return false;
    }

    // Check for available shaders
    if (!m_idVS || !m_idFS) {
        linkError("Insufficient stages for a program link");
        return false;
    }

    // Link The Program
    glAttachShader(m_id, m_idVS);
    glAttachShader(m_id, m_idFS);
    glLinkProgram(m_id);

    // Detach and delete shaders
    glDetachShader(m_id, m_idVS);
    glDetachShader(m_id, m_idFS);
    glDeleteShader(m_idVS);
    glDeleteShader(m_idFS);
    m_idVS = 0;
    m_idFS = 0;

    // Check the link status
    i32 status;
    glGetProgramiv(m_id, GL_LINK_STATUS, &status);
    m_isLinked = status == 1;
    if (!m_isLinked) {
        linkError("Program had link errors");
        return false;
    }
    return true;
}

void vg::GLProgram::initAttributes() {
    if (!isLinked()) return;

    // Obtain attribute count
    i32 count;
    glGetProgramiv(m_id, GL_ACTIVE_ATTRIBUTES, &count);

    // Necessary info
    char name[PROGRAM_VARIABLE_MAX_LENGTH + 1];
    i32 len;
    GLenum type;
    i32 amount;

    // Enumerate through attributes
    for (int i = 0; i < count; i++) {
        // Get attribute info
        glGetActiveAttrib(m_id, i, PROGRAM_VARIABLE_MAX_LENGTH, &len, &amount, &type, name);
        name[len] = 0;
        VGAttribute loc = glGetAttribLocation(m_id, name);

        // Get rid of system attributes
        if (strncmp(name, PROGRAM_VARIABLE_IGNORED_PREFIX, PROGRAM_VARIABLE_IGNORED_PREFIX_LEN) != 0 && loc != -1) {
            m_attributes[name] = loc;
        }
    }
}
void vg::GLProgram::initUniforms() {
    if (!isLinked()) return;

    // Obtain uniform count
    i32 count;
    glGetProgramiv(m_id, GL_ACTIVE_UNIFORMS, &count);

    // Necessary info
    char name[PROGRAM_VARIABLE_MAX_LENGTH + 1];
    i32 len;
    GLenum type;
    i32 amount;

    // Enumerate through uniforms
    for (int i = 0; i < count; i++) {
        // Get uniform info
        glGetActiveUniform(m_id, i, PROGRAM_VARIABLE_MAX_LENGTH, &len, &amount, &type, name);
        name[len] = 0;
        VGUniform loc = glGetUniformLocation(m_id, name);

        // Get rid of system uniforms
        if (strncmp(name, PROGRAM_VARIABLE_IGNORED_PREFIX, PROGRAM_VARIABLE_IGNORED_PREFIX_LEN) != 0 && loc != -1) {
            m_uniforms[name] = loc;
        }
    }
}

void vg::GLProgram::bindFragDataLocation(ui32 colorNumber, const char* name) {
    glBindAttribLocation(m_id, colorNumber, name);
}

void vg::GLProgram::enableVertexAttribArrays() const {
    for (auto& attrBind : m_attributes) {
        glEnableVertexAttribArray(attrBind.second);
    }
}

void vg::GLProgram::disableVertexAttribArrays() const {
    for (auto& attrBind : m_attributes) {
        glDisableVertexAttribArray(attrBind.second);
    }
}

void vg::GLProgram::use() {
    if (!isInUse()) {
        m_programInUse = m_id;
        glUseProgram(m_id);
    }
}

void vg::GLProgram::unuse() {
    if (m_programInUse) {
        m_programInUse = 0;
        glUseProgram(0);
    }
}

void vg::GLProgram::linkError(const nString& s) {
    char buf[256];
    GLsizei len;
    glGetProgramInfoLog(getID(), 255, &len, buf);
    buf[len] = 0;

    nString s2 = s + ": " + buf;
    onProgramLinkError(s2);
}
