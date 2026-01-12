#pragma once
#include <tuple>

template<typename... Returns>
std::tuple<Returns...> constexpr returns(Returns&&... returns) noexcept {
    return {std::forward<Returns>(returns)...};
}
