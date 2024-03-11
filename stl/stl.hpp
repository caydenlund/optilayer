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

    struct Facet {
        std::array<float, 3> normals;
        std::array<Vertex, 3> vertices;
    };

    Stl(const std::string& filename);

    std::vector<Facet> facets;

private:
    void _loadAsciiFile(const std::string& filename);
    void _loadBinaryFile(const std::string& filename);

    static constexpr std::size_t asciiHeaderSize = 6;
    static constexpr std::size_t binaryHeaderSize = 80;
};

struct StlError : public std::runtime_error {
    enum ErrorType : std::uint8_t { READ, WRITE, FORMAT };

    StlError(ErrorType errorType, const std::string& filename);
};

