#pragma once
#include <VkContext.h>

#include <ResourceTypes.hpp>

class TestRender {
    public:
        TestRender(VkContext& context);

        void render() const;

    private:
        VkContext& context;
        std::vector<raii::VkShaderModule> shaderModules{};
        raii::VkRenderPass renderPass;
        raii::VkPipelineLayout pipelineLayout;
        raii::VkPipeline graphicsPipeline;
        std::vector<raii::VkFramebuffer> frameBuffers{};
        raii::VkCommandPool commandPool;
        VkCommandBuffer commandBuffer{};
        raii::VkSemaphore imageAvailableSemaphore;
        raii::VkSemaphore renderFinishedSemaphore;
        raii::VkFence inFlightFence;

        void recordCommand(uint32_t imageIndex) const;
};
