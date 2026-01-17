#include "EvkContext.h"

#include <map>
#include <ranges>
#include <GLFW/glfw3.h>

using namespace std;

QueueFamilyInfo emptyQueueFamilyInfo{0, {1, 0, VK_QUEUE_FLAG_BITS_MAX_ENUM}};

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
    for (auto& queue_family_props : queueFamilyProps) {
        queueFamilies.emplace_back(i, queue_family_props);
        i++;
    }
}

QueueFamilyInfo &PhysicalDeviceContext::queryPresentQueueFamily(const VkSurfaceKHR &surface) {
    VkBool32 isSupport;
    for (auto& queueFamilyInfo : queueFamilies) {
        vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, queueFamilyInfo.queueFamilyIndex, surface, &isSupport);
        if (isSupport != VK_FALSE) return queueFamilyInfo;
    }
    return emptyQueueFamilyInfo;
}

QueueFamilyInfo &PhysicalDeviceContext::queryQueueFamily(const VkQueueFlags flags, const uint32_t requiredQuantity) {
    for (auto& queueFamilyInfo : queueFamilies) {
        bool isSatisfied = true;
        isSatisfied &= (queueFamilyInfo.queueFlags & flags) != 0;
        isSatisfied &= queueFamilyInfo.isAllowOccupation(requiredQuantity);
        if (isSatisfied) {
            return queueFamilyInfo;
        }
    }
    return emptyQueueFamilyInfo;
}

LogicDeviceContext::Builder &LogicDeviceContext::Builder::addLayer(const char *layer_name) {
    if (layer_name == nullptr) {
        return *this;
    }
    layerNames.push_back(layer_name);

    return *this;
}

LogicDeviceContext::Builder &LogicDeviceContext::Builder::addExtension(const char *extension_name) {
    if (extension_name == nullptr) {
        return *this;
    }
    extensionNames.push_back(extension_name);

    return *this;
}

LogicDeviceContext::Builder &LogicDeviceContext::Builder::addQueue(const string& identifier, QueueFamilyInfo &queue_family_info, uint32_t count, const float* priority) {
    if (count == 0 || priority == nullptr || identifier.empty()) {
        return *this;
    }
    VkDeviceQueueCreateInfo queueInfo{};
    queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueInfo.queueFamilyIndex = queue_family_info.queueFamilyIndex;
    queueInfo.queueCount = count;
    queueInfo.pQueuePriorities = priority;
    queueCreates.emplace_back(queueInfo);
    queueIdentifiers.push_back(identifier);

    return *this;
}

LogicDeviceContext::Builder &LogicDeviceContext::Builder::configFeatures(const VkPhysicalDeviceFeatures *enable_features) {
    if (enable_features == nullptr) {
        return *this;
    }
    features = enable_features;

    return *this;
}

LogicDeviceContext LogicDeviceContext::Builder::build() {
    VkDeviceCreateInfo deviceInfo{};
    deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceInfo.enabledLayerCount = layerNames.size();
    deviceInfo.ppEnabledLayerNames = layerNames.data();
    deviceInfo.enabledExtensionCount = extensionNames.size();
    deviceInfo.ppEnabledExtensionNames = extensionNames.data();
    deviceInfo.queueCreateInfoCount = queueCreates.size();
    deviceInfo.pQueueCreateInfos = queueCreates.data();
    deviceInfo.pEnabledFeatures = features;

    LogicDeviceContext logicDeviceContext{};
    VkResult result = vkCreateDevice(_physicalDeviceContext, &deviceInfo, nullptr, &logicDeviceContext);
    if (result != VK_SUCCESS) {
        glog.log<DefaultLevel::Error>("逻辑设备创建失败");
        terminate();
    }

    return logicDeviceContext;
}

LogicDeviceContext::Builder LogicDeviceContext::builder(PhysicalDeviceContext &physical_device_context) {
    return {physical_device_context};
}

void LogicDeviceContext::init(std::map<std::string, VkQueue> queue_map) {
    queueMap = std::move(queue_map);
}
