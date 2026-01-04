#pragma once
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <typeindex>

#include <GlobalLogger.hpp>

/**
 * @brief 资源类接口
 */
class IResource {
    public:
        IResource() = default;
        IResource(const IResource&) = delete;
        IResource& operator = (const IResource&) = delete;
        IResource(IResource&&) = default;
        IResource& operator = (IResource&&) = default;
        virtual ~IResource() = default;
};

/**
 * @brief 特异化资源管理器
 * @details 专注对特定资源类型的特化型资源管理器, 类型安全
 * @tparam ResourceType 资源类型
 */
template<typename ResourceType>
class TypedResourceManager {
    static_assert(std::is_base_of_v<IResource, ResourceType>, "错误: 资源管理器模板类型[ ResourceType ]必须派生自 IResource");
    public:
        TypedResourceManager() = default;
        ~TypedResourceManager() = default;

        TypedResourceManager(const TypedResourceManager&) = delete;
        TypedResourceManager(TypedResourceManager&&) = default;
        TypedResourceManager& operator = (const TypedResourceManager&) = delete;
        TypedResourceManager& operator = (TypedResourceManager&&) = default;


        /**
         * @brief 加载资源[转发]
         * @details 使用完美转发对资源类型的构造函数进行转发, 这是提供的默认加载方式, 派生此类后可无视
         * @tparam Args 资源类型构造形数集
         * @param identifier 资源标识符
         * @param args 资源类型构造实数集
         * @return 资源引用
         */
        template<typename... Args>
        ResourceType& load(const std::string& identifier, Args&&... args) {
            resourceMap.emplace(identifier, std::make_unique<ResourceType>(std::forward<Args>(args)...));
            return *resourceMap.at(identifier);
        }


        /**
         * @brief 查找资源
         * @details 他似乎不需要详细注释[划掉]
         * @param identifier 资源标识符
         * @return 资源引用
         */
        ResourceType& find(const std::string& identifier) {
            return *resourceMap.at(identifier);
        }

        /**
         * @brief 通过下标操作符查找资源
         * @param identifier 资源标识符
         * @return 资源引用
         */
        ResourceType& operator [] (const std::string& identifier) {
            return find(identifier);
        }

    protected:
        std::map<std::string, std::unique_ptr<ResourceType>> resourceMap;
};

/**
 * @brief 泛化资源管理器
 * @details 通过类型擦除实现对所有资源类型的适配, 但类型不安全
 */
class AnyResourceManager {
    public:
        AnyResourceManager() = default;
        ~AnyResourceManager() = default;

        AnyResourceManager(const AnyResourceManager&) = delete;
        AnyResourceManager(AnyResourceManager&&) = default;
        AnyResourceManager& operator = (const AnyResourceManager&) = delete;
        AnyResourceManager& operator = (AnyResourceManager&&) = default;

        /**
         * @brief 加载资源[转发]
         * @details 使用完美转发对资源类型的构造函数进行转发
         * @tparam ResourceType 资源类型
         * @tparam Args 资源类型构造形参集
         * @param identifier 资源标识符
         * @param args 资源类型构造实参集
         * @return 资源引用
         */
        template<typename ResourceType, typename... Args>
            ResourceType& load(const std::string& identifier, Args&&... args) {
            auto it = resourceMap[typeid(ResourceType)].emplace(identifier, std::make_unique<ResourceWrapper<ResourceType>>(std::forward<Args>(args)...));
            return find<ResourceType>(identifier);
        }

        /**
         * @brief 查找资源
         * @details 他似乎不需要详细注释[划掉]
         * @tparam ResourceType 资源类型
         * @param identifier 资源标识符
         * @return 资源引用
         */
        template<typename ResourceType>
        ResourceType& find(const std::string& identifier) {
            auto type_it = resourceMap.find(typeid(ResourceType));
            if (type_it == resourceMap.end()) {
                glog.log<DefaultLevel::Error>("查找未知类型资源");
                std::terminate();
            }
            auto identifier_it = type_it->second.find(identifier);
            if (identifier_it == type_it->second.end()) {
                glog.log<DefaultLevel::Error>("查找未知标识符资源");
                std::terminate();
            }
            return *static_cast<ResourceType *>(dynamic_cast<ResourceWrapper<ResourceType> &>(*identifier_it->second).value());
        }

        /**
         * @brief 通过下标操作符查找资源
         * @details 他似乎不需要详细注释[划掉], 而且这个重载很没用, 没法隐式调用
         * @tparam ResourceType 资源类型
         * @param identifier 资源标识符
         * @return 资源引用
         */
        template<typename ResourceType>
        ResourceType& operator [] (const std::string& identifier) {
            return find<ResourceType>(identifier);
        }

    protected:
        /**
         * @brief 类型擦除包装器基
         * @details 他似乎不需要详细注释[划掉]
         */
        class BaseWrapper {
            public:
                virtual ~BaseWrapper() = default;
                virtual void* value() {
                    return nullptr;
                }
        };

        /**
         * @brief 资源类型擦除包装器
         * @details 他似乎不需要详细注释[划掉]
         * @tparam ResourceType 资源类型
         */
        template<typename ResourceType>
        class ResourceWrapper: public BaseWrapper {
            public:
                template<typename ... Args>
                ResourceWrapper(Args... args) {
                    resource = std::make_unique<ResourceType>(std::forward<Args>(args)...);
                }
                ~ResourceWrapper() override = default;
                void* value() override {
                    return resource.get();
                }
            private:
                std::unique_ptr<ResourceType> resource;
        };

        std::map<std::type_index, std::map<std::string, std::unique_ptr<BaseWrapper>>> resourceMap;
};