#ifndef URCHINENGINE_TERRAIN_H
#define URCHINENGINE_TERRAIN_H

#include <string>
#include <vector>
#include "UrchinCommon.h"

#include "resources/image/Image.h"
#include "resources/material/Material.h"

namespace urchin
{

    class Terrain
    { //TODO handle: texture, shadow, map editor, AI
        public:
            Terrain(const std::string &, float, float);
            ~Terrain();

            void onCameraProjectionUpdate(const Matrix4<float> &);

            const std::vector<Point3<float>> &getVertices() const;
            unsigned int getXLength() const;
            unsigned int getZLength() const;
            float getAmbient() const;
            void setAmbient(float);

            void setMaterial(const std::string &);

            void setTransform(const Transform<float> &);
            const Transform<float> &getTransform() const;

            void display(const Matrix4<float> &) const;

        private:
            std::vector<Point3<float>> buildVertices(const Image *) const;
            std::vector<Point2<float>> buildTexCoordinates() const;
            std::vector<Vector3<float>> buildNormals() const;
            std::vector<unsigned int> buildIndices() const;

            std::vector<unsigned int> findTriangleIndices(unsigned int) const;

            unsigned int bufferIDs[4], vertexArrayObject;
            enum //buffer IDs indices
            {
                VAO_VERTEX_POSITION = 0,
                VAO_TEX_COORD,
                VAO_NORMAL,
                VAO_INDEX
            };
            enum //shader input
            {
                SHADER_VERTEX_POSITION = 0,
                SHADER_TEX_COORD,
                SHADER_NORMAL
            };
            unsigned int shader;
            int mModelLoc, mProjectionLoc, mViewLoc;
            int ambientLoc;

            float xzScale;
            float yScale;
            Matrix4<float> projectionMatrix;
            float ambient;

            Material *material;

            Transform<float> transform;

            unsigned int xLength, zLength;
            std::vector<Point3<float>> vertices;
            std::vector<Point2<float>> texCoordinates;
            std::vector<Vector3<float>> normals;
            std::vector<unsigned int> indices;
    };

}

#endif