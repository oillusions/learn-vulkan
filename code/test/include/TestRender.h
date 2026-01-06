#pragma once
#include <VkContext.h>

#include <ResourceTypes.hpp>
#include <ResourceUtils.hpp>

class TestRender {
    public:
        TestRender(VkContext& context);

    private:
        VkContext& context;
        std::vector<VkShaderModule> shaderModules{};

        ShaderResource& vert;
        ShaderResource& frag;
};
