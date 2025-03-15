#include "pch.h"
#include "MeshProperties.h"

using namespace DirectX;

struct TupleHash {
    std::size_t operator()(const std::tuple<int, int, int>& key) const {
        std::size_t seed = 0;
        auto hash_combine = [](std::size_t& seed, std::size_t value) {
            seed ^= value + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        };
        hash_combine(seed, std::hash<int>{}(std::get<0>(key)));
        hash_combine(seed, std::hash<int>{}(std::get<1>(key)));
        hash_combine(seed, std::hash<int>{}(std::get<2>(key)));
        return seed;
    }
};

void MeshProperties::NormalCalculation(std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
{
    // R�initialise toutes les normales � z�ro
    for (auto& vertex : vertices) {
        vertex.Normal = { 0.0f, 0.0f, 0.0f };
    }

    // Boucle sur chaque triangle
    for (size_t i = 0; i < indices.size(); i += 3) {
        Vertex& v0 = vertices[indices[i]];
        Vertex& v1 = vertices[indices[i + 1]];
        Vertex& v2 = vertices[indices[i + 2]];

        DirectX::XMVECTOR p0 = DirectX::XMLoadFloat3(&v0.Pos);
        DirectX::XMVECTOR p1 = DirectX::XMLoadFloat3(&v1.Pos);
        DirectX::XMVECTOR p2 = DirectX::XMLoadFloat3(&v2.Pos);

        // Calcul des vecteurs d'ar�tes
        DirectX::XMVECTOR edge1 = DirectX::XMVectorSubtract(p1, p0);
        DirectX::XMVECTOR edge2 = DirectX::XMVectorSubtract(p2, p0);

        // Produit vectoriel pour obtenir la normale
        DirectX::XMVECTOR normal = DirectX::XMVector3Cross(edge1, edge2);
        normal = DirectX::XMVector3Normalize(normal);

        // Conversion en XMFLOAT3
        DirectX::XMFLOAT3 normalFloat;
        DirectX::XMStoreFloat3(&normalFloat, normal);

        // Ajoute la normale � chaque sommet concern�
        v0.Normal.x = normalFloat.x; v0.Normal.y = normalFloat.y; v0.Normal.z = normalFloat.z;
        v1.Normal.x = normalFloat.x; v1.Normal.y = normalFloat.y; v1.Normal.z = normalFloat.z;
        v2.Normal.x = normalFloat.x; v2.Normal.y = normalFloat.y; v2.Normal.z = normalFloat.z;
    }

    // Normalisation des normales par sommet
   //for (auto& vertex : vertices) {
   //    DirectX::XMVECTOR normal = DirectX::XMLoadFloat3(&vertex.Normal);
   //    normal = DirectX::XMVector3Normalize(normal);
   //    DirectX::XMStoreFloat3(&vertex.Normal, normal);
   //}
}



MeshProperties MeshProperties::CreateBox(float size) {
    constexpr int vertexCount = 24; 
    constexpr int indexCount = 36;

    MeshProperties mesh(vertexCount, indexCount);
    float halfSize = size * 0.5f;

    mesh.vertices = {
        // Front Face
        {{-halfSize, -halfSize, -halfSize}, {1, 1, 1}, {0.0f, 1.0f}}, // Bottom-left
        {{ halfSize, -halfSize, -halfSize}, {1, 1, 1}, {1.0f, 1.0f}}, // Bottom-right
        {{-halfSize,  halfSize, -halfSize}, {1, 1, 1}, {0.0f, 0.0f}}, // Top-left
        {{ halfSize,  halfSize, -halfSize}, {1, 1, 1}, {1.0f, 0.0f}}, // Top-right

        // Back Face
        {{ halfSize, -halfSize,  halfSize}, {1, 1, 1}, {0.0f, 1.0f}}, // Bottom-left
        {{-halfSize, -halfSize,  halfSize}, {1, 1, 1}, {1.0f, 1.0f}}, // Bottom-right
        {{ halfSize,  halfSize,  halfSize}, {1, 1, 1}, {0.0f, 0.0f}}, // Top-left
        {{-halfSize,  halfSize,  halfSize}, {1, 1, 1}, {1.0f, 0.0f}}, // Top-right

        // Left Face
        {{-halfSize, -halfSize,  halfSize}, {1, 1, 1}, {0.0f, 1.0f}}, // Bottom-left
        {{-halfSize, -halfSize, -halfSize}, {1, 1, 1}, {1.0f, 1.0f}}, // Bottom-right
        {{-halfSize,  halfSize,  halfSize}, {1, 1, 1}, {0.0f, 0.0f}}, // Top-left
        {{-halfSize,  halfSize, -halfSize}, {1, 1, 1}, {1.0f, 0.0f}}, // Top-right

        // Right Face
        {{ halfSize, -halfSize, -halfSize}, {1, 1, 1}, {0.0f, 1.0f}}, // Bottom-left
        {{ halfSize, -halfSize,  halfSize}, {1, 1, 1}, {1.0f, 1.0f}}, // Bottom-right
        {{ halfSize,  halfSize, -halfSize}, {1, 1, 1}, {0.0f, 0.0f}}, // Top-left
        {{ halfSize,  halfSize,  halfSize}, {1, 1, 1}, {1.0f, 0.0f}}, // Top-right

        // Top Face
        {{-halfSize,  halfSize, -halfSize}, {1, 1, 1}, {0.0f, 1.0f}}, // Bottom-left
        {{ halfSize,  halfSize, -halfSize}, {1, 1, 1}, {1.0f, 1.0f}}, // Bottom-right
        {{-halfSize,  halfSize,  halfSize}, {1, 1, 1}, {0.0f, 0.0f}}, // Top-left
        {{ halfSize,  halfSize,  halfSize}, {1, 1, 1}, {1.0f, 0.0f}}, // Top-right

        // Bottom Face
        {{-halfSize, -halfSize,  halfSize}, {1, 1, 1}, {0.0f, 1.0f}}, // Bottom-left
        {{ halfSize, -halfSize,  halfSize}, {1, 1, 1}, {1.0f, 1.0f}}, // Bottom-right
        {{-halfSize, -halfSize, -halfSize}, {1, 1, 1}, {0.0f, 0.0f}}, // Top-left
        {{ halfSize, -halfSize, -halfSize}, {1, 1, 1}, {1.0f, 0.0f}}, // Top-right
    };

    mesh.indices = {
        0, 2, 1,  2, 3, 1,  // Front face
        4, 6, 5,  6, 7, 5,  // Back face
        8, 10, 9,  10, 11, 9, // Left face
        12, 14, 13, 14, 15, 13, // Right face
        16, 18, 17, 18, 19, 17, // Top face
        20, 22, 21, 22, 23, 21  // Bottom face
    };

    NormalCalculation(mesh.vertices, mesh.indices);
    mesh.vbByteSize = static_cast<UINT>(mesh.vertices.size() * sizeof(Vertex));
    mesh.ibByteSize = static_cast<UINT>(mesh.indices.size() * sizeof(uint32_t));

    return mesh;
}



MeshProperties MeshProperties::CreateSphere(float radius, int latitudeCount, int longitudeCount) {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    float dTheta = DirectX::XM_PI / latitudeCount;
    float dPhi = 2.0f * DirectX::XM_PI / longitudeCount;

    for (int lat = 0; lat <= latitudeCount; ++lat) {
        float theta = lat * dTheta;
        float sinTheta = sinf(theta);
        float cosTheta = cosf(theta);

        for (int lon = 0; lon <= longitudeCount; ++lon) {
            float phi = lon * dPhi;
            float sinPhi = sinf(phi);
            float cosPhi = cosf(phi);

            DirectX::XMFLOAT3 position = {
                radius * cosPhi * sinTheta,
                radius * cosTheta,
                radius * sinPhi * sinTheta
            };

            DirectX::XMFLOAT3 color = { position.x * 0.5f + 0.5f, position.y * 0.5f + 0.5f, position.z * 0.5f + 0.5f};
            XMFLOAT2 texCoord = { static_cast<float>(lon) / longitudeCount, static_cast<float>(lat) / latitudeCount };

            vertices.emplace_back(position, color, texCoord);
        }
    }

    for (int lat = 0; lat < latitudeCount; ++lat) {
        for (int lon = 0; lon < longitudeCount; ++lon) {
            uint32_t current = static_cast<uint32_t>(lat * (longitudeCount + 1) + lon);
            uint32_t next = static_cast<uint32_t>(current + longitudeCount + 1);

            indices.push_back(current);
            indices.push_back(next);
            indices.push_back(static_cast<uint32_t>(current + 1));

            indices.push_back(static_cast<uint32_t>(current + 1));
            indices.push_back(next);
            indices.push_back(static_cast<uint32_t>(next + 1));
        }
    }

    MeshProperties mesh(vertices, indices);
    NormalCalculation(mesh.vertices, mesh.indices);
    mesh.vbByteSize = static_cast<UINT>(vertices.size() * sizeof(Vertex));
    mesh.ibByteSize = static_cast<UINT>(indices.size() * sizeof(uint32_t));
    
    return mesh;
}


MeshProperties MeshProperties::CreateCylinder(float radius, float height, int sliceCount) {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    float halfHeight = height * 0.5f;
    float dTheta = 2.0f * DirectX::XM_PI / sliceCount;

    // Cr�ation des sommets du cylindre
    for (int i = 0; i <= sliceCount; ++i) {
        float theta = i * dTheta;
        float x = radius * cosf(theta);
        float z = radius * sinf(theta);

        // Coordonn�es de texture (u, v)
        float u = static_cast<float>(i) / sliceCount;
        float vTop = 1.0f;    // Pour le sommet sup�rieur
        float vBottom = 0.0f; // Pour le sommet inf�rieur

        // Ajout des sommets avec des normales par d�faut (initialis�es � z�ro)
        vertices.emplace_back(Vertex{
            XMFLOAT3(x, halfHeight, z),  // Position
            XMFLOAT3(0.0f, 0.0f, 0.0f),  // Normale (initialis�e � z�ro)
            XMFLOAT2(u, vTop)            // Coordonn�es de texture
            });

        vertices.emplace_back(Vertex{
            XMFLOAT3(x, -halfHeight, z), // Position
            XMFLOAT3(0.0f, 0.0f, 0.0f),  // Normale (initialis�e � z�ro)
            XMFLOAT2(u, vBottom)         // Coordonn�es de texture
            });
    }

    // Ajout des centres des cercles sup�rieur et inf�rieur
    int topCenterIndex = static_cast<int>(vertices.size());
    vertices.emplace_back(Vertex{
        XMFLOAT3(0, halfHeight, 0),      // Position
        XMFLOAT3(0.0f, 1.0f, 0.0f),      // Normale (pointant vers le haut)
        XMFLOAT2(0.5f, 0.5f)             // Coordonn�es de texture (centre)
        });

    int bottomCenterIndex = static_cast<int>(vertices.size());
    vertices.emplace_back(Vertex{
        XMFLOAT3(0, -halfHeight, 0),     // Position
        XMFLOAT3(0.0f, -1.0f, 0.0f),     // Normale (pointant vers le bas)
        XMFLOAT2(0.5f, 0.5f)             // Coordonn�es de texture (centre)
        });

    // Cr�ation des indices pour les triangles
    for (int i = 0; i < sliceCount; ++i) {
        uint32_t next = static_cast<uint32_t>((i + 1) % sliceCount);

        // Triangles pour la face lat�rale
        indices.insert(indices.end(), {
            static_cast<uint32_t>(2 * i), static_cast<uint32_t>(2 * next), static_cast<uint32_t>(2 * i + 1),
            static_cast<uint32_t>(2 * i + 1), static_cast<uint32_t>(2 * next), static_cast<uint32_t>(2 * next + 1)
            });

        // Triangles pour la face sup�rieure
        indices.insert(indices.end(), {
            static_cast<uint32_t>(topCenterIndex), static_cast<uint32_t>(2 * i), static_cast<uint32_t>(2 * next)
            });

        // Triangles pour la face inf�rieure
        indices.insert(indices.end(), {
            static_cast<uint32_t>(bottomCenterIndex), static_cast<uint32_t>(2 * i + 1), static_cast<uint32_t>(2 * next + 1)
            });
    }

    // Cr�ation de l'objet MeshProperties
    MeshProperties mesh(vertices, indices);
    mesh.vbByteSize = static_cast<UINT>(vertices.size() * sizeof(Vertex));
    mesh.ibByteSize = static_cast<UINT>(indices.size() * sizeof(uint32_t));

    // Calcul des normales
    mesh.NormalCalculation(vertices, mesh.indices);

    return mesh;
}

MeshProperties MeshProperties::CreatePyramid(float size)
{
    constexpr int vertexCount = 5;  
    constexpr int indexCount = 18;  

    MeshProperties mesh(vertexCount, indexCount);

    float halfSize = size * 0.5f;
    float height = size;  


    mesh.vertices = {
        {{-halfSize, 0.0f, -halfSize}, {1, 0, 0}, {0.0f, 1.0f}}, 
        {{ halfSize, 0.0f, -halfSize}, {0, 1, 0}, {1.0f, 1.0f}}, 
        {{ halfSize, 0.0f,  halfSize}, {0, 0, 1}, {1.0f, 0.0f}},
        {{-halfSize, 0.0f,  halfSize}, {1, 1, 0}, {0.0f, 0.0f}}, 
        {{0.0f, height, 0.0f}, {1, 1, 1}, {0.5f, 0.5f}} 
    };

    mesh.indices = {
        0, 1, 4,  
        1, 2, 4,  
        2, 3, 4,  
        3, 0, 4, 
        0, 3, 2, 
        0, 2, 1   
    };

    NormalCalculation(mesh.vertices, mesh.indices);
    mesh.vbByteSize = static_cast<UINT>(mesh.vertices.size() * sizeof(Vertex));
    mesh.ibByteSize = static_cast<UINT>(mesh.indices.size() * sizeof(uint32_t));

    return mesh;
}



MeshProperties MeshProperties::CreateCustomMesh(float size, const std::string& file) {
    std::ifstream objFile(file);
    if (!objFile.is_open()) {
        throw std::runtime_error("Failed to open OBJ file: " + file);
    }

    std::vector<DirectX::XMFLOAT3> tempPositions;
    std::vector<DirectX::XMFLOAT2> tempTexCoords;
    std::vector<DirectX::XMFLOAT3> tempNormals;
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    std::unordered_map<std::tuple<int, int, int>, uint32_t, TupleHash> vertexCache;

    std::string line;
    while (std::getline(objFile, line)) {
        if (line.empty() || line[0] == '#') continue;

        std::istringstream stream(line);
        std::string prefix;
        stream >> prefix;

        if (prefix == "v") {
            float x, y, z;
            stream >> x >> y >> z;
            tempPositions.emplace_back(x * size, y * size, z * size);
        }
        else if (prefix == "vt") {
            float u, v;
            stream >> u >> v;
            tempTexCoords.emplace_back(u, 1.0f - v);
        }
        else if (prefix == "vn") {
            float nx, ny, nz;
            stream >> nx >> ny >> nz;
            tempNormals.emplace_back(nx, ny, nz);
        }
        else if (prefix == "f") {
            std::vector<std::tuple<int, int, int>> vertexData;

            std::string vtn;
            while (stream >> vtn) {
                std::replace(vtn.begin(), vtn.end(), '/', ' ');
                std::istringstream vtnStream(vtn);

                int vIndex = 0, tIndex = 0, nIndex = 0;
                vtnStream >> vIndex;
                if (vtnStream.peek() == ' ') vtnStream >> tIndex;
                if (vtnStream.peek() == ' ') vtnStream >> nIndex;

                vertexData.emplace_back(vIndex - 1, tIndex - 1, nIndex - 1);
            }

            if (vertexData.size() == 3) {
                // Triangle face
                for (const auto& key : vertexData) {
                    auto it = vertexCache.find(key);
                    if (it == vertexCache.end()) {
                        Vertex vertex = {};
                        vertex.Pos = tempPositions[std::get<0>(key)];
                        vertex.TexCoord = std::get<1>(key) >= 0 ? tempTexCoords[std::get<1>(key)] : XMFLOAT2(0.0f, 0.0f);
                        uint32_t newIndex = static_cast<uint32_t>(vertices.size());
                        vertices.push_back(vertex);
                        vertexCache[key] = newIndex;
                        indices.push_back(newIndex);
                    }
                    else {
                        indices.push_back(it->second);
                    }
                }
            }
            else if (vertexData.size() == 4) {
                // Quad face -> Convert to two triangles
                uint32_t indicesArr[4];

                for (int i = 0; i < 4; i++) {
                    auto it = vertexCache.find(vertexData[i]);
                    if (it == vertexCache.end()) {
                        Vertex vertex = {};
                        vertex.Pos = tempPositions[std::get<0>(vertexData[i])];
                        vertex.TexCoord = std::get<1>(vertexData[i]) >= 0 ? tempTexCoords[std::get<1>(vertexData[i])] : XMFLOAT2(0.0f, 0.0f);
                        uint32_t newIndex = static_cast<uint32_t>(vertices.size());
                        vertices.push_back(vertex);
                        vertexCache[vertexData[i]] = newIndex;
                        indicesArr[i] = newIndex;
                    }
                    else {
                        indicesArr[i] = it->second;
                    }
                }

                // Split quad into two triangles
                indices.push_back(indicesArr[0]);
                indices.push_back(indicesArr[1]);
                indices.push_back(indicesArr[2]);

                indices.push_back(indicesArr[0]);
                indices.push_back(indicesArr[2]);
                indices.push_back(indicesArr[3]);
            }
        }
    }
    objFile.close();

    MeshProperties mesh(vertices, indices);
    NormalCalculation(mesh.vertices, mesh.indices);
    mesh.vbByteSize = static_cast<UINT>(vertices.size() * sizeof(Vertex));
    mesh.ibByteSize = static_cast<UINT>(indices.size() * sizeof(uint32_t));

    return mesh;
}
