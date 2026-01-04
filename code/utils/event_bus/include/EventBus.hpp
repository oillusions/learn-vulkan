#pragma once
#include <iostream>
#include <typeindex>
#include <functional>
#include <memory>
#include <map>

class EventBus {
    public:
        EventBus() = default;
        ~EventBus() = default;

        /**
         * @brief 发布事件
         * @details 他似乎不需要详细注释[划掉]
         * @tparam EventType 事件类型
         * @param identifier 事件标识符
         * @param content 事件内容
         */
        template<typename EventType>
        void publish(const std::string& identifier, EventType& content) const {
            if (identifier.empty()) return;
            if (_subscriptionMap.empty()) return;

            if (const auto typeMap_it = _subscriptionMap.find(typeid(EventType)); typeMap_it != _subscriptionMap.end()) {
                if (typeMap_it->second.empty()) return;
                if (const auto identifierMap_it = typeMap_it->second.find(identifier); identifierMap_it != typeMap_it->second.end()) {
                    if (identifierMap_it->second.empty()) return;
                    for (auto& [call, id] : identifierMap_it->second) {
                        if (auto* wrapper = dynamic_cast<CallWrapper<EventType&>*>(call.get())) {
                            wrapper->call(content);
                        }
                    }
                }
            }
        }

        /**
         * @brief 订阅事件
         * @details 他似乎不需要详细注释[划掉]
         * @tparam EventType 事件类型
         * @param identifier 事件标识符
         * @param call 事件回调
         * @return 回调ID
         */
        template<typename EventType>
        size_t subscribe(const std::string& identifier, std::function<void(EventType&)> call) const {
            _subscriptionMap[typeid(EventType)][identifier].emplace_back(CallInfo{std::make_unique<CallWrapper<EventType&>>(call), _idCounter});
            _idCounter++;
            return _idCounter - 1;
        }

        /**
         * @brief 退订事件
         * @details 他似乎不需要详细注释[划掉]
         * @tparam EventType 事件类型
         * @param identifier 事件标识符
         * @param id 事件id
         */
        template<typename EventType>
        void unsubscribe(const std::string& identifier, size_t id) {
            auto type_it = _subscriptionMap.find(typeid(EventType));
            if (type_it == _subscriptionMap.end()) return;

            auto identifier_it = type_it->second.find(identifier);
            if (identifier_it == type_it->second.end()) return;

            for (auto i = identifier_it->second.begin(); i != identifier_it->second.end(); i++) {
                if (i->id == id) {
                    identifier_it->second.erase(i);
                    return;
                }
            }
        }

    private:
        /**
         * @brief 回调类型擦除基类
         * @details 他似乎不需要详细注释[划掉]
         */
        class CallBase {
            public:
                virtual ~CallBase() = default;
                virtual void call() {
                    std::cout << "空" << std::endl;
                }
            };

        /**
         * @brief 类型擦除回调包装器
         * @tparam CallType 回调类型
         */
        template<typename CallType>
        class CallWrapper: public CallBase{
            public:
                CallWrapper(std::function<void(CallType&)> call): _call(call) {}
                ~CallWrapper() override = default;

                void call(CallType& content) {
                    _call(content);
                }

                void operator () (CallType& content) {
                    call(content);
                }
            private:
                std::function<void(CallType&)> _call;
        };

        /**
         * @brief 回调信息
         */
        struct CallInfo {
            std::unique_ptr<CallBase> call;
            size_t id;
        };


        mutable std::map<std::type_index, std::map<std::string, std::vector<CallInfo>>> _subscriptionMap;
        mutable size_t _idCounter{};

};
