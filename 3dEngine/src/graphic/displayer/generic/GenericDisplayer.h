#ifndef URCHINENGINE_GENERICDISPLAYER_H
#define URCHINENGINE_GENERICDISPLAYER_H

#include <vector>

#include "graphic/displayer/CoordDataType.h"
#include "graphic/displayer/ShapeType.h"
#include "graphic/displayer/Texture.h"

namespace urchin {

    class GenericDisplayerBuilder;

    class GenericDisplayer { //TODO rename: GenericRender + package name
        public:
            explicit GenericDisplayer(const GenericDisplayerBuilder *);
            ~GenericDisplayer();

            void updateTexture(std::size_t, Texture);
            unsigned int addAdditionalTexture(Texture);
            void clearAdditionalTextures();

            void display() const;

        private:
            void initializeTexture(Texture) const;
            void initializeDisplay(bool, bool);

            unsigned int shapeTypeToVertexCount(ShapeType) const;
            unsigned int shapeTypeToGlType(ShapeType) const;
            unsigned int dataTypeToSize(CoordDataType) const;
            unsigned int dataTypeToGlType(CoordDataType) const;

            ShapeType shapeType;
            unsigned int shapeCount;
            unsigned int dimension; //2=2D, 3=3D

            CoordDataType vertexDataType;
            void *vertexCoord;

            CoordDataType textureDataType;
            void *textureCoord;

            std::vector<Texture> textures, additionalTextures;

            unsigned int bufferIDs[2], vertexArrayObject;
            enum //buffer IDs indices
            {
                VAO_VERTEX_POSITION = 0,
                VAO_TEX_COORD
            };
            enum //shader input
            {
                SHADER_VERTEX_POSITION = 0,
                SHADER_TEX_COORD
            };
    };

}

#endif

