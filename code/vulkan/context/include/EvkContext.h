#pragma once
#include <map>

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>

#include <GlobalLogger.hpp>
#include <VulkanTypes.hpp>
#include <VulkanUtils.hpp>

class EvkContext;
class EvkDeviceContext;

class EvkContext {
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

        template<typename Func, typename... Args>
        raii::VkSurfaceKHR& createSurface(const std::string& identifier, Func&& func, Args&&... args) {
            auto& surface = _surfaceMap.emplace(identifier, _instance).first->second;
            func(_instance, surface, std::forward<Args>(args)...);
            return surface;
        }

        static Builder builder() {return Builder{};}
        static bool checkLayerSupport(const std::vector<const char*>& layerNames);
        static bool checkExtensionSupport(const std::vector<const char *> &extensionNames);
    private:
        raii::VkInstance _instance;
        std::map<std::string, raii::VkSurfaceKHR> _surfaceMap{};


#ifdef NDEBUG
        const bool EnableDebug = false;
#else
        const bool EnableDebug = true;
#endif


        EvkContext(const VkApplicationInfo &appInfo, std::vector<const char*> extensionNames, std::vector<const char*> layerNames);
};