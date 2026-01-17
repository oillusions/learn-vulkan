#pragma once

#include <map>

#include "VulkanTypes.hpp"

template<typename T, typename Type = std::decay_t<T>>
Type& reference_cast(T& _value) {
    return _value;
}

class PhysicalDeviceContext;

class QueueFamilyInfo {
    public:
        const uint32_t queueFamilyIndex;
        const uint32_t queueCount;
        const VkQueueFlags queueFlags;

        QueueFamilyInfo(uint32_t queue_family_index, uint32_t queue_count, VkQueueFlags queue_flags):
            queueFamilyIndex(queue_family_index),
            queueCount(queue_count),
            queueFlags(queue_flags),
            _queueOccupancyCount(0) {};

        QueueFamilyInfo(uint32_t queue_family_index, const VkQueueFamilyProperties& properties):
            queueFamilyIndex(queue_family_index),
            queueCount(properties.queueCount),
            queueFlags(properties.queueFlags),
            _queueOccupancyCount(0) {};

        bool occupies(uint32_t quantity) {
            if (isAllowOccupation(quantity)) {
                _queueOccupancyCount += quantity;
                return true;
            }
            return false;
        }

        [[nodiscard]] bool isAllowOccupation(uint32_t quantity) const {
            if (queryRemainingCount() < quantity) return false;
            return true;
        }

        [[nodiscard]] uint32_t queryRemainingCount() const {
            return queueCount - _queueOccupancyCount;
        }

        [[nodiscard]] uint32_t queueOccupancyCount() const {
            return _queueOccupancyCount;
        }
    private:
        uint32_t _queueOccupancyCount;
};

class VulkanContext;

class LogicDeviceContext: public raii::VkDevice {
    public:
        class Builder {
            public:
                PhysicalDeviceContext& _physicalDeviceContext;

                Builder(PhysicalDeviceContext& physical_device_context): _physicalDeviceContext(physical_device_context) {};

                Builder& addQueue(const std::string& identifier, QueueFamilyInfo& queue_family_info, uint32_t count = 1, const float* priority = nullptr);
                Builder& configFeatures(const VkPhysicalDeviceFeatures* enable_features);
                Builder& addLayer(const char* layer_name);
                Builder& addExtension(const char* extension_name);

                LogicDeviceContext build();
            private:
                std::vector<std::string> queueIdentifiers;
                std::vector<VkDeviceQueueCreateInfo> queueCreates;
                std::vector<const char*> layerNames;
                std::vector<const char*> extensionNames;
                const VkPhysicalDeviceFeatures* features{nullptr};

        };

        static Builder builder(PhysicalDeviceContext& physical_device_context);
    private:
        std::map<std::string, VkQueue> queueMap;

        LogicDeviceContext() = default;
        void init(std::map<std::string, VkQueue> queue_map);
};

class PhysicalDeviceContext: public raii::VkRAIIWrapper<VkPhysicalDevice> {
    public:
        VulkanContext& vulkanContext;
        VkPhysicalDevice& physicalDevice = _value;
        PhysicalDeviceContext(VulkanContext& vulkan_context, VkPhysicalDevice physical_device):
            vulkanContext(vulkan_context) {
            _value = physical_device;
            initQueueFamilies(physical_device);
        }

        QueueFamilyInfo &queryQueueFamily(VkQueueFlags flags, uint32_t requiredQuantity = 0);
        QueueFamilyInfo &queryPresentQueueFamily(const VkSurfaceKHR& surface);

        template<typename Func, typename... Args>
        raii::VkDevice createLogicDevice(Func&& func, Args&&... args) {
            raii::VkDevice device{};
            func(reference_cast(*this), reference_cast(device), std::forward<Args>(args)...);
            return device;
        }
    private:
        std::vector<QueueFamilyInfo> queueFamilies;

        void initQueueFamilies(const VkPhysicalDevice& physical_device);
};

class VulkanContext: public raii::VkInstance{
    public:
        ::VkInstance& instance = _value;
        std::unique_ptr<PhysicalDeviceContext> physicalDevice{};

        VulkanContext();

        template<typename Func, typename... Args>
        raii::VkSurfaceKHR createSurface(Func&& func, Args&&... args) {
            raii::VkSurfaceKHR surface{instance};
            func(reference_cast(instance), reference_cast(surface), std::forward<Args>(args)...);
            return surface;
        }
};