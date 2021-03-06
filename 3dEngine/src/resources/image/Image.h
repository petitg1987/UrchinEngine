#pragma once

#include <vector>
#include <memory>

#include <resources/Resource.h>
#include <graphic/texture/model/TextureFormat.h>
#include <graphic/texture/Texture.h>

namespace urchin {

    class Image : public Resource {
        public:
            enum ImageFormat {
                IMAGE_RGBA,
                IMAGE_GRAYSCALE
            };
            enum ChannelPrecision {
                CHANNEL_8,
                CHANNEL_16
            };

            Image(unsigned int, unsigned int, ImageFormat, std::vector<unsigned char>&&);
            Image(unsigned int, unsigned int, ImageFormat, std::vector<uint16_t>&&);
            ~Image() override = default;

            unsigned int getWidth() const;
            unsigned int getHeight() const;
            ImageFormat getImageFormat() const;
            ChannelPrecision getChannelPrecision() const;
            const std::vector<unsigned char>& getTexels() const;
            const std::vector<uint16_t>& getTexels16Bits() const;

            unsigned int retrieveComponentsCount() const;
            TextureFormat retrieveTextureFormat() const;

            std::shared_ptr<Texture> createTexture(bool);

        private:
            unsigned int width;
            unsigned int height;
            ImageFormat format;
            ChannelPrecision channelPrecision;
            std::vector<unsigned char> texels8; //8 bits
            std::vector<uint16_t> texels16; //16 bits
    };

}
