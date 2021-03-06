#pragma once

#include <vulkan/vulkan.h>

#include <graphic/render/target/RenderTarget.h>
#include <graphic/texture/Texture.h>

namespace urchin {

    class OffscreenRender : public RenderTarget {
        public:
            explicit OffscreenRender(std::string, DepthAttachmentType);
            ~OffscreenRender() override;

            void addTexture(const std::shared_ptr<Texture>&);
            void resetTextures();

            void initialize() override;
            void cleanup() override;

            unsigned int getWidth() const override;
            unsigned int getHeight() const override;
            unsigned int getLayer() const override;
            std::size_t getNumFramebuffer() const override;
            std::size_t getNumColorAttachment() const override;

            VkSemaphore retrieveQueueSubmitSemaphoreAndFlagUsed();

            void render() override;

        private:
            void initializeClearValues();
            void createRenderPass();
            void createFramebuffers();
            void createSyncObjects();
            void destroySyncObjects();

            void waitCommandBuffersIdle() const override;

            bool isInitialized;

            std::vector<VkClearValue> clearValues;
            std::vector<std::shared_ptr<Texture>> textures;

            VkSemaphore queueSubmitSemaphore;
            bool queueSubmitSemaphoreUsable;
            VkFence commandBufferFence;
    };

}
