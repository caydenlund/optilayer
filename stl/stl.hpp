#ifndef STL_HPP
#define STL_HPP

#include <array>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>

class Stl {
public:
    struct Vertex {
        std::array<float, 3> coordinates;
    };

    struct Triangle {
        std::array<std::size_t, 3> vertices;
    };

    explicit Stl(const std::string& filename);

    void saveFile(const std::string& filename) const;

    std::vector<Vertex> vertices;

    std::vector<Triangle> triangles;

private:
    void _loadAsciiFile(const std::string& filename);

    void _loadBinaryFile(const std::string& filename);

    static constexpr std::size_t asciiHeaderSize = 6;

    static constexpr std::size_t binaryHeaderSize = 80;
};

struct StlError : std::runtime_error {
    enum ErrorType : std::uint8_t { READ, WRITE, FORMAT };

    StlError(ErrorType errorType, const std::string& filename);
};

#endif
