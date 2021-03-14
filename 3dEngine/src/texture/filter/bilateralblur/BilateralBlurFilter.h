#ifndef URCHINENGINE_BILATERALBLURFILTER_H
#define URCHINENGINE_BILATERALBLURFILTER_H

#include <string>
#include <map>

#include "texture/filter/TextureFilter.h"
#include "graphic/shader/model/ShaderVar.h"

namespace urchin {

    class BilateralBlurFilterBuilder;

    class BilateralBlurFilter : public TextureFilter {
        public:
            enum BlurDirection {
                VERTICAL,
                HORIZONTAL
            };

            BilateralBlurFilter(const BilateralBlurFilterBuilder*, BlurDirection);

            void onCameraProjectionUpdate(float, float);

        private:
            std::string getShaderName() const override;
            void initiateAdditionalShaderVariables(const std::unique_ptr<Shader>&) override;
            void completeShaderTokens(std::map<std::string, std::string>&) const override;

            std::vector<float> computeOffsets() const;

            BlurDirection blurDirection;
            unsigned int blurSize;
            float blurSharpness;

            unsigned int textureSize;
            ShaderVar cameraPlanesShaderVar;

            std::string offsetsTab;
    };

}

#endif
