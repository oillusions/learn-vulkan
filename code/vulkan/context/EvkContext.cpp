#include "EvkContext.h"

using namespace std;

const vector deviceExtensions {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

EvkContext::Builder &EvkContext::Builder::appName(const std::string &name) {
    if (name.empty()) {
        glog.log<DefaultLevel::Warn>("EvkContext::Builder.appName [name]为空, 不执行");
    } else {
        _appInfo.pApplicationName = name.c_str();
    }
    return *this;
}

EvkContext::Builder &EvkContext::Builder::appVersion(uint32_t version) {
    _appInfo.applicationVersion = version;
    return *this;
}

EvkContext::Builder &EvkContext::Builder::apiVersion(APIVersion version) {
    _appInfo.apiVersion = static_cast<uint32_t>(version);
    return *this;
}


EvkContext::Builder &EvkContext::Builder::appendExtension(const char* extensionName) {
    if (extensionName == nullptr) {
        glog.log<DefaultLevel::Warn>("EvkContext::Builder.appendExtension [extensionName]为空, 不执行");
        return *this;
    }
    _extensionNames.push_back(extensionName);
    return *this;
}

EvkContext::Builder &EvkContext::Builder::appendLayer(const char* layerName) {
    if (layerName == nullptr) {
        glog.log<DefaultLevel::Warn>("EvkContext::Builder.appendLayer [layerName]为空, 不执行");
        return *this;
    }
    _layerNames.push_back(layerName);
    return *this;
}

EvkContext EvkContext::Builder::build() {
    return EvkContext(_appInfo, std::move(_extensionNames), std::move(_layerNames));
}

EvkContext::EvkContext(const VkApplicationInfo &appInfo, std::vector<const char*> extensionNames, std::vector<const char*> layerNames) {
    VkInstanceCreateInfo instanceInfo{};
    if (EnableDebug) {
        layerNames.push_back("VK_LAYER_KHRONOS_validation");
        extensionNames.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    if (!checkInstanceExtensionSupport(extensionNames)) {
        terminate();
    }
    if (!checkInstanceLayerSupport(layerNames)) {
        terminate();
    }

    instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceInfo.pApplicationInfo = &appInfo;
    instanceInfo.enabledExtensionCount = extensionNames.size();
    instanceInfo.ppEnabledExtensionNames = extensionNames.data();
    instanceInfo.enabledLayerCount = layerNames.size();
    instanceInfo.ppEnabledLayerNames = layerNames.data();
    if (vkCreateInstance(&instanceInfo, nullptr, &_value) != VK_SUCCESS) {
        glog.log<DefaultLevel::Error>("Vulkan 实例创建失败");
        terminate();
    }
    if (EnableDebug) {
        setupDebugMessenger();
    }

    initPhysicalDeviceMap();
}

void EvkContext::setupDebugMessenger() {
    VkDebugUtilsMessengerCreateInfoEXT messengerInfo{};
    messengerInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    messengerInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    messengerInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    messengerInfo.pfnUserCallback = debugCallback;
    messengerInfo.pUserData = nullptr;

    auto func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(_value, "vkCreateDebugUtilsMessengerEXT"));
    if (func != nullptr) {
        if (func(_value, &messengerInfo, nullptr, &_debugMessenger) != VK_SUCCESS) {
            glog.log<DefaultLevel::Warn>("VulkanContext 创建调试工具信使失败");
        };
        return;
    } else {
        glog.log<DefaultLevel::Warn>("VulkanContext 获取扩展函数[vkCreateDebugUtilsMessengerEXT]失败");
    }
}

void EvkContext::initPhysicalDeviceMap() {
    uint32_t count{};
    vkEnumeratePhysicalDevices(_value, &count, nullptr);
    if (count == 0) {
        glog.log<DefaultLevel::Error>("EvkContext 未找到设备");
        terminate();
    }
    vector<VkPhysicalDevice> physicalDevices(count);
    vkEnumeratePhysicalDevices(_value, &count, physicalDevices.data());

    for (auto physicalDevice : physicalDevices) {
        VkPhysicalDeviceProperties properties{};
        vkGetPhysicalDeviceProperties(physicalDevice, &properties);
        _physicalDevices.emplace_back(make_unique<EvkPhysicalDeviceContext>(*this, physicalDevice));
    }
    if (count == 1) {
        _defaultPhysicalDevice = _physicalDevices.begin()->get();
    } else {
        pickDefaultPhysicalDevice();
    }
}

void EvkContext::pickDefaultPhysicalDevice() {
    for (auto& physicalDevice : _physicalDevices) {
        bool isPass{true};
        isPass &= checkPhysicalDeviceExtensionSupport(*physicalDevice, deviceExtensions);
        if (isPass) {
            _defaultPhysicalDevice = physicalDevice.get();
            return;
        }
    }
}


bool EvkContext::checkInstanceLayerSupport(const std::vector<const char *> &layerNames) {
    uint32_t layerCount{};
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
    vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName : layerNames) {
        bool layerFound = false;
        for (const auto& layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }
        if (!layerFound) {
            glog.log<DefaultLevel::Warn>(format("checkInstanceLayerSupport 层:<{}>不支持", layerName));
            return false;
        }
    }
    return true;
}

bool EvkContext::checkInstanceExtensionSupport(const std::vector<const char *> &extensionNames) {
    uint32_t extensionCount{};
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, availableExtensions.data());

    for (const char* extensionName : extensionNames) {
        bool extensionFound = false;
        for (const auto& extensionProperties : availableExtensions) {
            if (strcmp(extensionName, extensionProperties.extensionName) == 0) {
                extensionFound = true;
                break;
            }
        }
        if (!extensionFound) {
            glog.log<DefaultLevel::Warn>(format("checkInstanceExtensionSupport 扩展:<{}>不支持", extensionName));
            return false;
        }
    }
    return true;
}

bool EvkContext::checkPhysicalDeviceExtensionSupport(const VkPhysicalDevice &physicalDevice, const std::vector<const char *> &extensionNames) {
    uint32_t extensionCount{};
    vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);
    vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, availableExtensions.data());

    for (const char* extensionName : extensionNames) {
        bool extensionFound = false;
        for (const auto& extensionProperties : availableExtensions) {
            if (strcmp(extensionName, extensionProperties.extensionName) == 0) {
                extensionFound = true;
                break;
            }
        }
        if (!extensionFound) {
            glog.log<DefaultLevel::Warn>(format("checkPhysicalDeviceExtensionSupport 扩展:<{}>不支持", extensionName));
            return false;
        }
    }
    return true;
}

VkBool32 EvkContext::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData) {
    switch (messageSeverity) {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT: {
            glog.log<DefaultLevel::Debug>("验证层: " + string(pCallbackData->pMessage));
            break;
        }
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT: {
            glog.log<DefaultLevel::Info>("验证层: " + string(pCallbackData->pMessage));
            break;
        }
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT: {
            glog.log<DefaultLevel::Warn>("验证层: " + string(pCallbackData->pMessage));
            break;
        }
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT: {
            glog.log<DefaultLevel::Error>("验证层: " + string(pCallbackData->pMessage));
            break;
        }
        default:;
    }
    return VK_FALSE;
}


EvkLogicDeviceContext::Builder &EvkLogicDeviceContext::Builder::appendQueue(uint32_t queueFamilyIndex, float priority) {
    auto& queueFamilies = _logicDeviceContext._physicalDeviceContext._queueFamilies;
    if (queueFamilyIndex < queueFamilies.size()) {
        glog.log<DefaultLevel::Warn>("EvkLogicDeviceContext::Builder.appendQueue [queueFamilyIndex]越界, 不执行");
        return *this;
    }
    auto& queueInfo = _queueCreateInfos.emplace_back();
    queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueInfo.queueCount = 1;
    queueInfo.queueFamilyIndex = queueFamilyIndex;
    queueInfo.pQueuePriorities = &priority;
    return *this;
}

EvkLogicDeviceContext::Builder &EvkLogicDeviceContext::Builder::appendExtension(const char *extensionName) {
    if (extensionName == nullptr) {
        glog.log<DefaultLevel::Warn>("EvkLogicDeviceContext::Builder.appendExtension [extensionName]为空, 不执行");
        return *this;
    }
    _extensionNames.push_back(extensionName);
    return *this;
}

EvkLogicDeviceContext::Builder &EvkLogicDeviceContext::Builder::appendLayer(const char *layerName) {
    if (layerName == nullptr) {
        glog.log<DefaultLevel::Warn>("EvkLogicDeviceContext::Builder.appendLayer [layerName]为空, 不执行");
        return *this;
    }
    _layerNames.push_back(layerName);
    return *this;
}

EvkLogicDeviceContext::Builder &EvkLogicDeviceContext::Builder::setupFeatures(const VkPhysicalDeviceFeatures &features) {
    _deviceInfo.pEnabledFeatures = &features;
    return *this;
}

EvkLogicDeviceContext &EvkLogicDeviceContext::Builder::build() {
    _deviceInfo.queueCreateInfoCount = _queueCreateInfos.size();
    _deviceInfo.pQueueCreateInfos = _queueCreateInfos.data();
    _deviceInfo.enabledExtensionCount = _extensionNames.size();
    _deviceInfo.ppEnabledExtensionNames = _extensionNames.data();
    _deviceInfo.enabledLayerCount = _layerNames.size();
    _deviceInfo.ppEnabledLayerNames = _layerNames.data();
    if (vkCreateDevice(_logicDeviceContext._physicalDeviceContext, &_deviceInfo, nullptr, &_logicDeviceContext.get())) {
        glog.log<DefaultLevel::Error>("EvkLogicDeviceContext::Builder 无法创建逻辑设备");
        terminate();
    }
    return _logicDeviceContext;
}


EvkPhysicalDeviceContext::EvkPhysicalDeviceContext(EvkContext &vkContext, VkPhysicalDevice physicalDevice):
    _vkContext(vkContext) {
    _value = physicalDevice;

    uint32_t queueFamilyCount{};
    vkGetPhysicalDeviceQueueFamilyProperties(_value, &queueFamilyCount, nullptr);
    // std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    _queueFamilies.resize(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(_value, &queueFamilyCount, _queueFamilies.data());
}

raii::VkSurfaceKHR &EvkPhysicalDeviceContext::findSurface(const std::string &identifier) {
    auto it = _surfaceMap.find(identifier);
    if (it == _surfaceMap.end()) {
        glog.log<DefaultLevel::Warn>(format("EvkPhysicalDeviceContext 不存在<{}>表面", identifier));
        terminate();
    }
    return it->second;
}