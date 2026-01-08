#pragma once
#include <RAIIWrapper.hpp>
#include <vulkan/vulkan.hpp>

#include "GlobalLogger.hpp"

namespace raii {
    template<typename CarriedType>
    using VkRAIIWrapper = RAIIWrapper<CarriedType, false, false>;

    class VkInstance: public RAIIWrapper<::VkInstance, true> {
    public:
        ~VkInstance() override {
            vkDestroyInstance(_value, nullptr);
            glog.log<DefaultLevel::Debug>("Vulkan 实例已析构");
        }
    };
    class VkSurfaceKHR: public VkRAIIWrapper<::VkSurfaceKHR> {
    public:
        VkSurfaceKHR(VkInstance &instance): _instance(instance) {
        }
        ~VkSurfaceKHR() override {
            vkDestroySurfaceKHR(_instance, _value, nullptr);
            glog.log<DefaultLevel::Debug>("Vulkan Surface已析构");
        }
    private:
        VkInstance& _instance;
    };
}
