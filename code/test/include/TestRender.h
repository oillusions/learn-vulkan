#pragma once
#include <VkContext.h>

#include <ResourceTypes.hpp>

class TestRender {
    public:
        TestRender(VkContext& context);
        ~TestRender();

        void render() const;

    private:
        VkContext& context;
        std::vector<VkShaderModule> shaderModules{};
        VkRenderPass renderPass{};
        VkPipelineLayout pipelineLayout{};
        VkPipeline graphicsPipeline{};
        std::vector<VkFramebuffer> frameBuffers{};
        VkCommandPool commandPool{};
        VkCommandBuffer commandBuffer{};
        VkSemaphore imageAvailableSemaphore{};
        VkSemaphore renderFinishedSemaphore{};
        VkFence inFlightFence{};

        void recordCommand(uint32_t imageIndex) const;
};
