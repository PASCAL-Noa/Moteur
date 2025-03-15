#pragma once
#include "Tools/Transform.h"

class MeshProperties
{
private:
    int mVertexSize;
    int mIndicesSize;

public:
    struct Vertex
    {
        DirectX::XMFLOAT3 Pos;
        DirectX::XMFLOAT3 Normal;
        DirectX::XMFLOAT2 TexCoord; 
    };

    std::vector<Vertex> vertices;
    std::vector<std::uint32_t> indices;

    UINT vbByteSize;
    UINT ibByteSize;
    Transform mMeshTransform;

    MeshProperties(int vertexSize, int indicesSize)
        : mVertexSize(vertexSize), mIndicesSize(indicesSize),
        vertices(vertexSize), indices(indicesSize)
    {
    }

    MeshProperties(const std::vector<Vertex>& verts, const std::vector<std::uint32_t>& inds)
        : mVertexSize(static_cast<int>(verts.size())),
        mIndicesSize(static_cast<int>(inds.size())),
        vertices(verts),
        indices(inds) {}

    int GetVertexSize() const { return mVertexSize; }
    int GetIndicesSize() const { return mIndicesSize; }

    static void NormalCalculation(std::vector<Vertex>& vertices,
                                 const std::vector<uint32_t>& indices);

    static MeshProperties CreateBox(float size);
    static MeshProperties CreateSphere(float radius, int latitudeCount, int longitudeCount);
    static MeshProperties CreateCylinder(float radius, float height, int sliceCount);
    static MeshProperties CreatePyramid(float size);
    static MeshProperties CreateCustomMesh(float size, const std::string& file);
};
