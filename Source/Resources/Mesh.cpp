#include <cstdint>
#include "Mesh.h"
#include "Math/Math.h"
#include "Datas/MeshData.h"
#include "Resources/ResourceManager.h"
#include "Core/Gfx/GfxResources.h"
#include "Core/Utility/UUID.h"
EFFECTS_NAMESPACE_BEGIN

Mesh::Mesh(MeshData *data)
    :mData(data)
{
}

Mesh::~Mesh()
{
    if(mData)
        delete mData;
    glDeleteVertexArrays(1, &mVertexArrayObject);
    glDeleteBuffers(1, &mVertexBuffer);
    glDeleteBuffers(1, &mIndexBuffer);
}

std::shared_ptr<Mesh> Mesh::create(const std::string& id, MeshData* data) {
    std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(data);
    mesh->mId = id;
    mesh->mUUID = generateUUID();
    mesh->mType = ResourceType::MESH;
    ResourceManager::instance().addRes(mesh);
    return mesh;
}

void Mesh::remove(std::shared_ptr<Mesh> mesh) {
    ResourceManager::instance().removeRes(mesh);
}

void Mesh::prepareGfxData()
{
    glGenVertexArrays(1, &mVertexArrayObject);
    glGenBuffers(1, &mVertexBuffer);
    glGenBuffers(1, &mIndexBuffer);

    glBindVertexArray(mVertexArrayObject);

    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, mData->getVertexBufferSize(), mData->getVertices(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mData->getIndexBufferSize(), mData->getIndices(), GL_STATIC_DRAW);

    if (mData->getVertexLayout()->hasVertexAttribute(SEMANTIC_POSITION))
    {
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, mData->getVertexStrideSize(), (GLvoid*)mData->getVertexLayout()->getVertexAttributeOffset(SEMANTIC_POSITION));
    }

    if (mData->getVertexLayout()->hasVertexAttribute(SEMANTIC_NORMAL))
    {
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, mData->getVertexStrideSize(), (GLvoid*)mData->getVertexLayout()->getVertexAttributeOffset(SEMANTIC_NORMAL));
    }

    if (mData->getVertexLayout()->hasVertexAttribute(SEMANTIC_COLOR))
    {
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, mData->getVertexStrideSize(), (GLvoid*)mData->getVertexLayout()->getVertexAttributeOffset(SEMANTIC_COLOR));
    }

    if (mData->getVertexLayout()->hasVertexAttribute(SEMANTIC_TANGENT))
    {
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, mData->getVertexStrideSize(), (GLvoid*)mData->getVertexLayout()->getVertexAttributeOffset(SEMANTIC_TANGENT));
    }

    if (mData->getVertexLayout()->hasVertexAttribute(SEMANTIC_JOINTS))
    {
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_INT, GL_FALSE, mData->getVertexStrideSize(), (GLvoid*)mData->getVertexLayout()->getVertexAttributeOffset(SEMANTIC_JOINTS));

        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, mData->getVertexStrideSize(), (GLvoid*)mData->getVertexLayout()->getVertexAttributeOffset(SEMANTIC_WEIGHTS));
    }

    if (mData->getVertexLayout()->hasVertexAttribute(SEMANTIC_TEXCOORD0))
    {
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 2, GL_FLOAT, GL_FALSE, mData->getVertexStrideSize(), (GLvoid*)mData->getVertexLayout()->getVertexAttributeOffset(SEMANTIC_TEXCOORD0));
    }

    if (mData->getVertexLayout()->hasVertexAttribute(SEMANTIC_TEXCOORD1))
    {
        glEnableVertexAttribArray(7);
        glVertexAttribPointer(7, 2, GL_FLOAT, GL_FALSE, mData->getVertexStrideSize(), (GLvoid*)mData->getVertexLayout()->getVertexAttributeOffset(SEMANTIC_TEXCOORD1));
    }

    if (mData->getVertexLayout()->hasVertexAttribute(SEMANTIC_TEXCOORD2))
    {
        glEnableVertexAttribArray(8);
        glVertexAttribPointer(8, 2, GL_FLOAT, GL_FALSE, mData->getVertexStrideSize(), (GLvoid*)mData->getVertexLayout()->getVertexAttributeOffset(SEMANTIC_TEXCOORD2));
    }

    if (mData->getVertexLayout()->hasVertexAttribute(SEMANTIC_TEXCOORD3))
    {
        glEnableVertexAttribArray(9);
        glVertexAttribPointer(9, 2, GL_FLOAT, GL_FALSE, mData->getVertexStrideSize(), (GLvoid*)mData->getVertexLayout()->getVertexAttributeOffset(SEMANTIC_TEXCOORD3));
    }

    glBindVertexArray(0);
}

void Mesh::draw(unsigned int mode)
{
    glBindVertexArray(mVertexArrayObject);
    glDrawElements(mode, this->getIndexCount(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

VertexLayout* Mesh::getVertexLayout()
{
    return mData->getVertexLayout();
}

uint32_t Mesh::getVertexCount()
{
    return mData->getNumVertices();
}

uint32_t Mesh::getIndexCount()
{
    return mData->getNumIndices();
}

uint32_t Mesh::getLayout()
{
    return mData->getVertexLayout()->getLayout();
}

    std::shared_ptr<Mesh> genCubeMesh()
    {
        float ws = 1;
        float hs = 1;
        float ls = 1;

        float hw = 1;
        float hh = 1;
        float hl = 1;

        glm::vec3 corners[] = {
                glm::vec3(-hw, -hh,  hl),
                glm::vec3(hw, -hh,  hl),
                glm::vec3(hw,  hh,  hl),
                glm::vec3(-hw,  hh,  hl),
                glm::vec3(hw, -hh, -hl),
                glm::vec3(-hw, -hh, -hl),
                glm::vec3(-hw,  hh, -hl),
                glm::vec3(hw,  hh, -hl),
        };

        uint32_t faceAxes[6][3] = {
                { 0, 1, 3 }, // FRONT
                { 4, 5, 7 }, // BACK
                { 3, 2, 6 }, // TOP
                { 1, 0, 4 }, // BOTTOM
                { 1, 4, 2 }, // RIGHT
                { 5, 0, 6 }  // LEFT
        };

        float faceNormals[6][3] = {
                {  0,  0,  1 }, // FRONT
                {  0,  0, -1 }, // BACK
                {  0,  1,  0 }, // TOP
                {  0, -1,  0 }, // BOTTOM
                {  1,  0,  0 }, // RIGHT
                { -1,  0,  0 }  // LEFT
        };

        std::vector<float> positions;
        std::vector<float> normals;
        std::vector<float> uvs;
        std::vector<uint32_t> indices;

        auto buildPlane = [&](uint32_t side, uint32_t uSegments, uint32_t vSegments){
            glm::vec3 temp1;
            glm::vec3 temp2;
            glm::vec3 temp3;
            glm::vec3 r;
            float u, v;
            int ix, iy;
            auto offset = positions.size() / 3;
            auto faceAxe = faceAxes[side];
            auto faceNormal = faceNormals[side];

            for (iy = 0; iy <= vSegments; iy++) {
                for (ix = 0; ix <= uSegments; ix++) {
                    u = ix / (float)uSegments;
                    v = iy / (float)vSegments;
                    temp1 = vectorLerp(corners[faceAxe[0]], corners[faceAxe[1]], u);
                    temp2 = vectorLerp(corners[faceAxe[0]], corners[faceAxe[2]], v);
                    temp3 = temp2 - corners[faceAxe[0]];
                    r = temp1 + temp3;

                    positions.push_back(r.x);
                    positions.push_back(r.y);
                    positions.push_back(r.z);

                    normals.push_back(faceNormal[0]);
                    normals.push_back(faceNormal[1]);
                    normals.push_back(faceNormal[2]);

                    uvs.push_back(u);
                    uvs.push_back(v);

                    if ((ix < uSegments) && (iy < vSegments)) {
                        auto useg1 = uSegments + 1;
                        auto a = ix + iy * useg1;
                        auto b = ix + (iy + 1) * useg1;
                        auto c = (ix + 1) + (iy + 1) * useg1;
                        auto d = (ix + 1) + iy * useg1;

                        indices.push_back(offset + a);
                        indices.push_back(offset + d);
                        indices.push_back(offset + b);
                        indices.push_back(offset + d);
                        indices.push_back(offset + c);
                        indices.push_back(offset + b);
                    }
                }
            }
        };

        buildPlane(0, ws, hs); // FRONT
        buildPlane(4, ls, hs); // RIGHT
        buildPlane(1, ws, hs); // BACK
        buildPlane(5, ls, hs); // LEFT
        buildPlane(3, ws, ls); // BOTTOM
        buildPlane(2, ws, ls); // TOP

        uint32_t layout = (uint32_t)SEMANTIC_POSITION;
        layout |= (uint32_t)SEMANTIC_TEXCOORD0;
        layout |= (uint32_t)SEMANTIC_NORMAL;
        VertexLayout* vertexLayout = new VertexLayout(layout);
        MeshData* meshData = new MeshData(positions.size() / 3, indices.size(), vertexLayout);
        meshData->setAttribute(SEMANTIC_POSITION, positions.data(), positions.size() * sizeof(float));
        meshData->setAttribute(SEMANTIC_TEXCOORD0, uvs.data(), uvs.size() * sizeof(float));
        meshData->setAttribute(SEMANTIC_NORMAL, normals.data(), normals.size() * sizeof(float));
        meshData->setIndexes(indices.data(), indices.size() * sizeof(uint32_t));
        std::shared_ptr<Mesh> mesh = Mesh::create("Cube_Mesh", meshData);
        mesh->prepareGfxData();
        return mesh;
    }

std::shared_ptr<Mesh> genQuadMesh()
{
    uint32_t layout = (uint32_t)SEMANTIC_POSITION;
    layout |= (uint32_t)SEMANTIC_TEXCOORD0;
    layout |= (uint32_t)SEMANTIC_NORMAL;
    VertexLayout* vertexLayout = new VertexLayout(layout);
    MeshData* meshData = new MeshData(4, 6, vertexLayout);
    float positionData[12] = {
            -1.0f,  1.0f, 0.0f,
            -1.0f, -1.0f, 0.0f,
            1.0f,  1.0f, 0.0f,
            1.0f, -1.0f, 0.0f};
    float texcoordData[8] = {
            0.0f, 1.0f,
            0.0f, 0.0f,
            1.0f, 1.0f,
            1.0f, 0.0f};
    float normalData[12] = {
            0.0f,  0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f,  0.0f, 1.0f,
            0.0f, 0.0f, 1.0f};

    uint32_t indexData[6] = {0, 1, 3, 0, 3, 2};

    meshData->setAttribute(SEMANTIC_POSITION, positionData, 12 * sizeof(float));
    meshData->setAttribute(SEMANTIC_TEXCOORD0, texcoordData, 8 * sizeof(float));
    meshData->setAttribute(SEMANTIC_NORMAL, normalData, 12 * sizeof(float));
    meshData->setIndexes(indexData, 6 * sizeof(uint32_t));
    std::shared_ptr<Mesh> mesh = Mesh::create("Quad_Mesh", meshData);
    mesh->prepareGfxData();
    return mesh;
}

std::shared_ptr<Mesh> genGridMesh(float horizontalLen, float verticalLen, uint32_t rows, uint32_t columns) {
    std::vector<float> positions;
    std::vector<float> normals;
    std::vector<float> uvs;
    std::vector<uint32_t> indices;

    int numVertices = (rows + 1) * (columns + 1);
    int numIndices = rows * columns * 6;

    float x, y;
    for (int i = 0; i < numVertices; i++) {
        x = (float)(i % (columns + 1)) / (float)columns;
        y = 1.0 - (float)(i / (columns + 1)) / (float)rows;

        positions.push_back(horizontalLen * (x - 0.5f));
        positions.push_back(verticalLen * (y - 0.5f));
        positions.push_back(0.0f);

        normals.push_back(0.0f);
        normals.push_back(1.0f);
        normals.push_back(0.0f);

        uvs.push_back(x);
        uvs.push_back(y);
    }

    uint32_t currentColumn, currentRow;
    for (uint32_t i = 0; i < rows * columns; i++) {
        currentColumn = i % columns;
        currentRow = i / columns;
        indices.push_back(currentColumn + currentRow * (columns + 1));
        indices.push_back(currentColumn + (currentRow + 1) * (columns + 1));
        indices.push_back((currentColumn + 1) + (currentRow + 1) * (columns + 1));
        indices.push_back((currentColumn + 1) + (currentRow + 1) * (columns + 1));
        indices.push_back((currentColumn + 1) + currentRow * (columns + 1));
        indices.push_back(currentColumn + currentRow * (columns + 1));
    }

    uint32_t layout = (uint32_t)SEMANTIC_POSITION;
    layout |= (uint32_t)SEMANTIC_TEXCOORD0;
    layout |= (uint32_t)SEMANTIC_NORMAL;
    VertexLayout* vertexLayout = new VertexLayout(layout);
    MeshData* meshData = new MeshData(positions.size() / 3, indices.size(), vertexLayout);
    meshData->setAttribute(SEMANTIC_POSITION, positions.data(), positions.size() * sizeof(float));
    meshData->setAttribute(SEMANTIC_TEXCOORD0, uvs.data(), uvs.size() * sizeof(float));
    meshData->setAttribute(SEMANTIC_NORMAL, normals.data(), normals.size() * sizeof(float));
    meshData->setIndexes(indices.data(), indices.size() * sizeof(uint32_t));
    std::shared_ptr<Mesh> mesh = Mesh::create("Grid_Mesh", meshData);
    mesh->prepareGfxData();
    return mesh;

}
EFFECTS_NAMESPACE_END
