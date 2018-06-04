#include "stdafx.h"
#include "graphics/ModelIO.h"

namespace vorb {
    namespace graphics {
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
                    case '\r':
                    case '\n':
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
                    ui32 index = (ui32)mesh.vertices.size();
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

ui32v2 vg::ModelIO::loadOBJ(CALLER_DELETE const cString data, OUT OBJMesh& mesh) {
    ui32v3 baseDataOffsets(
        mesh.positions.size(),
        mesh.uvs.size(),
        mesh.normals.size()
        );
    ui32 vertexCountInitial = (ui32)mesh.vertices.size();
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
                cs++;
                mesh.uvs.emplace_back(impl::readVec2(cs));
                break;
            case 'n':
                cs++;
                mesh.normals.emplace_back(impl::readVec3(cs));
                break;
            default:
                mesh.positions.emplace_back(impl::readVec3(cs));
                break;
            }
            break;
        case 'f': {
                      cs++;
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
        if (cs) cs = impl::moveToNewLine(cs);
    }

    return ui32v2((ui32)(mesh.vertices.size() - vertexCountInitial), indicesAdded);
}

CALLER_DELETE vg::MeshDataRaw vg::ModelIO::loadRAW(CALLER_DELETE const void* data, OUT vg::VertexDeclaration& decl, OUT ui32& indexSize) {
    vg::MeshDataRaw mesh = {};
    const ui8* bytes = (const ui8*)data;

    // Read the header
    char header[5];
    impl::readBinary<ui32>(bytes, (ui32*)header);
    header[4] = 0;
    if (strcmp(header, "VRAW") != 0) return mesh;

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
    impl::readBinary<ui32>(bytes, &indexSize);

    // Read vertex data
    impl::readBinary<ui32>(bytes, &mesh.vertexCount);
    ui32 s = vertexSize * mesh.vertexCount;
    mesh.vertices = new ui8[s];
    memcpy(mesh.vertices, bytes, s);
    bytes += s;

    // Read index data
    impl::readBinary<ui32>(bytes, &mesh.indexCount);
    s = indexSize * mesh.indexCount;
    mesh.indices = new ui8[s];
    memcpy(mesh.indices, bytes, s);
    bytes += s;

    return mesh;
}

CALLER_DELETE vg::Skeleton vg::ModelIO::loadAnim(CALLER_DELETE const void* data) {
    vg::Skeleton skeleton = {};
    const ui8* bytes = (const ui8*)data;

    // Read the header
    char header[5];
    impl::readBinary<ui32>(bytes, (ui32*)header);
    header[4] = 0;
    if (strcmp(header, "ANIM") != 0) return skeleton;

    { // Read number of bones
        ui32 numBones;
        impl::readBinary<ui32>(bytes, &numBones);
        skeleton.numBones = numBones;
    }

    { // Parse file data
        vg::BoneInformation* boneInfo = new vg::BoneInformation[skeleton.numBones]();
        std::unordered_map<nString, ui32> boneMap;
        for (size_t bi = 0; bi < skeleton.numBones; bi++) {
            BoneInformation& bone = boneInfo[bi];

            { // Read name
                ui32 l;
                impl::readBinary<ui32>(bytes, &l);
                bone.name.resize(l);
                memcpy(&bone.name[0], bytes, l);
                bytes += l;
                boneMap[bone.name] = (ui32)bi;
            }
            { // Read parent's name
                ui32 l;
                impl::readBinary<ui32>(bytes, &l);
                if (l != 0) {
                    bone.parent.resize(l);
                    memcpy(&bone.parent[0], bytes, l);
                    bytes += l;
                }
            }

            // Read the rest pose
            impl::readBinary<f32v4>(bytes, (f32v4*)&bone.rest.rotation);
            impl::readBinary<f32v3>(bytes, &bone.rest.translation);

            { // Read number of frames
                ui32 frames;
                impl::readBinary<ui32>(bytes, &frames);
                bone.keyframes.setData(frames);
                skeleton.numFrames += frames;
            }

            // Read all the keyframes
            for (size_t i = 0; i < bone.keyframes.size(); i++) {
                impl::readBinary<i32>(bytes, &bone.keyframes[i].frame);
                impl::readBinary<f32v4>(bytes, (f32v4*)&bone.keyframes[i].transform.rotation);
                impl::readBinary<f32v3>(bytes, &bone.keyframes[i].transform.translation);
            }
        }

        // Flatten data
        skeleton.bones = new Bone[skeleton.numBones]();
        skeleton.frames = new Keyframe[skeleton.numFrames]();

        size_t ki = 0;
        for (size_t bi = 0; bi < skeleton.numBones; bi++) {
            Bone& bone = skeleton.bones[bi];

            // Identifying information
            bone.name = boneInfo[bi].name;
            bone.index = (ui32)bi;
            bone.numChildren = 0;

            // Find parent
            bone.parent = nullptr;
            if (boneInfo[bi].parent.size() > 0) {
                auto kvp = boneMap.find(boneInfo[bi].parent);
                if (kvp != boneMap.end()) {
                    bone.parent = skeleton.bones + kvp->second;
                    skeleton.numChildrenReferences++;
                    bone.parent->numChildren++;
                }
            }

            // Rest pose
            bone.rest = boneInfo[bi].rest;

            // Copy keyframes
            bone.numFrames = boneInfo[bi].keyframes.size();
            bone.keyframes = skeleton.frames + ki;
            ki += bone.numFrames;
            memcpy(bone.keyframes, &boneInfo[bi].keyframes[0], bone.numFrames * sizeof(Keyframe));
        }
        delete[] boneInfo;
    }


    { // Make children pointers
        skeleton.childrenArray = new Bone*[skeleton.numChildrenReferences]();

        // Place initial pointer locations
        for (size_t bi = 0, ci = 0; bi < skeleton.numBones; bi++) {
            if (skeleton.bones[bi].numChildren > 0) {
                skeleton.bones[bi].children = skeleton.childrenArray + ci;
                ci += skeleton.bones[bi].numChildren;
            }
        }

        // Write children references
        for (size_t bi = 0; bi < skeleton.numBones; bi++) {
            if (skeleton.bones[bi].parent) {
                *skeleton.bones[bi].parent->children = skeleton.bones + bi;
                skeleton.bones[bi].parent->children++;
            }
        }

        // Reset pointers
        for (size_t bi = 0; bi < skeleton.numBones; bi++) {
            if (skeleton.bones[bi].numChildren > 0) {
                skeleton.bones[bi].children -= skeleton.bones[bi].numChildren;
            }
        }
    }

    return skeleton;
}
