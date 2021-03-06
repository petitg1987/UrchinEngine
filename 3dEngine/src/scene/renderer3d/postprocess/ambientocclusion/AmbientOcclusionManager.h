#pragma once

#include <UrchinCommon.h>

#include <graphic/render/shader/model/Shader.h>
#include <scene/renderer3d/camera/Camera.h>
#include <graphic/render/GenericRenderer.h>
#include <texture/filter/bilateralblur/BilateralBlurFilter.h>

namespace urchin {

    class AmbientOcclusionManager {
        public:
            enum AOTextureSize {
                FULL_SIZE = 0,
                HALF_SIZE = 1
            };

            struct Config {
                AOTextureSize textureSize = AOTextureSize::HALF_SIZE;
                unsigned int kernelSamples = 32;
                float radius = 0.35f; //scope radius in units
                float ambientOcclusionStrength = 0.10f;
                float depthStartAttenuation = 0.995f;
                float depthEndAttenuation = 0.997f;
                unsigned int noiseTextureSize = 4;
                float bias = 0.15f;
                bool isBlurActivated = true;
                unsigned int blurSize = 7;
                float blurSharpness = 40.0f;
            };

            AmbientOcclusionManager();
            ~AmbientOcclusionManager();

            void onTexturesUpdate(const std::shared_ptr<Texture>&, const std::shared_ptr<Texture>&);
            void onResize(unsigned int, unsigned int);
            void onCameraProjectionUpdate(const Camera*);

            void updateConfig(const Config&);
            const Config& getConfig() const;

            const std::shared_ptr<Texture>& getAmbientOcclusionTexture() const;

            void updateAOTexture(const Camera*);
            void loadAOTexture(GenericRenderer&, std::size_t) const;

        private:
            struct AmbientOcclusionShaderConst {
                uint32_t kernelSamples;
                float radius;
                float ambientOcclusionStrength;
                float depthStartAttenuation;
                float depthEndAttenuation;
                uint32_t noiseTextureSize;
                float bias;
            };

            void checkConfig() const;

            void createOrUpdateAO();
            void createOrUpdateAOTexture();
            void createOrUpdateRenderer();
            void createOrUpdateAOShader();
            void generateKernelSamples();
            void generateNoiseTexture();
            void exportSVG(const std::string&, const std::vector<Vector4<float>>&) const;

            int retrieveTextureSizeFactor() const;

            static const unsigned int KERNEL_SAMPLES_SHADER_LIMIT;

            //scene information
            float nearPlane, farPlane;

            //config
            Config config;
            unsigned int textureSizeX, textureSizeY;

            //frame buffer object
            std::unique_ptr<OffscreenRender> offscreenRenderTarget;
            std::shared_ptr<Texture> ambientOcclusionTexture;

            //ambient occlusion shader
            std::unique_ptr<Shader> ambientOcclusionShader;
            std::shared_ptr<Texture> noiseTexture;
            struct {
                alignas(16) Matrix4<float> inverseProjectionViewMatrix;
                alignas(16) Matrix4<float> projectionMatrix;
                alignas(16) Matrix4<float> viewMatrix;
            } positioningData;
            std::vector<Vector4<float>> ssaoKernel;
            Vector2<float> resolution;

            //visual data
            std::shared_ptr<Texture> depthTexture;
            std::shared_ptr<Texture> normalAndAmbientTexture;
            std::unique_ptr<GenericRenderer> renderer;

            std::unique_ptr<BilateralBlurFilter> verticalBlurFilter;
            std::unique_ptr<BilateralBlurFilter> horizontalBlurFilter;
    };

}
