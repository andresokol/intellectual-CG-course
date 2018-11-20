#include "geo_loaders.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iterator>
#include <stdlib.h>
#include "vec3.hpp"
#include "../primitives/Triangle.hpp"


namespace geo_loaders {
    namespace {
        std::vector<std::string> splitString(const std::string &s, char delim = ' ') {
            std::stringstream ss(s);
            std::string item;
            std::vector<std::string> elems;
            while (std::getline(ss, item, delim)) {
                if (item.length() > 0) {
                    elems.push_back(item);
                }
            }

            return elems;
        }

        int getFaceIndexes(const std::string &s, bool normal = false) {
            const auto &splitted = splitString(s, '/');
            const auto &index_s = splitted.at(normal ? splitted.size() - 1 : 0);
            return std::stoi(index_s) - 1;
        }
    }

    std::vector<Triangle> LoadObj(const std::string &filename, const Vec3 &pos, const Material &mat) {
        std::ifstream file(filename);
        std::string line;

        std::vector<Vec3> vertices;
        std::vector<Vec3> normals;
        std::vector<Triangle> tris;

        std::cout << "### Reading: " << filename << std::endl;

        while (getline(file, line)) {
            if (line[0] == '#') continue;

            const auto &data = splitString(line);

            if (data[0] == "v") {
                vertices.emplace_back(
                        std::stod(data[1]),
                        std::stod(data[2]),
                        std::stod(data[3])
                );

//                std::cout << "v: vertices read " << vertices.size() << std::endl;
            } else if (data[0] == "vn") {
                normals.emplace_back(
                        std::stod(data[1]),
                        std::stod(data[2]),
                        std::stod(data[3])
                );

//                std::cout << "vn: normals read " << normals.size() << std::endl;
            } else if (data[0] == "f") {
                int a = getFaceIndexes(data[1]);
                int b = getFaceIndexes(data[2]);
                int c = getFaceIndexes(data[3]);

                int n = getFaceIndexes(data[1], true);

                tris.emplace_back(
                        vertices[a] + pos,
                        vertices[b] + pos,
                        vertices[c] + pos,
                        normals[n],
                        mat
                );
//                std::cout << "f: tris read " << tris.size() << std::endl;
            } else {
//                std::cout << "[skip]: " << line << std::endl;
            }
        }

//        std::cout << "######" << std::endl;

        return tris;
    }
}