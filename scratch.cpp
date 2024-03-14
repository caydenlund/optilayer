#include "stl/stl.hpp"

#include <iostream>

int main(const int argc, const char** argv) {
    for (int ind = 1; ind < argc; ++ind) {
        std::cout << "Processing file `" << argv[ind] << ":`\n";

        const Stl stl(std::string {argv[ind]});

        for (const auto& [normals, vertices] : stl.facets) {
            std::cout << "    Facet:\n"
                      << "        - Normals:  " << normals[0] << "  " << normals[1] << "  " << normals[2] << "\n"
                      << "        - Vertices:\n"
                      << "            - " << vertices[0].coordinates[0] << "  " << vertices[0].coordinates[1] << "  "
                      << vertices[0].coordinates[2] << "\n"
                      << "            - " << vertices[1].coordinates[0] << "  " << vertices[1].coordinates[1] << "  "
                      << vertices[1].coordinates[2] << "\n"
                      << "            - " << vertices[2].coordinates[0] << "  " << vertices[2].coordinates[1] << "  "
                      << vertices[2].coordinates[2] << "\n";
        }
    }
}
