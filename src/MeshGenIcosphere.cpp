// Written by Frank McCoy
#include "stdafx.h"
#include "MeshGenerators.h"

#include "graphics/GpuMemory.h"

#include "math/VorbMath.hpp"

const static float GOLDEN_RATIO = 1.61803398875f;

const static int NUM_ICOSOHEDRON_VERTICES = 12;
const static f32v3 ICOSOHEDRON_VERTICES[12] = {
    f32v3(-1.0f, GOLDEN_RATIO, 0.0f),
    f32v3(1.0f, GOLDEN_RATIO, 0.0f),
    f32v3(-1.0f, -GOLDEN_RATIO, 0.0f),
    f32v3(1.0f, -GOLDEN_RATIO, 0.0f),

    f32v3(0.0f, -1.0f, GOLDEN_RATIO),//4
    f32v3(0.0f, 1.0f, GOLDEN_RATIO),
    f32v3(0.0f, -1.0, -GOLDEN_RATIO),
    f32v3(0.0f, 1.0f, -GOLDEN_RATIO),

    f32v3(GOLDEN_RATIO, 0.0f, -1.0f),//8
    f32v3(GOLDEN_RATIO, 0.0f, 1.0f),
    f32v3(-GOLDEN_RATIO, 0.0f, -1.0f),
    f32v3(-GOLDEN_RATIO, 0.0, 1.0f)
};

const static int NUM_ICOSOHEDRON_INDICES = 60;
const static ui32 ICOSOHEDRON_INDICES[60] = {
    0, 11, 5,
    0, 5, 1,
    0, 1, 7,
    0, 7, 10,
    0, 10, 11,

    1, 5, 9,
    5, 11, 4,
    11, 10, 2,
    10, 7, 6,
    7, 1, 8,

    3, 9, 4,
    3, 4, 2,
    3, 2, 6,
    3, 6, 8,
    3, 8, 9,

    4, 9, 5,
    2, 4, 11,
    6, 2, 10,
    8, 6, 7,
    9, 8, 1
};

class Vec3KeyFuncs {
public:
    size_t operator()(const f32v3& k)const {
        return std::hash<float>()(k.x) ^ std::hash<float>()(k.y) ^ std::hash<float>()(k.z);
    }

    bool operator()(const f32v3& a, const f32v3& b)const {
        return a.x == b.x && a.y == b.y && a.z == b.z;
    }
};

inline f32v3 findMidpoint(const f32v3& vertex1, const f32v3& vertex2) {
    return vmath::normalize(f32v3((vertex1.x + vertex2.x) / 2.0f, (vertex1.y + vertex2.y) / 2.0f, (vertex1.z + vertex2.z) / 2.0f));
}

void vmesh::generateIcosphereMesh(int lod, std::vector<ui32>& indices, std::vector<f32v3>& positions) {
    std::vector<ui32> newIndices;
    newIndices.reserve(256);

    std::unordered_map<f32v3, ui32, Vec3KeyFuncs, Vec3KeyFuncs> vertexLookup;
    
    indices.resize(NUM_ICOSOHEDRON_INDICES);
    for (ui32 i = 0; i < NUM_ICOSOHEDRON_INDICES; i++) {
        indices[i] = ICOSOHEDRON_INDICES[i];
    }
    positions.resize(NUM_ICOSOHEDRON_VERTICES);
    for (ui32 i = 0; i < NUM_ICOSOHEDRON_VERTICES; i++) {
        positions[i] = vmath::normalize(ICOSOHEDRON_VERTICES[i]);
        vertexLookup[vmath::normalize(ICOSOHEDRON_VERTICES[i])] = i;
    }

    for (ui32 i = 0; i < (ui32)lod; i++) {
        for (ui32 j = 0; j < indices.size(); j += 3) {
            /*
            j
            mp12   mp13
            j+1    mp23   j+2
            */
            //Defined in counter clockwise order
            f32v3 vertex1 = positions[indices[j + 0]];
            f32v3 vertex2 = positions[indices[j + 1]];
            f32v3 vertex3 = positions[indices[j + 2]];

            f32v3 midPoint12 = findMidpoint(vertex1, vertex2);
            f32v3 midPoint23 = findMidpoint(vertex2, vertex3);
            f32v3 midPoint13 = findMidpoint(vertex1, vertex3);

            ui32 mp12Index;
            ui32 mp23Index;
            ui32 mp13Index;

            auto iter = vertexLookup.find(midPoint12);
            if (iter != vertexLookup.end()) { //It is in the map
                mp12Index = iter->second;
            } else { //Not in the map
                mp12Index = (ui32)positions.size();
                positions.push_back(midPoint12);
                vertexLookup[midPoint12] = mp12Index;
            }

            iter = vertexLookup.find(midPoint23);
            if (iter != vertexLookup.end()) { //It is in the map
                mp23Index = iter->second;
            } else { //Not in the map
                mp23Index = (ui32)positions.size();
                positions.push_back(midPoint23);
                vertexLookup[midPoint23] = mp23Index;
            }

            iter = vertexLookup.find(midPoint13);
            if (iter != vertexLookup.end()) { //It is in the map
                mp13Index = iter->second;
            } else { //Not in the map
                mp13Index = (ui32)positions.size();
                positions.push_back(midPoint13);
                vertexLookup[midPoint13] = mp13Index;
            }

            newIndices.push_back(indices[j]);
            newIndices.push_back(mp12Index);
            newIndices.push_back(mp13Index);

            newIndices.push_back(mp12Index);
            newIndices.push_back(indices[j + 1]);
            newIndices.push_back(mp23Index);

            newIndices.push_back(mp13Index);
            newIndices.push_back(mp23Index);
            newIndices.push_back(indices[j + 2]);

            newIndices.push_back(mp12Index);
            newIndices.push_back(mp23Index);
            newIndices.push_back(mp13Index);
        }
        indices.swap(newIndices);
        newIndices.clear();
    }
}