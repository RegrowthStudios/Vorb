#include "stdafx.h"
#include "graphics/ModelIO.h"

namespace vorb {
    namespace io {
        namespace impl {
            static const cString traverseWhitespace(const cString s) {
                while (true) {
                    switch (*s) {
                    case 0:
                        return nullptr;
                    case ' ':
                    case '\t':
                        s++;
                        break;
                    default:
                        return s;
                    }
                }
            }
            static const cString traverseNonWhitespace(const cString s) {
                while (true) {
                    switch (*s) {
                    case 0:
                        return nullptr;
                    case ' ':
                    case '\t':
                        return s;
                    default:
                        s++;
                        break;
                    }
                }
            }
            static const cString moveToNewLine(const cString s) {
                bool looking = true;
                while (looking) {
                    switch (*s) {
                    case 0:
                        return nullptr;
                    case '\n':
                        looking = false;
                        s++;
                        break;
                    default:
                        s++;
                        break;
                    }
                }
                while (true) {
                    switch (*s) {
                    case 0:
                        return nullptr;
                    case ' ':
                    case '\t':
                    case '\r':
                    case '\n':
                        s++;
                        break;
                    default:
                        return s;
                    }
                }
            }
            static f32 readF32(const cString& data) {
                data = traverseWhitespace(data);
                f32 f = (f32)atof(data);
                data = traverseNonWhitespace(data);
                return f;
            }
            static f32v2 readVec2(const cString& data) {
                f32v2 ret;
                ret.x = readF32(data);
                ret.y = readF32(data);
                return ret;
            }
            static f32v3 readVec3(const cString& data) {
                f32v3 ret;
                ret.x = readF32(data);
                ret.y = readF32(data);
                ret.z = readF32(data);
                return ret;
            }
            static ui32 readNumeric(const cString& data) {
                ui32 v = 0;
                const cString initial = data;
                while (*data != 0) {
                    if (*data >= '0' && *data <= '9') {
                        v *= 10;
                        v += *data - '0';
                        data++;
                    } else {
                        return (data == initial) ? 1 : v;
                    }
                }
                return 1;
            }
            static ui32v3 readVertexIndices(const cString& data) {
                ui32v3 inds(1, 1, 1);
                data = traverseWhitespace(data);
                inds.x = readNumeric(data);
                if (*data == '/') {
                    data++;
                    inds.y = readNumeric(data);
                }
                if (*data == '/') {
                    data++;
                    inds.z = readNumeric(data);
                }
                inds -= 1;
                data = traverseNonWhitespace(data);
                return inds;
            }
            static ui32 indexOf(const ui32v3& vert, OBJMesh& mesh) {
                auto kvp = mesh.vertices.find(vert);
                if (kvp == mesh.vertices.end()) {
                    ui32 index = mesh.vertices.size();
                    mesh.vertices[vert] = index;
                    return index;
                } else {
                    return kvp->second;
                }
            }

            template<typename T>
            void readBinary(const ui8*& data, T* dst) {
                *dst = *((const T*)data);
                data += sizeof(T);
            }
        }
    }
}

ui32v2 vio::ModelIO::loadOBJ(CALLER_DELETE const cString data, OUT OBJMesh& mesh) {
    ui32v3 baseDataOffsets(
        mesh.positions.size(),
        mesh.uvs.size(),
        mesh.normals.size()
        );
    ui32 vertexCountInitial = mesh.vertices.size();
    ui32 indicesAdded = 0;

    const cString cs = data;
    cs += strspn(cs, " \t\r\n");
    while (cs) {
        switch (*cs) {
        case 0:
            cs = nullptr;
            break;
        case 'v':
            cs++;
            switch (*cs) {
            case 0:
                cs = nullptr;
                break;
            case 't':
                mesh.uvs.emplace_back(impl::readVec2(cs));
                break;
            case 'n':
                mesh.normals.emplace_back(impl::readVec3(cs));
                break;
            default:
                mesh.positions.emplace_back(impl::readVec3(cs));
                break;
            }
            break;
        case 'f': {
                      ui32v3 face;
                      ui32v3 v1 = impl::readVertexIndices(cs) + baseDataOffsets;
                      face.x = impl::indexOf(v1, mesh);
                      ui32v3 v2 = impl::readVertexIndices(cs) + baseDataOffsets;
                      face.y = impl::indexOf(v2, mesh);
                      ui32v3 v3 = impl::readVertexIndices(cs) + baseDataOffsets;
                      face.z = impl::indexOf(v3, mesh);
                      mesh.triangles.emplace_back(face);
                      indicesAdded += 3;
        }
            break;
        case '#':
            // Congrats, you used a comment in a mesh file
            break;
        default:
            break;
        }
        cs = impl::moveToNewLine(cs);
    }

    return ui32v2(mesh.vertices.size() - vertexCountInitial, indicesAdded);
}

CALLER_DELETE vg::MeshDataRaw vio::ModelIO::loadRAW(CALLER_DELETE const void* data, OUT vg::VertexDeclaration& decl, OUT size_t& indexSize) {
    const ui8* bytes = (const ui8*)data;

    // Read vertex elements
    ui32 count;
    impl::readBinary<ui32>(bytes, &count);
    ui32 vertexSize = 0;
    decl.setData(count);
    for (ui32 i = 0; i < count; i++) {
        impl::readBinary<vg::VertexElement>(bytes, &decl[i]);
        vertexSize += decl[i].componentSize * decl[i].componentCount;
    }

    // Read index size
    impl::readBinary<size_t>(bytes, &indexSize);

    vg::MeshDataRaw mesh;

    // Read vertex data
    impl::readBinary<size_t>(bytes, &mesh.vertexCount);
    size_t s = vertexSize * mesh.vertexCount;
    mesh.vertices = new ui8[s];
    memcpy(mesh.vertices, bytes, s);
    bytes += s;

    // Read index data
    impl::readBinary<size_t>(bytes, &mesh.indexCount);
    s = indexSize * mesh.indexCount;
    mesh.indices = new ui8[s];
    memcpy(mesh.indices, bytes, s);
    bytes += s;

    return mesh;
}
