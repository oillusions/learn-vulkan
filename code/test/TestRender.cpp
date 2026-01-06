#include "TestRender.h"

inline static std::filesystem::path vertShader = "./bin_shader/default.vert.spv";
inline static std::filesystem::path fragShader = "./bin_shader/default.frag.spv";

TestRender::TestRender(VkContext &context):
    context(context),
    vert(arm.load<ShaderResource>("shader.vert.default", vertShader)),
    frag(arm.load<ShaderResource>("shader.frag.default", vertShader)) {

    shaderModules.resize(2);
    for (auto& shaderModule : shaderModules) {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = vert->size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(vert->data());
        if (vkCreateShaderModule(context._device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
            glog.log<DefaultLevel::Error>("TestRender 着色器程序创建失败");
            std::terminate();
        }
    }
}
