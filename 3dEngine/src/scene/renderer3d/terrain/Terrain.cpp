#include <GL/glew.h>
#include <cassert>
#include <stdexcept>
#include <expat_external.h>

#include "Terrain.h"
#include "resources/MediaManager.h"
#include "utils/shader/ShaderManager.h"

#define RESTART_INDEX 4294967295 //(2^32)-1
#define DEFAULT_AMBIENT 0.3

namespace urchin
{
    Terrain::Terrain(const std::string &filename, float xzScale, float yScale) :
            xzScale(xzScale),
            yScale(yScale)
    {
        auto *imgTerrain = MediaManager::instance()->getMedia<Image>(filename);
        if(imgTerrain->getImageFormat()!=Image::IMAGE_LUMINANCE)
        {
            throw std::invalid_argument("Unsupported image format for terrain rendering: " + imgTerrain->getImageFormat());
        }

        xLength = imgTerrain->getWidth();
        zLength = imgTerrain->getHeight();
        vertices = buildVertices(imgTerrain);
        texCoordinates = buildTexCoordinates();
        indices = buildIndices();
        normals = buildNormals();
        imgTerrain->release();

        glGenBuffers(4, bufferIDs);
        glGenVertexArrays(1, &vertexArrayObject);
        glBindVertexArray(vertexArrayObject);

        glBindBuffer(GL_ARRAY_BUFFER, bufferIDs[VAO_VERTEX_POSITION]);
        glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float)*3, &vertices[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(SHADER_VERTEX_POSITION);
        glVertexAttribPointer(SHADER_VERTEX_POSITION, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glBindBuffer(GL_ARRAY_BUFFER, bufferIDs[VAO_TEX_COORD]);
        glBufferData(GL_ARRAY_BUFFER, texCoordinates.size()*sizeof(float)*2, &texCoordinates[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(SHADER_TEX_COORD);
        glVertexAttribPointer(SHADER_TEX_COORD, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

        glBindBuffer(GL_ARRAY_BUFFER, bufferIDs[VAO_NORMAL]);
        glBufferData(GL_ARRAY_BUFFER, normals.size()*sizeof(float)*3, &normals[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(SHADER_NORMAL);
        glVertexAttribPointer(SHADER_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferIDs[VAO_INDEX]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        shader = ShaderManager::instance()->createProgram("terrain.vert", "terrain.frag");
        ShaderManager::instance()->bind(shader);

        mModelLoc = glGetUniformLocation(shader, "mModel");
        mProjectionLoc = glGetUniformLocation(shader, "mProjection");
        mViewLoc = glGetUniformLocation(shader, "mView");
        int diffuseTexLoc = glGetUniformLocation(shader, "diffuseTex");
        ambientLoc = glGetUniformLocation(shader, "ambient");
        setAmbient(DEFAULT_AMBIENT);

        //activate texture
        glActiveTexture(GL_TEXTURE0);
        glUniform1i(diffuseTexLoc, 0);
    }

    Terrain::~Terrain()
    {
        glDeleteVertexArrays(1, &vertexArrayObject);
        glDeleteBuffers(4, bufferIDs);

        ShaderManager::instance()->removeProgram(shader);
    }

    void Terrain::onCameraProjectionUpdate(const Matrix4<float> &projectionMatrix)
    {
        this->projectionMatrix = projectionMatrix;
    }

    const std::vector<Point3<float>> &Terrain::getVertices() const
    {
        return vertices;
    }

    unsigned int Terrain::getXLength() const
    {
        return xLength;
    }

    unsigned int Terrain::getZLength() const
    {
        return zLength;
    }

    float Terrain::getAmbient() const
    {
        return ambient;
    }

    void Terrain::setAmbient(float ambient)
    {
        this->ambient = ambient;
    }

    void Terrain::setMaterial(const std::string &materialFilename)
    {
        material = MediaManager::instance()->getMedia<Material>(materialFilename, nullptr);
    }

    void Terrain::setTransform(const Transform<float> &transform)
    {
        this->transform = transform;
    }

    const Transform<float> &Terrain::getTransform() const
    {
        return transform;
    }

    std::vector<Point3<float>> Terrain::buildVertices(const Image *imgTerrain) const
    {
        std::vector<Point3<float>> vertices;
        vertices.reserve(xLength * zLength);

        float xStart = (-(xLength * xzScale) / 2.0) + (xzScale / 2.0);
        float zStart = (-(zLength * xzScale) / 2.0) + (xzScale / 2.0);

        float minElevation = std::numeric_limits<float>::max();
        float maxElevation = -std::numeric_limits<float>::max();
        for(unsigned int z=0; z<zLength; ++z)
        {
            float zFloat = zStart + static_cast<float>(z) * xzScale;
            for (unsigned int x = 0; x < xLength; ++x)
            {
                float elevation = imgTerrain->getTexels()[x + xLength * z] * yScale;
                if(elevation > maxElevation)
                {
                    maxElevation = elevation;
                }
                if(elevation < minElevation)
                {
                    minElevation = elevation;
                }
                float xFloat = xStart + static_cast<float>(x) * xzScale;
                vertices.emplace_back(Point3<float>(xFloat, elevation, zFloat));
            }
        }

        //center terrain on Y axis
        float elevationDifference = maxElevation - minElevation;
        float elevationDelta = (elevationDifference / 2.0) - maxElevation;
        for(auto &vertex : vertices)
        {
            vertex.Y += elevationDelta;
        }

        return vertices;
    }

    std::vector<Point2<float>> Terrain::buildTexCoordinates() const
    {
        std::vector<Point2<float>> texCoordinates;
        texCoordinates.reserve(xLength * zLength);

        for(unsigned int z = 0; z < zLength; ++z)
        {
            for (unsigned int x = 0; x < xLength; ++x)
            {
                float s = static_cast<float>(x)/xLength * 10.0; //TODO hardcoded
                float t = static_cast<float>(z)/zLength * 10.0;

                texCoordinates.emplace_back(Point2<float>(s, t));
            }
        }

        return texCoordinates;
    }

    std::vector<Vector3<float>> Terrain::buildNormals() const
    {
        //1. compute normal of triangles
        unsigned int totalTriangles = ((zLength - 1) * (xLength - 1)) * 2;
        std::vector<Vector3<float>> normalTriangles;
        normalTriangles.reserve(totalTriangles);

        bool isCwTriangle = true;
        for(unsigned int i=0; i<indices.size() - 2; ++i)
        {
            if(indices[i+2] != RESTART_INDEX)
            {
                Point3<float> point1 = vertices[indices[i]];
                Point3<float> point2 = vertices[indices[i+1]];
                Point3<float> point3 = vertices[indices[i+2]];

                Vector3<float> normal;
                if(isCwTriangle)
                {
                    normal = (point1.vector(point2).crossProduct(point3.vector(point1)));
                }else
                {
                    normal = (point1.vector(point2).crossProduct(point1.vector(point3)));
                }

                normalTriangles.push_back(normal.normalize());
                isCwTriangle = !isCwTriangle;
            }else
            {
                i += 2;
            }
        }
        #ifdef _DEBUG
            assert(totalTriangles == normalTriangles.size());
        #endif

        //2. compute normal of vertex
        std::vector<Vector3<float>> normalVertex;
        normalVertex.reserve(vertices.size());
        for(unsigned int i=0; i<vertices.size(); ++i)
        {
            std::vector<unsigned int> triangleIndices = findTriangleIndices(i);

            Vector3<float> vertexNormal(0.0, 0.0, 0.0);
            for(unsigned int triangleIndex : triangleIndices)
            {
                vertexNormal += normalTriangles[triangleIndex];
            }
            vertexNormal /= triangleIndices.size();
            normalVertex.emplace_back(vertexNormal.normalize());
        }

        return normalVertex;
    }

    std::vector<unsigned int> Terrain::findTriangleIndices(unsigned int vertexIndex) const
    {
        std::vector<unsigned int> triangleIndices;
        triangleIndices.reserve(6);

        unsigned int rowNum = vertexIndex / xLength;
        int squareIndex = vertexIndex - xLength - rowNum;

        bool isLeftBorderVertex = (vertexIndex % xLength) == 0;
        bool isRightBorderVertex = ((vertexIndex + 1) % xLength) == 0;
        bool isFirstRowVertex = rowNum == 0;
        bool isLastRowVertex = rowNum == (zLength - 1);

        //above triangles to the vertex
        if(!isFirstRowVertex)
        {
            int firstLeftTopTriangle = (squareIndex * 2) + 1;

            //left triangle
            if (!isLeftBorderVertex)
            {
                triangleIndices.emplace_back(firstLeftTopTriangle);
            }

            //right triangles
            if (!isRightBorderVertex)
            {
                triangleIndices.emplace_back(firstLeftTopTriangle + 1);
                triangleIndices.emplace_back(firstLeftTopTriangle + 2);
            }
        }

        //below triangles to the vertex
        if(!isLastRowVertex)
        {
            int firstLeftBottomTriangle = (squareIndex * 2) + (xLength - 1) * 2;

            //left triangles
            if (!isLeftBorderVertex)
            {
                triangleIndices.emplace_back(firstLeftBottomTriangle);
                triangleIndices.emplace_back(firstLeftBottomTriangle + 1);
            }

            //right triangle
            if (!isRightBorderVertex)
            {
                triangleIndices.emplace_back(firstLeftBottomTriangle + 2);
            }
        }

        return triangleIndices;
    }

    std::vector<unsigned int> Terrain::buildIndices() const
    {
        std::vector<unsigned int> indices;
        indices.reserve(((zLength-1) * xLength * 2) + (zLength-1));

        for(unsigned int z = 0; z < zLength - 1; ++z)
        {
            for (unsigned int x = 0; x < xLength; ++x)
            {
                indices.push_back(x + xLength * (z + 1));
                indices.push_back(x + xLength * z);
            }

            indices.push_back(RESTART_INDEX);
        }
        return indices;
    }

    void Terrain::display(const Matrix4<float> &viewMatrix) const
    {
        unsigned int shaderSaved = ShaderManager::instance()->getCurrentProgram();
        ShaderManager::instance()->bind(shader);

        glUniformMatrix4fv(mModelLoc, 1, GL_FALSE, (const float*) transform.getTransformMatrix());
        glUniformMatrix4fv(mProjectionLoc, 1, GL_FALSE, (const float*)projectionMatrix);
        glUniformMatrix4fv(mViewLoc, 1, GL_FALSE, (const float*)viewMatrix);
        glUniform1f(ambientLoc, ambient);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, material->getDiffuseTexture()->getTextureID());

        glEnable(GL_PRIMITIVE_RESTART);
        glPrimitiveRestartIndex(RESTART_INDEX);

        glBindVertexArray(vertexArrayObject);
        glDrawElements(GL_TRIANGLE_STRIP, indices.size(), GL_UNSIGNED_INT, nullptr);

        glDisable(GL_PRIMITIVE_RESTART);

        ShaderManager::instance()->bind(shaderSaved);
    }
}