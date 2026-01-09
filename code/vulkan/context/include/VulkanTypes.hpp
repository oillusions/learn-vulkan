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

    class VkDebugUtilsMessengerEXT: public VkRAIIWrapper<::VkDebugUtilsMessengerEXT> {
        public:
            VkDebugUtilsMessengerEXT(VkInstance& instance): _instance(instance) {};
            ~VkDebugUtilsMessengerEXT() override {
                glog.log<DefaultLevel::Debug>("Vulkan DebugUtilsMessengerEXT已析构");
                const auto func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(_instance, "vkDestroyDebugUtilsMessengerEXT"));
                if (func != nullptr) {
                    func(_instance, _value, nullptr);
                }
            }
        private:
            VkInstance& _instance;
    };

    class VkDevice: public VkRAIIWrapper<::VkDevice> {
        public:
            ~VkDevice() override {
                glog.log<DefaultLevel::Debug>("Vulkan Device已析构");
                vkDestroyDevice(_value, nullptr);
            }
    };

    class VkSurfaceKHR: public VkRAIIWrapper<::VkSurfaceKHR> {
    public:
        VkSurfaceKHR(VkInstance& instance): _instance(instance) {};
        ~VkSurfaceKHR() override {
            glog.log<DefaultLevel::Debug>("Vulkan Surface已析构");
            vkDestroySurfaceKHR(_instance, _value, nullptr);
        }
    private:
        VkInstance& _instance;
    };
}
