#pragma once
#include <vulkan/vulkan.hpp>

#include "RAIIWrapper.hpp"
#include <GlobalLogger.hpp>

namespace raii {
    class VkInstance: public RAIIWrapper<::VkInstance, true> {
        public:
            ~VkInstance() override {
                vkDestroyInstance(_value, nullptr);
                glog.log<DefaultLevel::Debug>("Vulkan 实例已析构");
            }
    };
}

class EvkContext;
class EvkDeviceContext;
