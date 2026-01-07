#pragma once

#include <filesystem>

#include <GlobalLogger.hpp>
#include <RAIIWrapper.hpp>
#include <Resource.hpp>
#include <ResourceUtils.hpp>

class ShaderResource: public RAIIWrapper<std::vector<char>>, IResource {
    public:
        ShaderResource(const std::filesystem::path& path) {
            if (!std::filesystem::exists(path)) {
                glog.log<DefaultLevel::Warn>("ShaderResource 加载失败: 文件不存在[" + path.string() + "]");
                return;
            }
            std::ifstream file(path, std::ios::binary | std::ios::ate);
            if (!file.is_open()) {
                glog.log<DefaultLevel::Warn>("ShaderResource 加载失败: 文件打开失败[" + path.string() + "]");
                return;
            }
            const size_t fileSize = file.tellg();
            if (fileSize == 0) {
                glog.log<DefaultLevel::Warn>("ShaderResource 加载失败: 文件为空[" + path.string() + "]");
            }
            _value.resize(fileSize);
            file.seekg(0);
            file.read(_value.data(), fileSize);
            file.close();

            isLoad = true;
        }

        ~ShaderResource() override {
            glog.log<DefaultLevel::Debug>("ShaderResource 已析构");
        }


    bool isComplete() const {
            return isLoad;
        }
    private:
        bool isLoad{false};
};
