#pragma once
#include <list>
#include <map>
#include <memory>
#include <string>

#include <GlobalLogger.hpp>
#include <Interfaces.hpp>

#include "Transform.h"

class LayerSceneElement {
    public:
        struct Composite {
            Transform common{};
            std::unique_ptr<Drawable> carried{nullptr};
        };
        LayerSceneElement(const std::string& identifier, Composite value = {Transform{}, nullptr}):
            _depth(0),
            _isRoot(true),
            _identifier(identifier),
            _parent(nullptr),
            _value(std::make_unique<Composite>(std::move(value))),
            _elementMap(),
            _sceneElements(std::make_shared<std::list<LayerSceneElement>>()) {
            if (identifier.empty()) {
                glog.log<DefaultLevel::Warn>("标识符长度为零");
            }
        };

        LayerSceneElement& addChild(const std::string& identifier, Composite value = {Transform{}, nullptr}) {
            if (identifier.empty()) {
                glog.log<DefaultLevel::Warn>("标识符长度为零");
                return *this;
            }
            _sceneElements->emplace_back(std::move(LayerSceneElement(identifier, this, std::move(value))));
            _elementMap.emplace(identifier, &_sceneElements->back());
            return _sceneElements->back();
        }

        LayerSceneElement& findChild(const std::string& identifier) {
            if (identifier.empty()) {
                glog.log<DefaultLevel::Warn>("标识符长度为零");
                return *this;
            }
            auto it = _elementMap.find(identifier);
            if (it == _elementMap.end()) {
                glog.log<DefaultLevel::Warn>("未找到[" + identifier + "]标识符元素");
                return *this;
            }
            return *it->second;
        }

        std::list<LayerSceneElement>& list() {
            return *_sceneElements;
        }

        const std::list<LayerSceneElement>& list() const {
            return *_sceneElements;
        }

        Composite& get() {
            return *_value;
        }

        Drawable& getCarried() {
            return *_value->carried;
        }

        Transform& getCommon() {
            return _value->common;
        }

        operator Composite& () {
            return *_value;
        }

        operator Drawable& () {
            return *_value->carried;
        }

        operator Transform& () {
            return _value->common;
        }

        LayerSceneElement& operator [] (const std::string& identifier) {
            return findChild(identifier);
        }

    private:
        size_t _depth{0};
        bool _isRoot{false};
        std::string _identifier;
        LayerSceneElement* _parent{};
        std::unique_ptr<Composite> _value{nullptr};
        std::map<std::string, LayerSceneElement*> _elementMap;
        std::shared_ptr<std::list<LayerSceneElement>> _sceneElements{nullptr};

        LayerSceneElement(const std::string& identifier, LayerSceneElement* parent, Composite value = {Transform{}, nullptr}):
            _depth(parent->_depth + 1),
            _isRoot(false),
            _identifier(identifier),
            _parent(parent),
            _value(std::make_unique<Composite>(std::move(value))),
            _elementMap(),
            _sceneElements(parent->_sceneElements) {
        };


};
