#ifndef URCHINENGINE_BILATERALBLURFILTERBUILDER_H
#define URCHINENGINE_BILATERALBLURFILTERBUILDER_H

#include "texture/filter/TextureFilterBuilder.h"

namespace urchin {

    class BilateralBlurFilterBuilder : public TextureFilterBuilder<BilateralBlurFilterBuilder> {
        public:
            enum BlurDirection {
                HORIZONTAL_BLUR,
                VERTICAL_BLUR
            };

            explicit BilateralBlurFilterBuilder(const std::shared_ptr<Texture>&);
            ~BilateralBlurFilterBuilder() override = default;

            BilateralBlurFilterBuilder* blurDirection(BlurDirection);

            BilateralBlurFilterBuilder* blurSize(unsigned int);
            unsigned int getBlurSize() const;

            BilateralBlurFilterBuilder* blurSharpness(float);
            float getBlurSharpness() const;

            std::unique_ptr<TextureFilter> build() override;
            std::unique_ptr<BilateralBlurFilter> buildBilateralBlur();

        private:
            BlurDirection pBlurDirection;
            unsigned int pBlurSize;
            float pBlurSharpness;
    };

}

#endif
