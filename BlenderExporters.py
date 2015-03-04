"""
TODO:

Vertex Color
Tangent Space
V Flip

Normalize Bone Weights

Specify Axis Change

"""

import bpy
import struct
from enum import Enum

class VertexAttributeUsage(Enum):
    POSITION = 0
    TEXTURE_COORDINATE = 1
    NORMAL = 2
    COLOR = 3
    BINORMAL = 4
    TANGENT = 5
    BONE_WEIGHTS = 6
    BONE_INDICES = 7
    CUSTOM = 8
class VertexElementFlags(Enum):
    NONE = 0x00
    FLOAT = 0x01
    UNSIGNED = 0x02
    NORMALIZED = 0x04
class VertexElement:
    def __init__(self, 
        componentSize = 4, componentCount = 3,
        usage = VertexAttributeUsage.POSITION, index = 0,
        flags = VertexElementFlags.FLOAT, offset = 0):
        # Fill all vertex element data
        self.offset = offset
        self.componentSize = componentSize
        self.componentCount = componentCount
        self.usage = usage
        self.index = index
        self.flags = flags
    
class MeshExportSettings:
    def __init__(self):
        self.exportNormals = False
        self.exportTextures = False
        self.exportTangentSpace = False
        self.exportVFlipped = False
        self.exportBoneWeights = False
        self.exportBinary = True
    
def getVertexElementsAndSize(mesh, settings):
    # Find all the vertex elements in the mesh (position if always a given)
    vertexElements = [VertexElement(4, 3, VertexAttributeUsage.POSITION, 0, VertexElementFlags.FLOAT, 0)]
    
    # Exporting normals is an option
    if settings.exportNormals:
        vertexElements.append(VertexElement(4, 3, VertexAttributeUsage.NORMAL, 0, VertexElementFlags.FLOAT, 0))
    
    # Export textures if they exist and are desired
    if settings.exportTextures:
        for i, uvl in enumerate(mesh.uv_layers):
            vertexElements.append(VertexElement(4, 2, VertexAttributeUsage.TEXTURE_COORDINATE, i, VertexElementFlags.FLOAT, 0))
            
        # Export tangent space for the first texture
        if settings.exportTangentSpace and (len(mesh.uv_layers) > 0):
            vertexElements.append(VertexElement(4, 3, VertexAttributeUsage.TANGENT, 0, VertexElementFlags.FLOAT, 0))
            vertexElements.append(VertexElement(4, 3, VertexAttributeUsage.BINORMAL, 0, VertexElementFlags.FLOAT, 0))
    
    # Export animation information (bone weights and indices)
    if settings.exportBoneWeights:
        vertexElements.append(VertexElement(4, 4, VertexAttributeUsage.BONE_WEIGHTS, 0, VertexElementFlags.FLOAT, 0))
        vertexElements.append(VertexElement(2, 4, VertexAttributeUsage.BONE_INDICES, 0, VertexElementFlags.UNSIGNED, 0))
    
    # Calculate vertex struct size and place offsets
    vertexSize = 0
    for element in vertexElements:
        element.offset = vertexSize
        vertexSize += element.componentSize * element.componentCount
        
    return (vertexElements, vertexSize)
    
def getVertexIndex(map, elementInds):
    key = tuple(elementInds)
    if key in map:
        return map[key]
    else:
        idx = len(map)
        map[key] = idx
        return idx
def getUnique(map, key):
    if key in map:
        return map[key]
    else:
        idx = len(map)
        map[key] = idx
        return idx
        
def getTris(loops):
    inds = [loops[0], loops[1], loops[2]]
    yield tuple(inds)
    i = 3
    while i < len(loops):
        inds[1] = inds[2]
        inds[2] = loops[i]
        i += 1
        yield tuple(inds)
        
def insertWeighting(l, w):
    for i, w2 in enumerate(l):
        if w[1] > w2[1]:
            return l[:i] + [w] + l[i:3]
    return l
    
def getAllUniqueVertexIndices(mesh, ne, settings):
    def vec2key(v):
        return (round(v[0], 4), round(v[1], 4))

    # Return data
    vd = {}
    uvDicts = [{} for uvl in mesh.uv_layers] if settings.exportTextures else []
    animL = [[(0,0.0), (0,0.0), (0,0.0), (0,0.0)] for vertex in mesh.vertices] if settings.exportBoneWeights else []
    tris = []

    # Used as a key for vertex information
    index = [0] * ne
    
    # Loop through all the triangles
    ti = 0
    for polyIndex, poly in enumerate(mesh.polygons):
        for tri in getTris(poly.loop_indices):
        
            # Loop through triangle vertices 
            inds = [0, 0, 0]
            for vti, vert in enumerate(tri):
                vi = mesh.loops[vert].vertex_index
            
                ii = 0
                
                # Position index
                index[ii] = vi
                ii += 1
                
                # Normal index
                if settings.exportNormals:
                    normIndex = vi if poly.use_smooth else -(polyIndex + 1)
                    index[ii] = normIndex
                    ii += 1
                    
                # UV index
                if settings.exportTextures:
                    for i, uvl in enumerate(mesh.uv_layers):
                        uvk = vec2key(uvl.data[vert].uv)
                        index[ii] = getUnique(uvDicts[i], uvk)
                        ii += 1
                        
                    # Export tangent space for the first texture
                    if settings.exportTangentSpace and (len(mesh.uv_layers) > 0):
                        index[ii] = ti # TODO: Fix this
                        ii += 1
                
                # Animation index
                if settings.exportBoneWeights:
                    # Bone information is unique to the vertex
                    index[ii] = vi
                    ii += 1
                    index[ii] = vi
                    ii += 1
                
                inds[vti] = getVertexIndex(vd, index)
            
            # Add a triangle
            tris.append(tuple(inds))
            ti += 1

    # Compute bone weight information here
    if settings.exportBoneWeights:
        for vertex in mesh.vertices:
            for group in vertex.groups:
                if group.weight > 0:
                    animL[vertex.index] = insertWeighting(animL[vertex.index], (group.group, group.weight))
    
    # Convert to list form
    vl = [None] * len(vd)
    uvl = [[None] * len(uvd) for uvd in uvDicts]
    for key, val in vd.items():
        vl[val] = key
    for i, uvd in enumerate(uvDicts):
        for key, val in uvd.items():
            uvl[i][val] = key
    
    return (vl, uvl, animL, tris)
    
def writeMesh(context, filepath, settings):
    # Grab mesh for export
    mesh = context.object.data
    
    # Get vertex element information
    vertexElements, vertexSize = getVertexElementsAndSize(mesh, settings)
    numElements = len(vertexElements)
    
    # Get mesh data
    vList, uvLists, animL, tris = getAllUniqueVertexIndices(mesh, len(vertexElements), settings)
    
    if settings.exportBinary:
        # Open the file in binary mode
        f = open(filepath, 'wb')
        
        # Write header
        f.write(struct.pack('<4s', bytes('VRAW','utf-8')))
        
        # Write vertex elements
        f.write(struct.pack('<I', len(vertexElements)))
        for element in vertexElements:
            f.write(struct.pack('<HBBBB', 
                element.offset,
                element.componentSize,
                element.componentCount,
                (element.index << 4) | element.usage.value,
                element.flags.value
                ))
        
        # Write index size
        f.write(struct.pack('<I', 4))
                
        # Write vertex data
        f.write(struct.pack('<I', len(vList)))
        for vertKey in vList:
            for i, ve in enumerate(vertexElements):
                if ve.usage == VertexAttributeUsage.POSITION:
                    pos = mesh.vertices[vertKey[i]].co
                    f.write(struct.pack('<fff', pos[0], pos[1], pos[2]))
                elif ve.usage == VertexAttributeUsage.NORMAL:
                    ni = vertKey[i]
                    normal = mesh.vertices[ni].normal if ni >= 0 else mesh.polygons[-ni - 1].normal
                    f.write(struct.pack('<fff', normal[0], normal[1], normal[2]))
                elif ve.usage == VertexAttributeUsage.TEXTURE_COORDINATE:
                    uv = uvLists[ve.index][vertKey[i]]
                    f.write(struct.pack('<ff', uv[0], uv[1]))
                elif ve.usage == VertexAttributeUsage.BONE_WEIGHTS:
                    weights = animL[vertKey[i]]
                    f.write(struct.pack('<ffff', weights[0][1], weights[1][1], weights[2][1], weights[3][1]))
                elif ve.usage == VertexAttributeUsage.BONE_INDICES:
                    weights = animL[vertKey[i]]
                    f.write(struct.pack('<HHHH', weights[0][0], weights[1][0], weights[2][0], weights[3][0]))
        
        # Write index data
        f.write(struct.pack('<I', len(tris) * 3))
        for tri in tris:
            f.write(struct.pack('<III', tri[0], tri[1], tri[2]))
        
        # Close file
        f.close()
    else:
        # Open the file in text mode with added YAML extension
        f = open(filepath + '.yml', 'w', encoding='utf-8')
        
        # Write vertex elements
        f.write("VertexElements:\n")
        for element in vertexElements:
            f.write('  - Offset:     %d\n' % (element.offset))
            f.write('    CompSize:   %d\n' % (element.componentSize))
            f.write('    CompCount:  %d\n' % (element.componentCount))
            f.write('    UsageType:  %s\n' % (element.usage.name))
            f.write('    UsageIndex: %d\n' % (element.index))
            f.write('    Flags:      %d\n' % (element.flags.value))
        
        # Write vertex data
        f.write("Vertices:\n")
        for vertKey in vList:
            for i, ve in enumerate(vertexElements):
                f.write('  - ' if i == 0 else '    ')
                if ve.usage == VertexAttributeUsage.POSITION:
                    pos = mesh.vertices[vertKey[i]].co
                    f.write('Position%d: [%f,%f,%f]\n' % (ve.index, pos[0], pos[1], pos[2]))
                elif ve.usage == VertexAttributeUsage.NORMAL:
                    ni = vertKey[i]
                    normal = mesh.vertices[ni].normal if ni >= 0 else mesh.polygons[-ni - 1].normal
                    f.write('Normal%d:   [%f,%f,%f]\n' % (ve.index, normal[0], normal[1], normal[2]))
                elif ve.usage == VertexAttributeUsage.TEXTURE_COORDINATE:
                    uv = uvLists[ve.index][vertKey[i]]
                    f.write('TexCoord%d: [%f,%f]\n' % (ve.index, uv[0], uv[1]))
                elif ve.usage == VertexAttributeUsage.BONE_WEIGHTS:
                    weights = animL[vertKey[i]]
                    f.write('BWeights%d: [%f,%f,%f,%f]\n' % (ve.index, weights[0][1], weights[1][1], weights[2][1], weights[3][1]))
                elif ve.usage == VertexAttributeUsage.BONE_INDICES:
                    weights = animL[vertKey[i]]
                    f.write('BIndices%d: [%d,%d,%d,%d]\n' % (ve.index, weights[0][0], weights[1][0], weights[2][0], weights[3][0]))
        
        # Write index data
        f.write("Triangles:\n")
        for tri in tris:
            f.write('  - [%d,%d,%d]\n' % (tri[0], tri[1], tri[2]))
        
        # Close file
        f.close()

    return {'FINISHED'}

def getKeyframes(context, object):
    # Deselect all bones
    for bone in object.data.bones:
        bone.select = False
        
    keyframes = [[] for bone in object.data.bones]
    for i, bone in enumerate(object.data.bones):
        bone.select = True
        bpy.ops.screen.frame_jump(0)
        context.scene.update()
        context.scene.frame_set(context.scene.frame_current)
        keyframes[i].append(context.scene.frame_current)
        while {'FINISHED'} == bpy.ops.screen.keyframe_jump():
            context.scene.update()
            context.scene.frame_set(context.scene.frame_current)
            keyframes[i].append(context.scene.frame_current)
        bone.select = False
    return keyframes
    
    
def writeSkeleton(context, filepath, exportBinary):
    # Get the armature
    object = bpy.context.object

    # TODO: Make sure we're not in POSE mode
    if object.mode != 'POSE':
        bpy.ops.object.posemode_toggle()

    # Obtain the keyframes
    keyframes = getKeyframes(context, object)

    if exportBinary:
        # Open the file in binary mode
        f = open(filepath, 'wb')
        
        # Write header
        f.write(b'ANIM')
        
        # Write all the bones
        f.write(struct.pack('<I', len(object.pose.bones)))
        for boneIndex, bone in enumerate(object.pose.bones):
            # Write bone name and parent
            f.write(struct.pack('<I', len(bone.name)))
            f.write(bytes(bone.name, 'ASCII'))
            if bone.parent:
                f.write(struct.pack('<I', len(bone.parent.name)))
                f.write(bytes(bone.parent.name, 'ASCII'))
            else:
                f.write(struct.pack('<I', 0))
                
            # Write bone rest pose
            object.data.pose_position = 'REST'
            context.scene.update()
            rotation = bone.rotation_quaternion
            f.write(struct.pack('<ffff', rotation[1], rotation[2], rotation[3], rotation[0]))
            matrix = bone.matrix
            f.write(struct.pack('<fff', matrix[0][3], matrix[1][3], matrix[2][3]))
            object.data.pose_position = 'POSE'
            context.scene.update()
            
            # Write all keyframes for the bone
            f.write(struct.pack('<I', len(keyframes[boneIndex])))
            for frame in keyframes[boneIndex]:
                # Move to animation frame
                bpy.context.scene.frame_set(frame)
                bpy.context.scene.update()
                
                # Write frame and pose matrix
                f.write(struct.pack('<i', frame))
                rotation = bone.rotation_quaternion
                f.write(struct.pack('<ffff', rotation[1], rotation[2], rotation[3], rotation[0])) # WXYZ
                matrix = bone.matrix
                f.write(struct.pack('<fff', matrix[0][3], matrix[1][3], matrix[2][3]))
            
        f.close()
    else:
        # Open the file in text mode with added YAML extension
        f = open(filepath + '.yml', 'w', encoding='utf-8')
        
        # Write all the bones
        f.write("Bones:\n");
        for boneIndex, bone in enumerate(object.pose.bones):
            # Write bone name and parent
            f.write("  - Name:   " + bone.name + "\n")
            if bone.parent:
                f.write("    Parent: " + bone.parent.name + "\n")
                
            # Write bone rest pose
            object.data.pose_position = 'REST'
            context.scene.update()
            f.write("    Rest:\n")
            rotation = bone.rotation_quaternion
            f.write("      Rotation:    [%f, %f, %f, %f]\n" % (rotation[1], rotation[2], rotation[3], rotation[0]))
            matrix = bone.matrix
            f.write("      Translation: [%f, %f, %f]\n" % (matrix[0][3], matrix[1][3], matrix[2][3]))
            object.data.pose_position = 'POSE'
            context.scene.update()
            
            # Write all keyframes for the bone
            f.write("    Frames:\n")
            for frame in keyframes[boneIndex]:
                # Move to animation frame
                bpy.context.scene.frame_set(frame)
                bpy.context.scene.update()
                
                # Write frame and pose matrix
                f.write("      - Frame:       %d\n" % (frame))
                rotation = bone.rotation_quaternion
                f.write("        Rotation:    [%f, %f, %f, %f]\n" % (rotation[1], rotation[2], rotation[3], rotation[0]))
                matrix = bone.matrix
                f.write("        Translation: [%f, %f, %f]\n" % (matrix[0][3], matrix[1][3], matrix[2][3]))
            
        f.close()
    
    return {'FINISHED'}
    
# ExportHelper is a helper class, defines filename and
# invoke() function which calls the file selector.
from bpy_extras.io_utils import ExportHelper
from bpy.props import StringProperty, BoolProperty, EnumProperty
from bpy.types import Operator

class VRAWMeshExporter(Operator, ExportHelper):
    """Export active mesh with selective information"""
    bl_idname = "export_mesh.vraw"  # important since its how bpy.ops.import_test.some_data is constructed
    bl_label = "VRAW Mesh Exporter"

    # ExportHelper mixin class uses this
    filename_ext = ".vraw"
    filter_glob = StringProperty(
            default="*.vraw",
            options={'HIDDEN'},
            )

    # List of operator properties, the attributes will be assigned
    # to the class instance from the operator settings before calling.
    settings_exportNormals = BoolProperty(
        name="Export Normals",
        description="Export Normal Information",
        default=False,
        )
    settings_exportTextures = BoolProperty(
        name="Export UV Layers",
        description="Export UV Mapping Information",
        default=False,
        )
    settings_exportTangentSpace = BoolProperty(
        name="Export Tangents",
        description="Export Tangent Space Vectors For First UV Layer",
        default=False,
        )
    settings_exportBoneWeights = BoolProperty(
        name="Export Bone Weights",
        description="Export Bone Weights And Indices",
        default=False,
        )
    settings_exportVFlipped = BoolProperty(
        name="Flip Texture V",
        description="Flip V Coordinates (1-V) In all UV layers",
        default=False,
        )
    settings_exportBinary = BoolProperty(
        name="Binary",
        description="Export Data In The Binary Format",
        default=True,
        )

    def execute(self, context):
        settings = MeshExportSettings()
        settings.exportNormals = self.settings_exportNormals
        settings.exportTextures = self.settings_exportTextures
        settings.exportTangentSpace = self.settings_exportTangentSpace
        settings.exportVFlipped = self.settings_exportVFlipped
        settings.exportBinary = self.settings_exportBinary
        settings.exportBoneWeights = self.settings_exportBoneWeights
        return writeMesh(context, self.filepath, settings)
        
class AnimationExporter(Operator, ExportHelper):
    """Export active mesh with selective information"""
    bl_idname = "export_animation.anim"  # important since its how bpy.ops.import_test.some_data is constructed
    bl_label = "Animation Exporter"

    # ExportHelper mixin class uses this
    filename_ext = ".anim"
    filter_glob = StringProperty(
            default="*.anim",
            options={'HIDDEN'},
            )

    # List of operator properties, the attributes will be assigned
    # to the class instance from the operator settings before calling.
    exportBinary = BoolProperty(
        name="Binary",
        description="Export Data In The Binary Format",
        default=True,
        )

    def execute(self, context):
        return writeSkeleton(context, self.filepath, self.exportBinary)

# Only needed if you want to add into a dynamic menu
def menu_func_exportMesh(self, context):
    self.layout.operator(VRAWMeshExporter.bl_idname, text="VRAW Mesh (.vraw)")
def menu_func_exportAnimation(self, context):
    self.layout.operator(AnimationExporter.bl_idname, text="Animation (.anim)")
def register():
    bpy.utils.register_class(VRAWMeshExporter)
    bpy.types.INFO_MT_file_export.append(menu_func_exportMesh)
    bpy.utils.register_class(AnimationExporter)
    bpy.types.INFO_MT_file_export.append(menu_func_exportAnimation)
def unregister():
    bpy.utils.unregister_class(VRAWMeshExporter)
    bpy.types.INFO_MT_file_export.remove(menu_func_exportMesh)
    bpy.utils.unregister_class(AnimationExporter)
    bpy.types.INFO_MT_file_export.remove(menu_func_exportAnimation)

if __name__ == "__main__":
    register()
    # test call
    # bpy.ops.export_mesh.vraw('INVOKE_DEFAULT')
    # bpy.ops.export_animation.anim('INVOKE_DEFAULT')
