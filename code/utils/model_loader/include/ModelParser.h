#pragma once
#include <VertexLayout.hpp>

class ModelParser {
    public:
        ~ModelParser() = default;
        static std::map<std::string, VertexLayout<float>> ObjModelLoader(const std::string& source);
    private:
        struct VertexCounter {
            size_t count;
            size_t start;
        };
        class ObjModelLoader {
            public:
                ~ObjModelLoader() = default;
                static std::map<std::string, VertexLayout<float>> parser(const std::string& source);
                static void objectProcess(const std::string& name, std::istringstream &source, std::map<std::string, VertexLayout<float>>& models, VertexCounter& v, VertexCounter& t, VertexCounter& n);
                static void lineProcess(const std::string &line, std::vector<float>& vertices, std::vector<float>& texCoord, std::vector<float>& normal, std::vector<unsigned int> &indices, VertexCounter& v, VertexCounter& t, VertexCounter& n);
        };
};