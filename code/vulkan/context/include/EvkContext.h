#pragma once
#include <map>

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>

#include <GlobalLogger.hpp>
#include <VulkanTypes.hpp>
#include <VulkanUtils.hpp>

class EvkContext;
class EvkPhysicalDeviceContext;
class EvkLogicDeviceContext;
class EvkSwapChainContext;

class EvkContext: public raii::VkRAIIWrapper<raii::VkInstance> {
    public:
        enum class APIVersion: uint32_t {
            V1_0 = VK_API_VERSION_1_0,
            V1_1 = VK_API_VERSION_1_1,
            V1_2 = VK_API_VERSION_1_2,
            V1_3 = VK_API_VERSION_1_3,
            V1_4 = VK_API_VERSION_1_4,
        };

        class Builder {
            public:
                Builder() = default;
                ~Builder() = default;
                Builder& apiVersion(APIVersion version);
                Builder& appName(const std::string& name);
                Builder& appVersion(uint32_t version);

                Builder& appendExtension(const char* extensionName);
                Builder& appendLayer(const char* layerName);
                EvkContext build();
            private:
                VkApplicationInfo _appInfo{
                    .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
                    .pApplicationName = "Undefined APPName",
                    .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
                    .pEngineName = nullptr,
                    .engineVersion = VK_MAKE_VERSION(1, 0, 0),
                    .apiVersion = VK_API_VERSION_1_0,
                };
                std::vector<const char*> _extensionNames{};
                std::vector<const char*> _layerNames{};
        };

        EvkPhysicalDeviceContext& defaultPhysicalDevice() {return *_defaultPhysicalDevice;};

        static Builder builder() {return Builder{};}
        static bool checkInstanceLayerSupport(const std::vector<const char*>& layerNames);
        static bool checkInstanceExtensionSupport(const std::vector<const char *> &extensionNames);
        static bool checkPhysicalDeviceExtensionSupport(const VkPhysicalDevice& physicalDevice, const std::vector<const char *> &extensionNames);
        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData);
    private:
        raii::VkDebugUtilsMessengerEXT _debugMessenger{_value};
        EvkPhysicalDeviceContext* _defaultPhysicalDevice;
        std::vector<std::unique_ptr<EvkPhysicalDeviceContext>> _physicalDevices{};


#ifdef NDEBUG
        const bool EnableDebug = false;
#else
        const bool EnableDebug = true;
#endif

        EvkContext(const VkApplicationInfo &appInfo, std::vector<const char*> extensionNames, std::vector<const char*> layerNames);

        void initPhysicalDeviceMap();
        void pickDefaultPhysicalDevice();
        void setupDebugMessenger();
};

class EvkLogicDeviceContext: public raii::VkRAIIWrapper<raii::VkDevice> {
    friend EvkPhysicalDeviceContext;
    public:
        class Builder {
            public:
                Builder(EvkLogicDeviceContext& logicDeviceContext):
                    _logicDeviceContext(logicDeviceContext) {};

                Builder& appendQueue(uint32_t queueFamilyIndex, float priority = 1.0f);
                Builder& appendExtension(const char* extensionName);
                Builder& appendLayer(const char* layerName);
                Builder& setupFeatures(const VkPhysicalDeviceFeatures& features);

                EvkLogicDeviceContext& build();

            private:
                EvkLogicDeviceContext& _logicDeviceContext;
                VkDeviceCreateInfo _deviceInfo{
                    .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
                    .pEnabledFeatures = nullptr,
                };
                std::vector<VkDeviceQueueCreateInfo> _queueCreateInfos{};
                std::vector<const char*> _extensionNames{};
                std::vector<const char*> _layerNames{};
        };

        EvkPhysicalDeviceContext& _physicalDeviceContext;

        EvkLogicDeviceContext(EvkPhysicalDeviceContext& physicalDevice);
    private:
        std::vector<VkQueue> _queues{};


};

class EvkPhysicalDeviceContext: public raii::VkRAIIWrapper<VkPhysicalDevice> {
    friend EvkLogicDeviceContext;
    public:
        EvkContext& _vkContext;
        EvkPhysicalDeviceContext(EvkContext& vkContext, VkPhysicalDevice physicalDevice);
        ~EvkPhysicalDeviceContext() override {
            glog.log<DefaultLevel::Debug>("PhysicalDeviceContext 已析构");
        }

        template<typename Func, typename... Args>
        raii::VkSurfaceKHR& createSurface(const std::string& identifier, Func&& func, Args&&... args) {
            auto& surface = _surfaceMap.emplace(identifier, _vkContext).first->second;

            func(_vkContext.get(), surface, std::forward<Args>(args)...);
            return surface;
        }

        EvkLogicDeviceContext::Builder logicDeviceBuilder(const std::string& identifier) {
            return {_logicDeviceMap.emplace(identifier, *this).first->second};
        }

        raii::VkSurfaceKHR& findSurface(const std::string& identifier);
    private:
        std::map<std::string, raii::VkSurfaceKHR> _surfaceMap{};
        std::vector<VkQueueFamilyProperties> _queueFamilies{};
        std::map<std::string, EvkLogicDeviceContext> _logicDeviceMap{};
};