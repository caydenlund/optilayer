#ifndef MESH_HPP
#define MESH_HPP

#include <array>
#include <vector>

class Mesh {
public:
    using Vertex = std::array<float, 3>;
    using Face = std::array<Vertex, 3>;
    using Tetrahedron = std::array<Vertex, 4>;

    explicit Mesh(const std::vector<Face>& faces);
};

#endif
