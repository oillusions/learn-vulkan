#pragma once
#include <algorithm>

/**
 * @brief RAII包装器
 * @tparam CarriedType 包装类型
 * @tparam Copy 启用拷贝
 */
template<typename CarriedType, bool Copy = false>
class RAIIWrapper;

template<typename CarriedType, bool Copy>
class RAIIWrapper {
    public:
        template<typename T = CarriedType, typename = std::enable_if_t<std::is_default_constructible_v<T>>>
        RAIIWrapper(): _value() {};
        RAIIWrapper(CarriedType value): _value(std::move(value)) {};
        virtual ~RAIIWrapper() = default;
        RAIIWrapper(RAIIWrapper&&) = default;
        RAIIWrapper& operator = (RAIIWrapper&&) = default;

        template<bool Enable = Copy, typename = std::enable_if_t<Enable>>
        RAIIWrapper(const RAIIWrapper& other): _value(other) {};
        template<bool Enable = Copy, typename = std::enable_if_t<Enable>>
        RAIIWrapper& operator = (const RAIIWrapper& other) {
            if (this != &other) {
                _value = other._value;
            }
            return *this;
        }

        CarriedType& get() {
            return _value;
        }

        const CarriedType& get() const {
            return _value;
        }

        operator CarriedType& () {
            return _value;
        }

        operator const CarriedType& () const {
            return _value;
        }

        CarriedType* operator & () {
            return &_value;
        }

        CarriedType* operator -> () {
            return &_value;
        }

        const CarriedType* operator & () const {
            return &_value;
        }

        const CarriedType* operator -> () const {
            return &_value;
        }

        bool operator == (const RAIIWrapper& other) const {return _value == other._value;}
        bool operator != (const RAIIWrapper& other) const {return _value != other._value;}
    protected:
        CarriedType _value;
};