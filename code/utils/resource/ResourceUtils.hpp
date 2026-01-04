#pragma once
#include <fstream>
#include <filesystem>

#include <GlobalLogger.hpp>
#include <Resource.hpp>

namespace resource::utils {
    using namespace std;
    namespace fs = std::filesystem;

    /**
     * @brief 读取文件并转换为字符串
     * @details 他似乎不需要详细注释[划掉]
     * @param path 文件路径
     * @return 字符串
     */
    inline string readFileToStr(const fs::path& path) {
        if (!fs::exists(path)) {
            glog.log<DefaultLevel::Error>("文件不存在: " + path.string());
            return {};
        }

        ifstream file(path);
        if (!file.is_open()) {
            glog.log<DefaultLevel::Error>("文件无法打开: " + path.string());
            return {};
        }
        string out{istreambuf_iterator<char>(file), istreambuf_iterator<char>()};
        file.close();
        return out;
    }
}

/**
 * @brief 全局通用资源管理器
 * @details 他似乎不需要详细注释[划掉]
 */
inline AnyResourceManager arm{};