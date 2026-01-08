#include "EvkContext.h"

using namespace std;

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
        terminate();
    }
    _extensionNames.push_back(extensionName);
    return *this;
}

EvkContext::Builder &EvkContext::Builder::appendLayer(const char* layerName) {
    if (layerName == nullptr) {
        glog.log<DefaultLevel::Warn>("EvkContext::Builder.appendLayer [layerName]为空, 不执行");
        terminate();
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

    if (!checkExtensionSupport(extensionNames)) {
        terminate();
    }
    if (!checkLayerSupport(layerNames)) {
        terminate();
    }

    instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceInfo.pApplicationInfo = &appInfo;
    instanceInfo.enabledExtensionCount = extensionNames.size();
    instanceInfo.ppEnabledExtensionNames = extensionNames.data();
    instanceInfo.enabledLayerCount = layerNames.size();
    instanceInfo.ppEnabledLayerNames = layerNames.data();
    if (vkCreateInstance(&instanceInfo, nullptr, &_instance) != VK_SUCCESS) {
        glog.log<DefaultLevel::Error>("Vulkan 实例创建失败");
        terminate();
    }
    if (EnableDebug) {
        setupDebugMessenger();
    }
}

void EvkContext::setupDebugMessenger() {
    VkDebugUtilsMessengerCreateInfoEXT messengerInfo{};
    messengerInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    messengerInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    messengerInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    messengerInfo.pfnUserCallback = debugCallback;
    messengerInfo.pUserData = nullptr;

    auto func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(_instance, "vkCreateDebugUtilsMessengerEXT"));
    if (func != nullptr) {
        if (func(_instance, &messengerInfo, nullptr, &_debugMessenger) != VK_SUCCESS) {
            glog.log<DefaultLevel::Debug>("VulkanContext 创建调试工具信使失败");
            terminate();
        };
        return;
    } else {
        glog.log<DefaultLevel::Error>("VulkanContext 获取扩展函数[vkCreateDebugUtilsMessengerEXT]失败");
        terminate();
    }
}


bool EvkContext::checkLayerSupport(const std::vector<const char *> &layerNames) {
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
            glog.log<DefaultLevel::Warn>(format("checkLayerSupport 层:<{}>不支持", layerName));
            return false;
        }
    }
    return true;
}

bool EvkContext::checkExtensionSupport(const std::vector<const char *> &extensionNames) {
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
            glog.log<DefaultLevel::Warn>(format("checkLayerSupport 扩展:<{}>不支持", extensionName));
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
