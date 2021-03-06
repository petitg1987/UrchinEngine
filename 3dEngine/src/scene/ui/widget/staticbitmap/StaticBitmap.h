#pragma once

#include <string>
#include <memory>

#include <scene/ui/widget/Widget.h>
#include <scene/ui/widget/Position.h>
#include <scene/ui/widget/Size.h>
#include <resources/image/Image.h>
#include <graphic/render/GenericRenderer.h>

namespace urchin {

    class StaticBitmap : public Widget {
        public:
            StaticBitmap(Widget*, Position, Size, std::string);

        protected:
            void createOrUpdateWidget() override;
            void prepareWidgetRendering(float) override;

        private:
            //properties
            const std::string filename;

            //visual
            std::shared_ptr<Texture> tex;
            std::unique_ptr<GenericRenderer> bitmapRenderer;
    };

}
