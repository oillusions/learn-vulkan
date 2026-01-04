#include "ModelParser.h"
#include <sstream>

#include <GlobalLogger.hpp>

using namespace std;

std::map<std::string, VertexLayout<float> > ModelParser::ObjModelLoader(const std::string &source) {
    if (source.empty()) {
        glog.log<DefaultLevel::Error>("错误: obj模型源为空");
        std::terminate();
    }
    return ObjModelLoader::parser(source);
}

std::map<std::string, VertexLayout<float> > ModelParser::ObjModelLoader::parser(const std::string &source) {
    map<string, VertexLayout<float>> models{};
    istringstream sourceIss(source);
    string line;
    VertexCounter v_size{0, 0}, t_size{0, 0}, n_size{0, 0};
    while (getline(sourceIss, line)) {
        if (line[0] == '#' || line.empty()) continue;
        if (line[0] == 'o') {
            objectProcess(line.substr(2), sourceIss, models, v_size, t_size, n_size);
        }
    }
    return models;
}

void ModelParser::ObjModelLoader::objectProcess(const string& name, istringstream &source, std::map<std::string, VertexLayout<float>>& models, VertexCounter& v, VertexCounter& t, VertexCounter& n) {
    auto builder = VertexLayout<float>::builder();
    vector<float> vertices{};
    vector<float> texCoord{};
    vector<float> normal{};
    vector<unsigned int> indices;

    VertexCounter local_v{0, v.count}, local_t{0, t.count}, local_n{0, n.count};

    string line;
    while (getline(source, line)) {
        if (line[0] == '#' || line.empty()) continue;
        if (line[0] == 'o') {
            v.start += local_v.count;
            v.count += local_v.count;
            t.start += local_t.count;
            t.count += local_t.count;
            n.start += local_n.count;
            n.count += local_n.count;

            objectProcess(line.substr(2), source, models, v, t, n);
            break;
        }
        lineProcess(line, vertices, texCoord, normal, indices, local_v, local_t, local_n);
    }
    if (!vertices.empty()) {
        vertices.shrink_to_fit();
        builder.appendElement("vertices", 3)
            .attachSource("vertices", std::move(vertices));
    }
    if (!texCoord.empty()) {
        texCoord.shrink_to_fit();
        builder.appendElement("texCoord", 2)
            .attachSource("texCoord", std::move(texCoord));
    }
    if (!normal.empty()) {
        normal.shrink_to_fit();
        builder.appendElement("normal", 3)
            .attachSource("normal", std::move(normal));
    }
    if (!indices.empty()) {
        builder.attachIndices(std::move(indices));
    }
    models.emplace(name, std::move(builder.build()));
}

void ModelParser::ObjModelLoader::lineProcess(const std::string &line, vector<float>& vertices, vector<float>& texCoord, vector<float>& normal, vector<unsigned int> &indices, VertexCounter& v, VertexCounter& t, VertexCounter& n) {
    istringstream lineIss(line);
    string token;
    while (getline(lineIss, token, ' ')) {
        if (token == "v") {  // 顶点坐标
            float x, y, z;
            lineIss >> x >> y >> z;
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
            v.count++;
        }
        else if (token == "vt") {  // 纹理坐标
            float vertex_u, vertex_v;
            lineIss >> vertex_u >> vertex_v;
            texCoord.push_back(vertex_u);
            texCoord.push_back(vertex_v);
            t.count++;
        }
        else if (token == "vn") {  // 法线
            float x, y, z;
            lineIss >> x >> y >> z;
            normal.push_back(x);
            normal.push_back(y);
            normal.push_back(z);
            n.count++;
        }
        else if (token == "f") {  // 面
            string fLine;
            while (getline(lineIss, token, ' ')) {
                string index;
                istringstream tokenIss(token);
                size_t counterIndex{0};
                while (getline(tokenIss, index, '/')) {
                    switch (counterIndex) {
                        case 0: {
                            indices.push_back(stoull(index) - 1 - v.start);
                            break;
                        }
                        case 1: {
                            indices.push_back(stoull(index) - 1 - t.start);
                            break;
                        }
                        case 2: {
                            indices.push_back(stoull(index) - 1 - n.start);
                            break;
                        }
                        default: break;
                    }
                    counterIndex++;
                }
            }
        }
    }
}



