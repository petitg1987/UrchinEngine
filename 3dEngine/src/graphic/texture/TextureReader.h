#ifndef URCHINENGINE_TEXTUREREADER_H
#define URCHINENGINE_TEXTUREREADER_H

#include "graphic/texture/Texture.h"
#include "graphic/texture/model/TextureType.h"
#include "graphic/texture/model/TextureParam.h"

namespace urchin {

    class TextureReader {
        public:
            static TextureReader build(std::shared_ptr<Texture>, TextureParam);

            const std::shared_ptr<Texture>& getTexture() const;
            TextureParam getParam() const;

        private:
            TextureReader(std::shared_ptr<Texture>, TextureParam);

            std::shared_ptr<Texture> texture;
            TextureParam param;
    };

}

#endif
