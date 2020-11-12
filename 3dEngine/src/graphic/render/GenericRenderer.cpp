#include <GL/glew.h>
#include <stdexcept>
#include <cassert>

#include "graphic/render/GenericRenderer.h"
#include "graphic/render/GenericRendererBuilder.h"
#include "graphic/texture/TextureManager.h"

namespace urchin {

    GenericRenderer::GenericRenderer(const GenericRendererBuilder *rendererBuilder) :
            shapeType(rendererBuilder->getShapeType()),
            shapeCount(rendererBuilder->getShapeCount()),
            vertexCoordType(rendererBuilder->getVertexCoordType()),
            vertexCoordDimension(rendererBuilder->getVertexCoordDimension()),
            textureCoordType(rendererBuilder->getTextureCoordType()),
            textureCoordDimension(rendererBuilder->getTextureCoordDimension()),
            transparencyEnabled(rendererBuilder->isTransparencyEnabled()),
            depthTestEnabled(rendererBuilder->isDepthTestEnabled()),
            cullFaceEnabled(rendererBuilder->isCullFaceEnabled()),
            polygonMode(rendererBuilder->getPolygonMode()),
            outlineSize(rendererBuilder->getOutlineSize()),
            textures(rendererBuilder->getTextures()),
            bufferIDs(),
            vertexArrayObject(0) {
        glGenBuffers(2, bufferIDs);
        glGenVertexArrays(1, &vertexArrayObject);

        for (const auto &texture : textures) {
            initializeTexture(texture);
        }
        additionalTextures.reserve(2); //estimated memory size

        initializeDisplay(rendererBuilder->getVertexCoord(), rendererBuilder->getTextureCoord());
    }

    GenericRenderer::~GenericRenderer() {
        if (vertexArrayObject!=0) {
            glDeleteVertexArrays(1, &vertexArrayObject);
        }

        glDeleteBuffers(2, bufferIDs);
    }

    void GenericRenderer::initializeTexture(Texture texture) const {
        unsigned int textureType = texture.getGlType();
        glBindTexture(textureType, texture.getId());

        unsigned int readMode = texture.getParam().getGlReadMode();
        glTexParameteri(textureType, GL_TEXTURE_WRAP_S, readMode);
        glTexParameteri(textureType, GL_TEXTURE_WRAP_T, readMode);
        if (textureType == GL_TEXTURE_CUBE_MAP) {
            glTexParameteri(textureType, GL_TEXTURE_WRAP_R, readMode);
        }

        glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, texture.getParam().getGlReadQualityMinifyingFilter());
        glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, texture.getParam().getGlReadQualityMagnificationFilter());

        if (texture.getParam().needAnisotropy() && GLEW_EXT_texture_filter_anisotropic) {
            glTexParameterf(textureType, GL_TEXTURE_MAX_ANISOTROPY_EXT, TextureManager::instance()->getAnisotropy());
        }
    }

    void GenericRenderer::initializeDisplay(void *vertexCoord, void *textureCoord) {
        glBindVertexArray(vertexArrayObject);

        auto vertexCoordDim = coordDimensionToSize(vertexCoordDimension);
        auto vertexMemorySize = coordTypeToSize(vertexCoordType) * vertexCoordDim * countVertexNumber(shapeType, shapeCount);
        glBindBuffer(GL_ARRAY_BUFFER, bufferIDs[VAO_VERTEX_POSITION]);
        glBufferData(GL_ARRAY_BUFFER, vertexMemorySize, vertexCoord, GL_STATIC_DRAW);
        glEnableVertexAttribArray(SHADER_VERTEX_POSITION);
        glVertexAttribPointer(SHADER_VERTEX_POSITION, vertexCoordDim, coordTypeToGlType(vertexCoordType), GL_FALSE, 0, nullptr);

        auto textureCoordDim = coordDimensionToSize(textureCoordDimension);
        auto textureMemorySize = coordTypeToSize(textureCoordType) * textureCoordDim * countVertexNumber(shapeType, shapeCount);
        glBindBuffer(GL_ARRAY_BUFFER, bufferIDs[VAO_TEX_COORD]);
        glBufferData(GL_ARRAY_BUFFER, textureMemorySize, textureCoord, GL_STATIC_DRAW);
        glEnableVertexAttribArray(SHADER_TEX_COORD);
        glVertexAttribPointer(SHADER_TEX_COORD, textureCoordDim, coordTypeToGlType(textureCoordType), GL_FALSE, 0, nullptr);
    }

    unsigned int GenericRenderer::countVertexNumber(ShapeType shapeType, unsigned int shapeCount) const {
        if (shapeType == ShapeType::RECTANGLE) {
            return shapeCount * 4;
        } else if (shapeType == ShapeType::LINE) {
            return shapeCount * 2;
        } else if (shapeType == ShapeType::LINE_STRIP) {
            return shapeCount + 1;
        } else if (shapeType == ShapeType::TRIANGLE) {
            return shapeCount * 3;
        } else if (shapeType == ShapeType::TRIANGLE_STRIP) {
            return shapeCount + 2;
        }
        throw std::runtime_error("Unknown shape type: " + std::to_string(shapeType));
    }

    unsigned int GenericRenderer::shapeTypeToGlType(ShapeType shapeType) const {
        if (shapeType == ShapeType::RECTANGLE) {
            return GL_QUADS;
        } else if (shapeType == ShapeType::LINE) {
            return GL_LINES;
        } else if (shapeType == ShapeType::LINE_STRIP) {
            return GL_LINE_STRIP;
        } else if (shapeType == ShapeType::TRIANGLE) {
            return GL_TRIANGLES;
        } else if (shapeType == ShapeType::TRIANGLE_STRIP) {
            return GL_TRIANGLE_STRIP;
        }
        throw std::runtime_error("Unknown shape type: " + std::to_string(shapeType));
    }

    unsigned int GenericRenderer::coordTypeToSize(CoordType coordType) const {
        if (coordType == CoordType::FLOAT) {
            return sizeof(float);
        } else if (coordType == CoordType::INT) {
            return sizeof(int);
        } else if (coordType == CoordType::UNSIGNED_INT) {
            return sizeof(unsigned int);
        }
        throw std::runtime_error("Unknown coordinate type: " + std::to_string(coordType));
    }

    unsigned int GenericRenderer::coordTypeToGlType(CoordType coordType) const {
        if (coordType == CoordType::FLOAT) {
            return GL_FLOAT;
        } else if (coordType == CoordType::INT) {
            return GL_INT;
        } else if (coordType == CoordType::UNSIGNED_INT) {
            return GL_UNSIGNED_INT;
        }
        throw std::runtime_error("Unknown coordinate type: " + std::to_string(coordType));
    }

    unsigned int GenericRenderer::coordDimensionToSize(CoordDimension coordDimension) const {
        if (coordDimension == CoordDimension::_2D) {
            return 2;
        } else if (coordDimension == CoordDimension::_3D) {
            return 3;
        } else if (coordDimension == CoordDimension::_4D) {
            return 4;
        }
        throw std::runtime_error("Unknown coordinate dimension: " + std::to_string(coordDimension));
    }

    void GenericRenderer::updateTexture(std::size_t textureUnit, Texture texture) {
        initializeTexture(texture);

        assert(textures.size() > textureUnit);
        textures[textureUnit] = texture;
    }

    /**
     * @return Texture unit
     */
    unsigned int GenericRenderer::addAdditionalTexture(Texture texture) {
        initializeTexture(texture);

        additionalTextures.push_back(texture);
        return textures.size() + additionalTextures.size() - 1;
    }

    void GenericRenderer::clearAdditionalTextures() {
        additionalTextures.clear();
    }

    void GenericRenderer::draw() const {
        if(shapeCount == 0) {
            return;
        }

        unsigned int textureUnit = 0;
        for (const auto &texture : textures) {
            glActiveTexture(GL_TEXTURE0 + textureUnit++);
            glBindTexture(texture.getGlType(), texture.getId());
        }
        for (const auto &additionalTexture : additionalTextures) {
            glActiveTexture(GL_TEXTURE0 + textureUnit++);
            glBindTexture(additionalTexture.getGlType(), additionalTexture.getId());
        }

        if(transparencyEnabled) {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }

        if(!depthTestEnabled) {
            glDisable(GL_DEPTH_TEST);
            glDepthMask(GL_FALSE);
        }

        if(!cullFaceEnabled) {
            glDisable(GL_CULL_FACE);
        }

        if(polygonMode == PolygonMode::WIREFRAME) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glLineWidth(outlineSize);
        }

        glBindVertexArray(vertexArrayObject);
        glDrawArrays(shapeTypeToGlType(shapeType), 0, countVertexNumber(shapeType, shapeCount));

        resetDrawDefaultValues();
    }

    void GenericRenderer::resetDrawDefaultValues() const {
        if(transparencyEnabled) {
            glDisable(GL_BLEND);
        }

        if(!depthTestEnabled) {
            glEnable(GL_DEPTH_TEST);
            glDepthMask(GL_TRUE);
        }

        if(!cullFaceEnabled) {
            glEnable(GL_CULL_FACE);
            glCullFace(GL_FRONT);
        }

        if(polygonMode != PolygonMode::FILL) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glLineWidth(1.0f);
            glPointSize(1.0f);
        }
    }
}
