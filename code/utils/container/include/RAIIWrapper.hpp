#pragma once
#include <algorithm>

/**
 * @brief RAII包装器
 * @tparam CarriedType 包装值
 */
template<typename CarriedType>
class RAIIWrapper {
    public:
        template<typename T = CarriedType, typename = std::enable_if_t<std::is_default_constructible_v<T>>>
        RAIIWrapper(): _value() {};
        RAIIWrapper(CarriedType value): _value(std::move(value)) {};
        virtual ~RAIIWrapper() = default;
        RAIIWrapper(const RAIIWrapper&) = delete;
        RAIIWrapper(RAIIWrapper&&) = default;
        RAIIWrapper& operator = (const RAIIWrapper&) = delete;
        RAIIWrapper& operator = (RAIIWrapper&&) = default;

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
    protected:
        CarriedType _value;
};
