#include <algorithm>

#include <resources/geometry/GeometryManager.h>

namespace urchin {

    GeometryManager::GeometryManager(RenderTarget &renderTarget) :
            renderTarget(renderTarget) {

    }

    void GeometryManager::addGeometry(GeometryModel* geometry) {
        if (geometry) {
            geometryModels.push_back(geometry);

            geometry->initialize(renderTarget);
            geometry->onCameraProjectionUpdate(projectionMatrix);
        }
    }

    void GeometryManager::removeGeometry(GeometryModel* geometry) {
        if (geometry) {
            auto it = std::find(geometryModels.begin(), geometryModels.end(), geometry);
            if (it != geometryModels.end()) {
                geometryModels.erase(it);
            }
        }
    }

    void GeometryManager::onCameraProjectionUpdate(const Camera* camera) {
        this->projectionMatrix = camera->getProjectionMatrix();

        for (auto& geometryModel : geometryModels) {
            geometryModel->onCameraProjectionUpdate(projectionMatrix);
        }
    }

    void GeometryManager::prepareRendering(const Matrix4<float>& viewMatrix) const {
        for (auto* geometryModel : geometryModels) {
            geometryModel->prepareRendering(viewMatrix);
        }
    }

}
