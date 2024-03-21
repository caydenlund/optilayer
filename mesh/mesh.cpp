#include "mesh.hpp"

#include <algorithm>
#include <unordered_set>

Mesh::Mesh(const std::vector<Face>& faces) {
    struct HashFloat {
        std::size_t operator()(const float value) const noexcept {
            constexpr float precision = 0.00001;

            return std::hash<float> {}(static_cast<float>(static_cast<long long>(value / precision)) * precision);
        }
    };

    struct HashVertex {
        std::size_t operator()(Vertex vertex) const noexcept {
            std::ranges::sort(vertex);
            constexpr unsigned int lshift0 = 3;
            constexpr unsigned int lshift1 = 5;
            constexpr unsigned int lshift2 = 7;
            constexpr std::hash<float> hash;

            return hash(vertex[0]) << lshift0 ^ hash(vertex[1]) << lshift1 ^ hash(vertex[2]) << lshift2;
        }
    };

    const std::vector vertices {[&] {
        std::unordered_set<Vertex, HashVertex> vertexSet;

        for (const auto& face : faces) {
            for (const auto& vertex : face) vertexSet.insert(vertex);
        }

        return std::vector<Vertex> {vertexSet.begin(), vertexSet.end()};
    }()};
}
