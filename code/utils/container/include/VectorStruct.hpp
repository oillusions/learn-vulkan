#pragma once
#include <tuple>
#include <cinttypes>

template<typename... Members>
struct VectorStruct {
    std::tuple<Members...> memberPack;

    template<typename... Args>
    VectorStruct(Args&&... args): memberPack(std::forward<Args>(args)...) {};

    template<size_t N>
    auto& get() {
        return std::get<N>(memberPack);
    }

    template<size_t N>
    const auto& get() const {
        return std::get<N>(memberPack);
    }
};
