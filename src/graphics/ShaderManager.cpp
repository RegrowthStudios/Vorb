#include "stdafx.h"
#include "graphics/ShaderManager.h"

#include "io/IOManager.h"

CALLER_DELETE vg::GLProgram* vg::ShaderManager::createProgram(const cString vertSrc, const cString fragSrc, cString defines /*= nullptr*/) {

}

CALLER_DELETE vg::GLProgram* vg::ShaderManager::createProgramFromFile(const vio::Path& vertPath, const vio::Path& fragPath, vio::IOManager* iom /*= nullptr*/, cString defines /*= nullptr*/) {

}

void vg::ShaderManager::destroyProgram(CALLEE_DELETE GLProgram** program) {

}

void vg::ShaderManager::destroyAllPrograms() {

}

bool vg::ShaderManager::registerProgram(const nString& name, GLProgram* program) {

}

CALLER_DELETE vg::GLProgram* vg::ShaderManager::unregisterProgram(const nString& name) {

}

CALLER_DELETE vg::GLProgram* vg::ShaderManager::unregisterProgram(const GLProgram* program) {

}

vg::GLProgramMap vg::ShaderManager::m_programMap;
