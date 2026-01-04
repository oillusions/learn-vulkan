#pragma once

#include <algorithm>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <iostream>

#include "GlobalLogger.hpp"

/**
 * @brief 缓冲区组装布局
 * @tparam T 缓冲区类型
 */
template<typename T>
class VertexLayout {
    public:
        /**
         * @brief 布局元素
         */
        class LayoutElement {
            public:
                std::vector<T> _source;
                std::string identifier;
                size_t length;
                size_t location{};
                size_t origin{};
                size_t step{};
                bool* _isDirty{};

                /**
                 * @brief 元素构造
                 * @details 他似乎不需要详细注释[划掉]
                 * @param identifier 元素标识符
                 * @param length 元素长度
                 * @param origin 元素原点
                 * @param location 元素位置
                 */
                LayoutElement(const std::string& identifier, size_t length, size_t origin, size_t location):
                    identifier(identifier),
                    length(length),
                    location(location),
                    origin(origin * sizeof(T)) {}

                ~LayoutElement() = default;

                /**
                 * @brief 脏标记
                 * @details 他似乎不需要详细注释[划掉]
                 */
                void dirty() const {
                    if (_isDirty != nullptr) {
                        *_isDirty = true;
                    }
                }

                /**
                 * @brief 配置数据源[拷贝]
                 * @details 他似乎不需要详细注释[划掉]
                 * @param source 数据源
                 */
                void setSource(const std::vector<T>& source) {
                    if (_source == source) return;
                    _source = source;
                    dirty();
                }

                /**
                 * @brief 配置数据源[移动]
                 * @details 他似乎不需要详细注释[划掉]
                 * @param source 数据源
                 */
                void setSource(std::vector<T>&& source) {
                    if (_source == source) return;
                    _source = std::move(source);
                    dirty();
                }

                /**
                 * @brief 获取当前数据源数组引用
                 * @details 他似乎不需要详细注释[划掉]
                 * @return 数据源引用
                 */
                const std::vector<T>& getSource() const {
                    return _source;
                }

                /**
                 * @brief 通过类型转换操作符获取数据源数组引用
                 * @details 他似乎不需要详细注释[划掉]
                 * @return 数据源引用
                 */
                operator const std::vector<T>& () const {
                    return getSource();
                }

                /**
                 * @brief 通过赋值操作符配置数据源[拷贝]
                 * @details 他似乎不需要详细注释[划掉]
                 * @param source 数据源
                 * @return 布局元素本身
                 */
                LayoutElement& operator = (const std::vector<T>& source) {
                    setSource(source);
                    return *this;
                }

                /**
                 * @brief 通过下标下标操作符下标数据源
                 * @details 他似乎不需要详细注释[划掉]
                 * @param index 下标
                 * @return 数据源数据
                 */
                const T& operator [] (size_t index) const {
                    return _source[index];
                }
            };

        class Builder {
            public:
                /**
                 * @brief 缓冲区组装布局构建者构造
                 * @details 他似乎不需要详细注释[划掉]
                 */
                Builder() {
                    elements = std::vector<LayoutElement>();
                    identifierMap = std::map<std::string, size_t>();
                }
                ~Builder() = default;

                /**
                 * @brief 预分配元素内存
                 * @details 他似乎不需要详细注释[划掉]
                 * @param number 元素数量
                 * @return 构建者引用
                 */
                Builder& elementNumber(size_t number) {
                    if (number != 0) {
                        elements.resize(number);
                    }
                    return *this;
                }

                /**
                 * @brief 添加布局元素
                 * @details 他似乎不需要详细注释[划掉]
                 * @param identifier 元素标识符
                 * @param length 元素长度
                 * @return 构建者引用
                 */
                Builder& appendElement(const std::string& identifier, size_t length) {
                    if (identifier.empty()) {
                        glog.log<DefaultLevel::Warn>("错误: 标识符为空: " + identifier);
                        return *this;
                    }
                    if (length == 0) {
                        glog.log<DefaultLevel::Warn>("错误: 元素为空: " + std::to_string(length));
                        return *this;
                    }
                    elements.emplace_back(LayoutElement(identifier, length, originCounter, locationCounter));
                    identifierMap.emplace(identifier, locationCounter);
                    locationCounter++;
                    originCounter += length;
                    return *this;
                }

                /**
                 * @brief 向布局元素附加数据源[拷贝]
                 * @details 他似乎不需要详细注释[划掉]
                 * @param identifier 元素标识符
                 * @param source 数据源
                 * @return 构建者引用
                 */
                Builder& attachSource(const std::string& identifier, const std::vector<T>& source) {
                    if (elements.empty()) {
                        glog.log<DefaultLevel::Warn>("错误: 当前未拥有任何元素");
                        return *this;
                    }
                    auto index = identifierMap.find(identifier);
                    if (index == identifierMap.end()) {
                        glog.log<DefaultLevel::Warn>("错误: 当前未拥有[" + identifier + "]标识符的元素");
                        return *this;
                    }
                    elements.at(index->second).setSource(source);
                    return *this;
                }

                /**
                 * @brief 向布局元素附加数据源[移动]
                 * @details 他似乎不需要详细注释[划掉]
                 * @param identifier 元素标识符
                 * @param source 数据源
                 * @return 构建者引用
                 */
                Builder& attachSource(const std::string& identifier, std::vector<T>&& source) {
                    if (elements.empty()) {
                        glog.log<DefaultLevel::Warn>("错误: 当前未拥有任何元素: " + identifier);
                        return *this;
                    }
                    auto index = identifierMap.find(identifier);
                    if (index == identifierMap.end()) {
                        glog.log<DefaultLevel::Warn>("错误: 当前未拥有[" + identifier + "]标识符的元素");
                        return *this;
                    }
                    elements.at(index->second).setSource(std::move(source));
                    return *this;
                }

                /**
                 * @brief 向缓冲区布局附加索引源[拷贝]
                 * @details 他似乎不需要详细注释[划掉]
                 * @param indices 索引源
                 * @return 构建者引用
                 */
                Builder& attachIndices(const std::vector<unsigned int>& indices) {
                    rawIndices = indices;
                    return *this;
                }

                /**
                 * @brief 向缓冲区布局附加索引源[移动]
                 * @details 他似乎不需要详细注释[划掉]
                 * @param indices 索引源
                 * @return 构建者引用
                 */
                Builder& attachIndices(std::vector<unsigned int>&& indices) {
                    rawIndices = std::move(indices);
                    return *this;
                }

                /**
                 * @brief 查找构建者中是否包含元素
                 * @details 他似乎不需要详细注释[划掉]
                 * @param identifier 元素标识符
                 * @return 构建者引用
                 */
                bool contain(const std::string& identifier) {
                    if (elements.empty()) return false;
                    if (identifierMap.find(identifier) == identifierMap.end()) return false;
                    return true;
                }

                /**
                 * @brief 构建缓冲区组装布局
                 * @details 他似乎不需要详细注释[划掉]
                 * @return 缓冲区组装布局
                 */
                VertexLayout build() {
                    if (elements.empty()) {
                        glog.log<DefaultLevel::Warn>("错误: 构建时未拥有任何元素");
                        std::terminate();
                    }
                    for (auto& e : elements) {
                        e.step = originCounter * sizeof(T);
                    }
                    return VertexLayout(std::move(elements), std::move(identifierMap), std::move(rawIndices));
                }

            private:
                std::vector<LayoutElement> elements;
                std::map<std::string, size_t> identifierMap;
                std::vector<unsigned int> rawIndices;
                size_t locationCounter{0};
                size_t stepCounter{0};
                size_t originCounter{0};
        };

        /**
         * @brief 获取缓冲区组装布局构建者
         * @details 他似乎不需要详细注释[划掉]
         * @return 构建者
         */
        static Builder builder() {
            return Builder();
        }

        /**
         * @brief 缓冲区组装布局构造
         * @details 他似乎不需要详细注释[划掉]
         * @param layout 布局元素数组
         */
        explicit VertexLayout(std::vector<LayoutElement> layout):
                VertexLayout(std::move(layout), std::move(std::map<std::string, size_t>())) {}

        VertexLayout(const VertexLayout&) = delete;
        VertexLayout& operator=(const VertexLayout&) = delete;

        /**
         * @brief 缓冲区组装布局构造[移动]
         * @details 他似乎不需要详细注释[划掉]
         * @param other 移动源
         */
        VertexLayout(VertexLayout&& other) noexcept :
            _layout(std::move(other._layout)),
            _cache(std::move(other._cache)),
            _indices(std::move(other._indices)),
            _rawIndices(std::move(other._rawIndices)),
            _identifierMap(std::move(other._identifierMap)),
            _isDirty(true)
            {
                size_t index{0};
                for (LayoutElement& e : _layout) {
                    e._isDirty = &this->_isDirty;
                    _identifierMap.emplace(e.identifier, index);
                    index++;
                }
            };

        /**
         * @brief 通过赋值操作符进行缓冲区组装布局移动
         * @details 他似乎不需要详细注释[划掉]
         * @param other 移动源
         * @return 缓冲区组装布局引用
         */
        VertexLayout& operator=(VertexLayout&& other) noexcept {
            if (this != &other) {
                _layout = std::move(other._layout);
                _cache = std::move(other._cache);
                _indices = std::move(other._indices);
                _rawIndices = std::move(other._rawIndices);
                _identifierMap = std::move(other._identifierMap);
                _isDirty = true;
                for (LayoutElement& e : _layout) {
                    e._isDirty = &this->_isDirty;
                }
                _isDirty = true;
            }
            return *this;
        }

        ~VertexLayout() = default;

        void test() {
            for (const auto& e : _layout) {
                std::cout << "identifier: " << e.identifier << ", location: " << e.location << ",length: " << e.length << ", origin: " << e.origin << ", step:" << e.step << std::endl;
            }
        }

        /**
         * @brief 向opengl声明当前缓冲区结构
         * @details 他似乎不需要详细注释[划掉]
         */
        void bufferLayoutDeclaration() {
            if (_layout.empty()) {
                glog.log<DefaultLevel::Warn>("错误: 当前未拥有任何元素");
                return;
            }
        }

        /**
         * @brief 通过location顺序组装缓冲区
         * @details 他似乎不需要详细注释[划掉]
         * @return 缓冲区引用
         */
        const std::vector<T>& assemblyBuffer() const {
            if (!_isDirty) {
                return _cache;
            }
            size_t size{};
            for (const auto& e : _layout) {
                size += e._source.size();
            }
            _cache.clear();
            _cache.reserve(size);
            _indices.clear();
            _indices.reserve(size);

            size_t stepCounter{};
            size_t indexCounter{};
            size_t i{};
            while (true) {
                for (auto& e : _layout) {
                    _cache.insert(_cache.end(), e.length, 0);
                    std::copy_n(e._source.begin() + (e.length * stepCounter),
                    e.length,
                    _cache.begin() + i
                    );
                    i += e.length;
                }
                stepCounter++;
                _indices.push_back(indexCounter);
                indexCounter++;
                if (i >= size) break;
            }

            _isDirty = false;
            return _cache;
        }

        /**
         * @brief 配置索引源[拷贝]
         * @details 他似乎不需要详细注释[划掉]
         * @param indexStr 索引源
         */
        void indices(const std::vector<unsigned int>& indexStr) {
            if (!indexStr.empty()) {
                _rawIndices = indexStr;
            }
        }

        /**
         * @brief 配置索引源[移动]
         * @details 他似乎不需要详细注释[划掉]
         * @param indexStr 索引源
         */
        void indices(std::vector<unsigned int>&& indexStr) {
            if (!indexStr.empty()) {
                _rawIndices = std::move(indexStr);
            }
        }

        /**
         * @brief 获取原始索引数组
         * @details 他似乎不需要详细注释[划掉]
         * @return 原始索引数组
         */
        std::vector<unsigned int> rawIndices() const {
            return _rawIndices;
        }


        /**
         * @brief 通过索引组装缓冲区
         * @details 他似乎不需要详细注释[划掉]
         * @return 缓冲区引用
         */
        const std::vector<T>& ExpandIndices() const {
            if (!_isDirty) {
                return _cache;
            }
            if (_rawIndices.empty()) {
                glog.log<DefaultLevel::Warn>("错误: 使用索引缓冲区组装时索引未空");
                return _cache;
            }

            size_t size = (_rawIndices.size() / _layout.size()) * (_layout[0].step / sizeof(T));
            _cache.clear();
            _cache.reserve(size);
            _indices.clear();
            _indices.reserve(_rawIndices.size() / (_layout[0].step / sizeof(T)));

            size_t i{};
            size_t stepCounter{0};
            size_t elementIndex{0};
            size_t indexCounter{0};
            for (const auto& index : _rawIndices) {
                auto& e = _layout.at(elementIndex);
                _cache.insert(_cache.end(), e.length, 0);
                std::copy_n(e._source.begin() + (e.length * index),
                    e.length,
                    _cache.begin() + i
                    );
                i += e.length;
                stepCounter++;
                elementIndex++;
                elementIndex %= _layout.size();

                if (elementIndex == 0) {
                    _indices.push_back(indexCounter);
                    indexCounter++;
                }
            }
            _isDirty = false;
            return _cache;
        }

        /**
         * @brief 获取通过索引组装的缓冲区的索引[毫无作用][划掉]
         * @details 他似乎不需要详细注释[划掉]
         * @return 索引数组引用
         */
        const std::vector<unsigned int>& bufferOfIndices() const {
            return _indices;
        }

        /**
         * @brief 缓冲区组装布局是否包含元素
         * @details 他似乎不需要详细注释[划掉]
         * @param identifier 元素标识符
         * @return 他似乎不需要注释[划掉]
         */
        bool contain(const std::string& identifier) const {
            if (_layout.empty()) return false;
            if (_identifierMap.find(identifier) == _identifierMap.end()) return false;
            return true;
        }

        /**
         * @brief 通过下标操作符查找并获取布局元素
         * @details 他似乎不需要详细注释[划掉]
         * @param identifier 元素标识符
         * @return 元素引用
         */
        LayoutElement& operator [] (const std::string& identifier) {
            if (_layout.empty()) {
                glog.log<DefaultLevel::Error>("错误: 当前未拥有任何元素");
                std::terminate();
            }
            auto index = _identifierMap.find(identifier);
            if (index == _identifierMap.end()) {
                glog.log<DefaultLevel::Warn>("错误: 当前未拥有[" + identifier + "]标识符的元素");
                throw std::runtime_error("当前未拥有[" + identifier + "]标识符的元素");
            }
            return _layout.at(index->second);
        }


    private:
        std::vector<LayoutElement> _layout;
        std::map<std::string, size_t> _identifierMap;
        mutable std::vector<T> _cache;
        mutable std::vector<unsigned int> _indices;
        std::vector<unsigned int> _rawIndices;
        mutable bool _isDirty{true};

        /**
         * @brief 构建者使用的缓冲区组装布局构造
         * @details 他似乎不需要详细注释[划掉]
         * @param layout 布局元素
         * @param identifierMap 布局元素索引
         * @param rawIndices 原始索引
         */
        explicit VertexLayout(std::vector<LayoutElement> layout, std::map<std::string, size_t> identifierMap, std::vector<unsigned int> rawIndices):
            _layout(std::move(layout)),
            _identifierMap(std::move(identifierMap)),
            _cache(std::vector<T>()),
            _indices(std::vector<unsigned int>()),
            _rawIndices(std::move(rawIndices)),
            _isDirty(true) {
            size_t index{0};
            for (LayoutElement& e : _layout) {
                e._isDirty = &this->_isDirty;
                _identifierMap.emplace(e.identifier, index);
                index++;
            }
        }
        explicit VertexLayout(std::vector<LayoutElement> layout, std::map<std::string, size_t> identifierMap):
            VertexLayout(std::move(layout), std::move(identifierMap), std::move(std::vector<unsigned int>())) {};
};