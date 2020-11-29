#include <cmath>
#include <stdexcept>

#include "ShadowData.h"

namespace urchin {

    ShadowData::ShadowData(const Light* light, unsigned int nbFrustumSplit) :
            light(light),
            renderTarget(nullptr) {
        for (unsigned int frustumSplitIndex =0; frustumSplitIndex<nbFrustumSplit; ++frustumSplitIndex) {
            frustumShadowData.push_back(new FrustumShadowData(frustumSplitIndex));
        }
    }

    ShadowData::~ShadowData() {
        for (auto& fsd : frustumShadowData) {
            delete fsd;
        }
    }

    void ShadowData::setRenderTarget(std::unique_ptr<OffscreenRender>&& renderTarget) {
        this->renderTarget = std::move(renderTarget);
    }

    const OffscreenRender *ShadowData::getRenderTarget() const {
        return renderTarget.get();
    }

    void ShadowData::setDepthTexture(const std::shared_ptr<Texture>& depthTexture) {
        this->depthTexture = depthTexture;
    }

    const std::shared_ptr<Texture> &ShadowData::getDepthTexture() const {
        return depthTexture;
    }

    /**
     * @param shadowMapTexture Shadow map texture (variance shadow map)
     */
    void ShadowData::setShadowMapTexture(const std::shared_ptr<Texture>& shadowMapTexture) {
        this->shadowMapTexture = shadowMapTexture;
    }

    /**
     * Returns shadow map texture (variance shadow map)
     */
    const std::shared_ptr<Texture> &ShadowData::getShadowMapTexture() const {
        return shadowMapTexture;
    }

    void ShadowData::addTextureFilter(std::unique_ptr<TextureFilter> textureFilter) {
        textureFilters.push_back(std::move(textureFilter));
    }

    void ShadowData::applyTextureFilters() const {
        unsigned int layersToUpdate = 0;
        for (std::size_t i = 0; i < getNbFrustumShadowData(); ++i) {
            if (getFrustumShadowData(i)->needShadowMapUpdate()) {
                layersToUpdate = layersToUpdate | MathAlgorithm::powerOfTwo(i);
            }
        }

        std::shared_ptr<Texture> texture = shadowMapTexture;
        for (auto& textureFilter : textureFilters) {
            textureFilter->applyOn(texture, static_cast<int>(layersToUpdate));
            texture = textureFilter->getTexture();
        }
    }

    const std::shared_ptr<Texture> &ShadowData::getFilteredShadowMapTexture() const {
        if (textureFilters.empty()) {
            return shadowMapTexture;
        }

        return textureFilters.back()->getTexture();
    }

    void ShadowData::setLightViewMatrix(const Matrix4<float>& lightViewMatrix) {
        this->lightViewMatrix = lightViewMatrix;
    }

    const Matrix4<float> &ShadowData::getLightViewMatrix() const {
        return lightViewMatrix;
    }

    unsigned int ShadowData::getNbFrustumShadowData() const {
        return frustumShadowData.size();
    }

    /**
     * @param index Index of frustum split (0: frustum split nearest to eye)
     */
    FrustumShadowData *ShadowData::getFrustumShadowData(std::size_t index) {
        return frustumShadowData[index];
    }

    /**
     * @param index Index of frustum split (0: frustum split nearest to eye)
     */
    const FrustumShadowData *ShadowData::getFrustumShadowData(std::size_t index) const {
        return frustumShadowData[index];
    }

    const std::vector<Model *> &ShadowData::retrieveModels() const {
        models.clear();

        for (std::size_t i=0; i<getNbFrustumShadowData(); ++i) {
            if (getFrustumShadowData(i)->needShadowMapUpdate()) {
                const std::vector<Model *> &frustumSplitModels = getFrustumShadowData(i)->getModels();
                OctreeableHelper<Model>::merge(models, frustumSplitModels);
            }
        }

        return models;
    }
}
