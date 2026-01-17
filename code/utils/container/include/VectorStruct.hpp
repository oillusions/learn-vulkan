#pragma once
#include <tuple>

template<typename... Returns>
constexpr std::tuple<Returns...> returns(Returns&&... returns) noexcept {
    return {std::forward<Returns>(returns)...};
}
