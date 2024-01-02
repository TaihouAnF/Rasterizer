#pragma once
#include <vector>
#include <fstream>
#include <strstream>
#include <iostream>
#include "Triangle.h"

/**
 * @brief A Mesh of multiple Triangles, use this to represent arbitrary type of objects.
 */
struct Mesh {
    std::vector<Triangle> tris;

    // We are only having one function here so I put implementation here in the header

    /**
     * @brief Load obj file using the string "filename".
     * @param filename The string representing the obj file
     * @return true if successfully loaded, otherwise false
    */
    bool LoadFromObjFile(std::string filename) {
        std::ifstream f(filename);

        // If the file cannot be opened
        if (!f.is_open()) {
            std::cerr << "The file cannot be opened.\n";
            return false;
        }

        // A cache for vertices
        std::vector<Vector3d> vertices;
        while (!f.eof()) {

            // String buffer
            char line[128];

            // Read the line
            f.getline(line, 128);

            // String stream buffer
            std::strstream s;

            // Input the line
            s << line;

            // Junk, as we don't need the first character to be stored
            char junk;

            // If currently the line represent a vertex
            if (line[0] == 'v') {
                Vector3d vec{};
                s >> junk >> vec.x >> vec.y >> vec.z;
                vertices.push_back(vec);
            }
            // If currently the line represent a triangle
            if (line[0] == 'f') {
                int f[3]{};
                s >> junk >> f[0] >> f[1] >> f[2];
                tris.push_back({ vertices[f[0] - 1], vertices[f[1] - 1], vertices[f[2] - 1] });
            }
        }

        return true;
    }
};