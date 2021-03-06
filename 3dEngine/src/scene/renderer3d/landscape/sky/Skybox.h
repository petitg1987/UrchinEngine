#pragma once

#include <string>
#include <vector>
#include <memory>
#include <UrchinCommon.h>

#include <graphic/render/shader/model/Shader.h>
#include <graphic/render/target/RenderTarget.h>
#include <graphic/render/GenericRenderer.h>
#include <resources/image/Image.h>
#include <scene/renderer3d/camera/Camera.h>

namespace urchin {

    class Skybox {
        public:
            explicit Skybox(const std::vector<std::string>&);
            ~Skybox();

            void initialize(RenderTarget&);
            void onCameraProjectionUpdate(const Matrix4<float>&) const;

            float getOffsetY() const;
            void setOffsetY(float);
            const std::vector<std::string>& getFilenames() const;

            void prepareRendering(const Matrix4<float>&, const Point3<float>&);

        private:
            void clearSkyboxImages();

            bool isInitialized;

            std::vector<std::string> filenames;
            std::array<Image*, 6> skyboxImages;

            float offsetY;
            Matrix4<float> translationMatrix;

            //visual
            std::unique_ptr<GenericRenderer> skyboxRenderer;
            std::unique_ptr<Shader> skyboxShader;
    };

}
