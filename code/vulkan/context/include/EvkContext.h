#pragma once

#include "VulkanTypes.hpp"

template<typename T, typename Type = std::decay_t<T>>
Type& reference_cast(T& _value) {
    return _value;
}

class VulkanContext: public raii::VkRAIIWrapper<raii::VkInstance>{
    public:
        raii::VkInstance& instance = _value;

        VulkanContext();

        template<typename Func, typename... Args>
        raii::VkSurfaceKHR createSurface(Func&& func, Args&&... args) {
            raii::VkSurfaceKHR surface{instance};
            func(reference_cast(instance), reference_cast(surface), std::forward<Args>(args)...);
            return surface;
        }
};