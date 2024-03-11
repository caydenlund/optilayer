#include "stl/stl.hpp"

#include <iostream>

int main(int argc, char** argv) {
    for (int ind = 1; ind < argc; ++ind) {
        std::cout << "Processing file `" << argv[ind] << ":`\n";

        Stl stl(std::string {argv[ind]});

        for (const auto& facet : stl.facets) {
            std::cout << "    Facet:\n"
                      << "        - Normals:  " << facet.normals[0] << "  " << facet.normals[1] << "  "
                      << facet.normals[2] << "\n"
                      << "        - Vertices:\n"
                      << "            - " << facet.vertices[0].coordinates[0] << "  "
                      << facet.vertices[0].coordinates[1] << "  " << facet.vertices[0].coordinates[2] << "\n"
                      << "            - " << facet.vertices[1].coordinates[0] << "  "
                      << facet.vertices[1].coordinates[1] << "  " << facet.vertices[1].coordinates[2] << "\n"
                      << "            - " << facet.vertices[2].coordinates[0] << "  "
                      << facet.vertices[2].coordinates[1] << "  " << facet.vertices[2].coordinates[2] << "\n";
        }
    }
}

