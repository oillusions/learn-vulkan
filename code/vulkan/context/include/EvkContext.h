#pragma once

#include "VulkanTypes.hpp"

template<typename T, typename Type = std::decay_t<T>>
Type& reference_cast(T& _value) {
    return _value;
}

class QueueFamilyInfo {
    public:
        const uint32_t queueCount;
        const VkQueueFlags queueFlags;

        QueueFamilyInfo(uint32_t queueCount, VkQueueFlags queueFlags):
            queueCount(queueCount),
            queueFlags(queueFlags),
            _queueOccupancyCount(0) {};

        QueueFamilyInfo(const VkQueueFamilyProperties& properties):
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

class PhysicalDeviceContext: public raii::VkRAIIWrapper<VkPhysicalDevice> {
    public:
        VulkanContext& vulkanContext;
        VkPhysicalDevice& physicalDevice = _value;
        PhysicalDeviceContext(VulkanContext& vulkan_context, VkPhysicalDevice physical_device):
            vulkanContext(vulkan_context) {
            _value = physical_device;
            initQueueFamilies(physical_device);
        }

        std::pair<uint32_t, QueueFamilyInfo> &queryQueueFamily(VkQueueFlags flags, uint32_t requiredQuantity = 0);
        std::pair<uint32_t, QueueFamilyInfo> &queryPresentQueueFamily(VkSurfaceKHR& surface);

        template<typename Func, typename... Args>
        raii::VkDevice createLogicDevice(Func&& func, Args&&... args) {
            raii::VkDevice device{};
            func(reference_cast(*this), reference_cast(device), std::forward<Args>(args)...);
            return device;
        }
    private:
        std::vector<std::pair<uint32_t, QueueFamilyInfo>> queueFamilies;

        void initQueueFamilies(const VkPhysicalDevice& physical_device);
};

class VulkanContext: public raii::VkRAIIWrapper<raii::VkInstance>{
    public:
        raii::VkInstance& instance = _value;
        std::unique_ptr<PhysicalDeviceContext> physicalDevice{};

        VulkanContext();

        template<typename Func, typename... Args>
        raii::VkSurfaceKHR createSurface(Func&& func, Args&&... args) {
            raii::VkSurfaceKHR surface{instance};
            func(reference_cast(instance), reference_cast(surface), std::forward<Args>(args)...);
            return surface;
        }
};