#include "EvkContext.h"

#include <ranges>
#include <GLFW/glfw3.h>

using namespace std;

pair<uint32_t, QueueFamilyInfo> emptyQueueFamilyInfo{0, {0, VK_QUEUE_FLAG_BITS_MAX_ENUM}};

VulkanContext::VulkanContext() {
    VkApplicationInfo appInfo{};
    VkInstanceCreateInfo instanceInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "undefined";
    appInfo.apiVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = nullptr;
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    uint32_t count = 0;
    const char **glfwExtensions = glfwGetRequiredInstanceExtensions(&count);

    const std::vector extensions(glfwExtensions, glfwExtensions + count);

    instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceInfo.pApplicationInfo = &appInfo;
    instanceInfo.enabledLayerCount = 0;
    instanceInfo.ppEnabledLayerNames = nullptr;
    instanceInfo.enabledExtensionCount = extensions.size();
    instanceInfo.ppEnabledExtensionNames = extensions.data();
    vkCreateInstance(&instanceInfo, nullptr, &instance);

    vkEnumeratePhysicalDevices(instance, &count, nullptr);
    vector<VkPhysicalDevice> physicalDevices(count);
    vkEnumeratePhysicalDevices(instance, &count, physicalDevices.data());
    physicalDevice = make_unique<PhysicalDeviceContext>(*this, physicalDevices[0]);
}

void PhysicalDeviceContext::initQueueFamilies(const VkPhysicalDevice &physical_device) {
    uint32_t count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &count, nullptr);
    vector<VkQueueFamilyProperties> queueFamilyProps(count);
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &count, queueFamilyProps.data());

    queueFamilies.reserve(count);
    size_t i{};
    for (QueueFamilyInfo queueFamilyInfo : queueFamilyProps) {
        queueFamilies.emplace_back(i, queueFamilyInfo);
        i++;
    }
}

std::pair<uint32_t, QueueFamilyInfo> &PhysicalDeviceContext::queryPresentQueueFamily(VkSurfaceKHR &surface) {
    VkBool32 isSupport;
    for (auto& queueFamilyInfo : queueFamilies) {
        vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, queueFamilyInfo.first, surface, &isSupport);
        if (isSupport != VK_FALSE) return queueFamilyInfo;
    }
    return emptyQueueFamilyInfo;
}

pair<uint32_t, QueueFamilyInfo> &PhysicalDeviceContext::queryQueueFamily(VkQueueFlags flags, uint32_t requiredQuantity) {
    for (auto& queueFamilyInfo : queueFamilies) {
        bool isSatisfied = true;
        isSatisfied &= (queueFamilyInfo.second.queueFlags & flags) != 0;
        isSatisfied &= queueFamilyInfo.second.isAllowOccupation(requiredQuantity);
        if (isSatisfied) {
            return queueFamilyInfo;
        }
    }
    return emptyQueueFamilyInfo;
}
