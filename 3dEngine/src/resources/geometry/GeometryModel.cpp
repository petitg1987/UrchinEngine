#include "resources/geometry/GeometryModel.h"
#include "graphic/render/shader/builder/ShaderBuilder.h"
#include "graphic/render/GenericRendererBuilder.h"

namespace urchin {

    GeometryModel::GeometryModel() :
            isInitialized(false),
            color(Vector4<float>(0.0f, 1.0f, 0.0f, 1.0f)),
            polygonMode(WIREFRAME),
            outlineSize(1.3f),
            transparencyEnabled(false),
            alwaysVisible(false) {
        shader = ShaderBuilder::createShader("displayGeometry.vert", "", "displayGeometry.frag");
    }

    void GeometryModel::initialize(const std::shared_ptr<RenderTarget>& renderTarget) {
        this->renderTarget = renderTarget;

        modelMatrix = retrieveModelMatrix();
        refreshRenderer();

        isInitialized = true;
    }

    void GeometryModel::refreshRenderer() {
        if(!renderTarget) {
            return;
        }

        std::vector<Point3<float>> vertexArray = retrieveVertexArray();

        auto rendererBuilder = GenericRendererBuilder::create("geometry model", renderTarget, shader, getShapeType())
                ->addData(vertexArray)
                ->addShaderData(sizeof(positioningData), &positioningData) //binding 0
                ->addShaderData(sizeof(color), &color) //binding 1
                ->disableCullFace()
                ->outlineSize(outlineSize)
                ->polygonMode(polygonMode);

        if (!alwaysVisible) {
            rendererBuilder->enableDepthOperations();
        }

        if (transparencyEnabled) {
            rendererBuilder->enableTransparency();
        }

        renderer = rendererBuilder->build();
    }

    void GeometryModel::onCameraProjectionUpdate(const Matrix4<float>& projectionMatrix) {
        positioningData.projectionMatrix = projectionMatrix;
    }

    Vector4<float> GeometryModel::getColor() const {
        return color;
    }

    void GeometryModel::setColor(float red, float green, float blue, float alpha) {
        color = Vector4<float>(red, green, blue, alpha);
    }

    PolygonMode GeometryModel::getPolygonMode() const {
        return polygonMode;
    }

    void GeometryModel::setPolygonMode(PolygonMode polygonMode) {
        this->polygonMode = polygonMode;
        refreshRenderer();
    }

    void GeometryModel::setOutlineSize(float outlineSize) {
        this->outlineSize = outlineSize;
        refreshRenderer();
    }

    bool GeometryModel::isTransparencyEnabled() const {
        return transparencyEnabled;
    }

    void GeometryModel::enableTransparency() {
        transparencyEnabled = true;
        refreshRenderer();
    }

    bool GeometryModel::isAlwaysVisible() const {
        return alwaysVisible;
    }

    void GeometryModel::setAlwaysVisible(bool alwaysVisible) {
        this->alwaysVisible = alwaysVisible;
        refreshRenderer();
    }

    void GeometryModel::prepareRendering(const Matrix4<float>& viewMatrix) const {
        if (!isInitialized) {
            throw std::runtime_error("Geometry model must be initialized before call prepare rendering");
        }

        positioningData.viewModelMatrix = viewMatrix * modelMatrix;
        renderer->updateShaderData(0, &positioningData);
        renderer->updateShaderData(1, &color);
    }

}
