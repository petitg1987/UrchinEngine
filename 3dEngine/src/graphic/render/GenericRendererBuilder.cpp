#include "graphic/render/GenericRendererBuilder.h"

namespace urchin {

    GenericRendererBuilder::GenericRendererBuilder(ShapeType shapeType) :
            shapeType(shapeType),
            transparencyEnabled(false),
            depthTestEnabled(false),
            cullFaceEnabled(true),
            pPolygonMode(PolygonMode::FILL),
            pOutlineSize(1.0f) {

    }

    ShapeType GenericRendererBuilder::getShapeType() const {
        return shapeType;
    }

    GenericRendererBuilder *GenericRendererBuilder::addPointsCoord(const std::vector<Point2<float>> *pointsCoord) {
        GenericRenderer::PointsCoord pointsCoord2d{};
        pointsCoord2d.coordType = CoordType::FLOAT;
        pointsCoord2d.coordDimension = CoordDimension::TWO_DIMENSION;
        pointsCoord2d.ptr = &(*pointsCoord)[0];
        pointsCoord2d.pointsCount = pointsCoord->size();
        pointsCoords.push_back(pointsCoord2d);
        return this;
    }

    GenericRendererBuilder *GenericRendererBuilder::addPointsCoord(const std::vector<Point3<float>> *pointsCoord) {
        GenericRenderer::PointsCoord pointsCoord3d{};
        pointsCoord3d.coordType = CoordType::FLOAT;
        pointsCoord3d.coordDimension = CoordDimension::THREE_DIMENSION;
        pointsCoord3d.ptr = &(*pointsCoord)[0];
        pointsCoord3d.pointsCount = pointsCoord->size();
        pointsCoords.push_back(pointsCoord3d);
        return this;
    }

    GenericRendererBuilder *GenericRendererBuilder::addPointsCoord(const std::vector<Vector3<float>> *pointsCoord) {
        addPointsCoord(reinterpret_cast<const std::vector<Point3<float>> *>(pointsCoord));
        return this;
    }

    const std::vector<GenericRenderer::PointsCoord> &GenericRendererBuilder::getPointsCoords() const {
        return pointsCoords;
    }

    GenericRendererBuilder *GenericRendererBuilder::indices(const std::vector<unsigned int> *indices) {
        pIndices.ptr = &(*indices)[0];
        pIndices.indicesCount = indices->size();
        return this;
    }

    const GenericRenderer::Indices &GenericRendererBuilder::getIndices() const {
        return pIndices;
    }

    GenericRendererBuilder *GenericRendererBuilder::enableTransparency() {
        this->transparencyEnabled = true;
        return this;
    }

    bool GenericRendererBuilder::isTransparencyEnabled() const {
        return transparencyEnabled;
    }

    GenericRendererBuilder *GenericRendererBuilder::enableDepthTest() {
        this->depthTestEnabled = true;
        return this;
    }

    bool GenericRendererBuilder::isDepthTestEnabled() const {
        return depthTestEnabled;
    }

    GenericRendererBuilder *GenericRendererBuilder::disableCullFace() {
        cullFaceEnabled = false;
        return this;
    }

    bool GenericRendererBuilder::isCullFaceEnabled() const {
        return cullFaceEnabled;
    }

    GenericRendererBuilder *GenericRendererBuilder::polygonMode(PolygonMode pPolygonMode) {
        this->pPolygonMode = pPolygonMode;
        return this;
    }

    PolygonMode GenericRendererBuilder::getPolygonMode() const {
        return pPolygonMode;
    }

    GenericRendererBuilder *GenericRendererBuilder::outlineSize(float pOutlineSize) {
        this->pOutlineSize = pOutlineSize;
        return this;
    }

    float GenericRendererBuilder::getOutlineSize() const {
        return pOutlineSize;
    }

    GenericRendererBuilder *GenericRendererBuilder::addTexture(Texture texture) {
        textures.push_back(texture);
        return this;
    }

    const std::vector<Texture> &GenericRendererBuilder::getTextures() const {
        return textures;
    }

    std::unique_ptr<GenericRenderer> GenericRendererBuilder::build() {
        return std::make_unique<GenericRenderer>(this);
    }

}
