#pragma once
#include <RAIIWrapper.hpp>
#include <vulkan/vulkan.hpp>

#include "GlobalLogger.hpp"

namespace raii {
    template<typename CarriedType>
    using VkRAIIWrapper = RAIIWrapper<CarriedType>;

    class VkInstance: public RAIIWrapper<::VkInstance, true> {
    public:
        ~VkInstance() override {
            if (_value == VK_NULL_HANDLE) return;
            vkDestroyInstance(_value, nullptr);
            glog.log<DefaultLevel::Debug>("Vulkan Instance已析构");
        }
    };

    class VkDebugUtilsMessengerEXT: public VkRAIIWrapper<::VkDebugUtilsMessengerEXT> {
        public:
            VkDebugUtilsMessengerEXT(VkInstance& instance): _instance(instance) {};
            ~VkDebugUtilsMessengerEXT() override {
                if (_value == VK_NULL_HANDLE) return;
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
                if (_value == VK_NULL_HANDLE) return;
                glog.log<DefaultLevel::Debug>("Vulkan Device已析构");
                vkDestroyDevice(_value, nullptr);
            }
    };

    class VkImageView: public VkRAIIWrapper<::VkImageView> {
        public:
            VkImageView(::VkDevice& device): _device(device) {};
            ~VkImageView() override {
                if (_value == VK_NULL_HANDLE) return;
                glog.log<DefaultLevel::Debug>("Vulkan ImageView已析构");
                vkDestroyImageView(_device, _value, nullptr);
            }
        private:
            ::VkDevice& _device;
    };

    class VkSurfaceKHR: public VkRAIIWrapper<::VkSurfaceKHR> {
    public:
        VkSurfaceKHR(VkInstance& instance): _instance(instance) {};
        ~VkSurfaceKHR() override {
            if (_value == VK_NULL_HANDLE) return;
            glog.log<DefaultLevel::Debug>("Vulkan Surface已析构");
            vkDestroySurfaceKHR(_instance, _value, nullptr);
        }
    private:
        VkInstance& _instance;
    };

    class VkSwapChainKHR: public  VkRAIIWrapper<::VkSwapchainKHR> {
        public:
            VkSwapChainKHR(::VkDevice& device): _device(device) {};
            ~VkSwapChainKHR() override {
                if (_value == VK_NULL_HANDLE) return;
                glog.log<DefaultLevel::Debug>("Vulkan SwapChain已析构");
                vkDestroySwapchainKHR(_device, _value, nullptr);
            }
        private:
            ::VkDevice& _device;
    };

    class VkCommandPool: public VkRAIIWrapper<::VkCommandPool> {
    public:
        VkCommandPool(::VkDevice& device): _device(device) {};
        ~VkCommandPool() override {
            if (_value == VK_NULL_HANDLE) return;
            glog.log<DefaultLevel::Debug>("Vulkan CommandPool已析构");
            vkDestroyCommandPool(_device, _value, nullptr);
        }
    private:
        ::VkDevice& _device;
    };

    class VkFramebuffer: public VkRAIIWrapper<::VkFramebuffer> {
        public:
            VkFramebuffer(::VkDevice& device): _device(device) {};
            ~VkFramebuffer() override {
                if (_value == VK_NULL_HANDLE) return;
                glog.log<DefaultLevel::Debug>("Vulkan FrameBuffer已析构");
                vkDestroyFramebuffer(_device, _value, nullptr);
            }
        private:
            ::VkDevice& _device;
    };

    class VkPipeline: public VkRAIIWrapper<::VkPipeline> {
        public:
            VkPipeline(::VkDevice& device): _device(device) {};
            ~VkPipeline() override {
                if (_value == VK_NULL_HANDLE) return;
                glog.log<DefaultLevel::Debug>("Vulkan PipeLine已析构");
                vkDestroyPipeline(_device, _value, nullptr);
            }
        private:
            ::VkDevice& _device;
    };

    class VkPipelineLayout: public VkRAIIWrapper<::VkPipelineLayout> {
        public:
            VkPipelineLayout(::VkDevice& device): _device(device) {};
            ~VkPipelineLayout() override {
                if (_value == VK_NULL_HANDLE) return;
                glog.log<DefaultLevel::Debug>("Vulkan PipeLineLayout已析构");
                vkDestroyPipelineLayout(_device, _value, nullptr);
            }
        private:
            ::VkDevice& _device;
    };

    class VkRenderPass: public VkRAIIWrapper<::VkRenderPass> {
        public:
            VkRenderPass(::VkDevice& device): _device(device) {};
            ~VkRenderPass() override {
                if (_value == VK_NULL_HANDLE) return;
                glog.log<DefaultLevel::Debug>("Vulkan RenderPass已析构");
                vkDestroyRenderPass(_device, _value, nullptr);
            }
        private:
            ::VkDevice& _device;
    };

    class VkShaderModule: public VkRAIIWrapper<::VkShaderModule> {
        public:
            VkShaderModule(::VkDevice& device): _device(device) {};
            ~VkShaderModule() override {
                if (_value == VK_NULL_HANDLE) return;
                glog.log<DefaultLevel::Debug>("Vulkan ShaderModule已析构");
                vkDestroyShaderModule(_device, _value, nullptr);
            }
        private:
            ::VkDevice& _device;
    };

    class VkSemaphore: public VkRAIIWrapper<::VkSemaphore> {
        public:
            VkSemaphore(::VkDevice& device): _device(device) {};
            ~VkSemaphore() override {
                if (_value == VK_NULL_HANDLE) return;
                glog.log<DefaultLevel::Debug>("Vulkan Semaphore已析构");
                vkDestroySemaphore(_device, _value, nullptr);
            }
        private:
            ::VkDevice& _device;
    };

    class VkFence: public VkRAIIWrapper<::VkFence> {
        public:
            VkFence(::VkDevice& device): _device(device) {};
            ~VkFence() override {
                if (_value == VK_NULL_HANDLE) return;
                glog.log<DefaultLevel::Debug>("Vulkan Fence已析构");
                vkDestroyFence(_device, _value, nullptr);
            }
        private:
            ::VkDevice& _device;
    };
}
