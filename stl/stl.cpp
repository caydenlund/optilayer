#include "stl.hpp"

#include <array>
#include <cstring>
#include <fstream>
#include <iostream>

Stl::Stl(const std::string& filename) {
    // Type definitions
    using ByteType = char;

    // Open file
    std::ifstream file(filename);
    if (!file) throw StlError(StlError::READ, filename);

    // Read the first 6 bytes.
    // If they aren't `input `, then this is a binary STL file.
    std::array<ByteType, asciiHeaderSize> asciiHeader {};
    file.read(asciiHeader.data(), asciiHeaderSize);
    if (!file) throw StlError(StlError::FORMAT, filename);

    const std::string asciiExpected = "solid ";
    for (std::size_t ind = 0; ind < asciiHeaderSize; ++ind) {
        if (asciiHeader.at(ind) != asciiExpected[ind]) {
            _loadBinaryFile(filename);
            return;
        }
    }

    // Otherwise, this is an ASCII STL file.
    _loadAsciiFile(filename);
}

void Stl::_loadAsciiFile(const std::string& filename) {
    // ASCII file format (whitespace is ignored):
    //
    //     solid [optional name] [optional metadata...]
    //
    //     [For each facet:]
    //         facet normal [num] [num] [num]
    //             outer loop
    //                 vertex [num] [num] [num]
    //                 vertex [num] [num] [num]
    //                 vertex [num] [num] [num]
    //             endloop
    //         endfacet
    //
    //     endsolid [optional name]
    //
    // A number is of the following format:
    //     [sign][mantissa]e[sign][exponent]

    enum StateType : std::uint8_t {
        BEGIN,
        FACET,
        FACET_NORMAL_0,
        FACET_NORMAL_1,
        FACET_NORMAL_2,
        FACET_NORMAL_3,
        OUTER,
        OUTER_LOOP,
        OUTER_LOOP_VERTEX_0_0,
        OUTER_LOOP_VERTEX_0_1,
        OUTER_LOOP_VERTEX_0_2,
        OUTER_LOOP_VERTEX_0_3,
        OUTER_LOOP_VERTEX_1_0,
        OUTER_LOOP_VERTEX_1_1,
        OUTER_LOOP_VERTEX_1_2,
        OUTER_LOOP_VERTEX_1_3,
        OUTER_LOOP_VERTEX_2_0,
        OUTER_LOOP_VERTEX_2_1,
        OUTER_LOOP_VERTEX_2_2,
        OUTER_LOOP_VERTEX_2_3,
        ENDLOOP
    };

    std::string word;

    std::ifstream file(filename);
    std::getline(file, word);

    Facet currentFacet {};

    StateType state = BEGIN;
    file >> word;
    while (true) {
        switch (state) {
            case BEGIN:
                if (word == "endsolid") return;

                if (word == "facet") {
                    state = FACET;
                } else {
                    throw StlError(StlError::FORMAT, filename);
                }
                break;
            case FACET:
                if (word == "normal") {
                    state = FACET_NORMAL_0;
                } else {
                    throw StlError(StlError::FORMAT, filename);
                }
                break;
            case FACET_NORMAL_0:
                try {
                    currentFacet.normals[0] = std::stof(word);
                    state = FACET_NORMAL_1;
                } catch (const std::invalid_argument&) { throw StlError(StlError::FORMAT, filename); }
                break;
            case FACET_NORMAL_1:
                try {
                    currentFacet.normals[1] = std::stof(word);
                    state = FACET_NORMAL_2;
                } catch (const std::invalid_argument&) { throw StlError(StlError::FORMAT, filename); }
                break;
            case FACET_NORMAL_2:
                try {
                    currentFacet.normals[2] = std::stof(word);
                    state = FACET_NORMAL_3;
                } catch (const std::invalid_argument&) { throw StlError(StlError::FORMAT, filename); }
                break;
            case FACET_NORMAL_3:
                if (word == "outer") {
                    state = OUTER;
                } else {
                    throw StlError(StlError::FORMAT, filename);
                }
                break;
            case OUTER:
                if (word == "loop") {
                    state = OUTER_LOOP;
                } else {
                    throw StlError(StlError::FORMAT, filename);
                }
                break;
            case OUTER_LOOP:
                if (word == "vertex") {
                    state = OUTER_LOOP_VERTEX_0_0;
                } else {
                    throw StlError(StlError::FORMAT, filename);
                }
                break;
            case OUTER_LOOP_VERTEX_0_0:
                try {
                    currentFacet.vertices[0].coordinates[0] = std::stof(word);
                    state = OUTER_LOOP_VERTEX_0_1;
                } catch (const std::invalid_argument&) { throw StlError(StlError::FORMAT, filename); }
                break;
            case OUTER_LOOP_VERTEX_0_1:
                try {
                    currentFacet.vertices[0].coordinates[1] = std::stof(word);
                    state = OUTER_LOOP_VERTEX_0_2;
                } catch (const std::invalid_argument&) { throw StlError(StlError::FORMAT, filename); }
                break;
            case OUTER_LOOP_VERTEX_0_2:
                try {
                    currentFacet.vertices[0].coordinates[2] = std::stof(word);
                    state = OUTER_LOOP_VERTEX_0_3;
                } catch (const std::invalid_argument&) { throw StlError(StlError::FORMAT, filename); }
                break;
            case OUTER_LOOP_VERTEX_0_3:
                if (word == "vertex") {
                    state = OUTER_LOOP_VERTEX_1_0;
                } else {
                    throw StlError(StlError::FORMAT, filename);
                }
                break;
            case OUTER_LOOP_VERTEX_1_0:
                try {
                    currentFacet.vertices[1].coordinates[0] = std::stof(word);
                    state = OUTER_LOOP_VERTEX_1_1;
                } catch (const std::invalid_argument&) { throw StlError(StlError::FORMAT, filename); }
                break;
            case OUTER_LOOP_VERTEX_1_1:
                try {
                    currentFacet.vertices[1].coordinates[1] = std::stof(word);
                    state = OUTER_LOOP_VERTEX_1_2;
                } catch (const std::invalid_argument&) { throw StlError(StlError::FORMAT, filename); }
                break;
            case OUTER_LOOP_VERTEX_1_2:
                try {
                    currentFacet.vertices[1].coordinates[2] = std::stof(word);
                    state = OUTER_LOOP_VERTEX_1_3;
                } catch (const std::invalid_argument&) { throw StlError(StlError::FORMAT, filename); }
                break;
            case OUTER_LOOP_VERTEX_1_3:
                if (word == "vertex") {
                    state = OUTER_LOOP_VERTEX_2_0;
                } else {
                    throw StlError(StlError::FORMAT, filename);
                }
                break;
            case OUTER_LOOP_VERTEX_2_0:
                try {
                    currentFacet.vertices[2].coordinates[0] = std::stof(word);
                    state = OUTER_LOOP_VERTEX_2_1;
                } catch (const std::invalid_argument&) { throw StlError(StlError::FORMAT, filename); }
                break;
            case OUTER_LOOP_VERTEX_2_1:
                try {
                    currentFacet.vertices[2].coordinates[1] = std::stof(word);
                    state = OUTER_LOOP_VERTEX_2_2;
                } catch (const std::invalid_argument&) { throw StlError(StlError::FORMAT, filename); }
                break;
            case OUTER_LOOP_VERTEX_2_2:
                try {
                    currentFacet.vertices[2].coordinates[2] = std::stof(word);
                    state = OUTER_LOOP_VERTEX_2_3;
                } catch (const std::invalid_argument&) { throw StlError(StlError::FORMAT, filename); }
                break;
            case OUTER_LOOP_VERTEX_2_3:
                if (word == "endloop") {
                    state = ENDLOOP;
                } else {
                    throw StlError(StlError::FORMAT, filename);
                }
                break;
            case ENDLOOP:
                if (word == "endfacet") {
                    state = BEGIN;
                    this->facets.push_back(currentFacet);
                    currentFacet = {};
                } else {
                    throw StlError(StlError::FORMAT, filename);
                }
                break;
        }
        file >> word;
    }
}

void Stl::_loadBinaryFile(const std::string& filename) {
    // Binary file format (whitespace added for clarity):
    //
    // [80:header]
    // [4:number of facets]
    //
    // [for each facet:]
    //     [4:norm1] [4:norm2] [4:norm3]
    //     [4:vert1_coord1] [4:vert1_coord2] [4:vert1_coord3]
    //     [4:vert2_coord1] [4:vert2_coord2] [4:vert2_coord3]
    //     [4:vert3_coord1] [4:vert3_coord2] [4:vert3_coord3]
    //     [2:0]
    using ByteType = char;
    constexpr std::size_t numberSize = 4;

    std::ifstream file(filename);
    file.seekg(binaryHeaderSize);

    std::int32_t numFacets;
    file.read(reinterpret_cast<ByteType*>(&numFacets), numberSize);

    if (numFacets < 0) throw StlError(StlError::FORMAT, filename);
    this->facets.resize(numFacets);

    for (std::uint32_t facetNum = 0; facetNum < numFacets; ++facetNum) {
        std::cout << "facet:\n";

        auto& [normals, vertices] = this->facets[facetNum];

        std::cout << "    normals:";
        for (std::uint8_t normalInd = 0; normalInd < 3; ++normalInd) {
            if (!file) throw StlError(StlError::FORMAT, filename);
            float normal;
            file.read(reinterpret_cast<ByteType*>(&normal), numberSize);
            normals.at(normalInd) = normal;
            std::cout << " " << normal;
        }

        std::cout << "\n"
                  << "    vertices:\n";
        for (std::uint8_t vertexInd = 0; vertexInd < 3; ++vertexInd) {
            std::cout << "        -";
            for (std::uint8_t coordinateInd = 0; coordinateInd < 3; ++coordinateInd) {
                if (!file) throw StlError(StlError::FORMAT, filename);
                float coordinate;
                file.read(reinterpret_cast<ByteType*>(&coordinate), numberSize);
                vertices.at(vertexInd).coordinates.at(coordinateInd) = coordinate;
                std::cout << " " << coordinate;
            }
            std::cout << "\n";
        }

        if (!file) throw StlError(StlError::FORMAT, filename);
        std::uint16_t numAttributeBytes;
        file.read(reinterpret_cast<ByteType*>(&numAttributeBytes), sizeof(numAttributeBytes));
    }

    // Check whether there's any leftover data at the end.
    file.seekg(0, std::ios::end);
    constexpr std::size_t numNumbersPerFacet = 12;
    if (file.tellg() != binaryHeaderSize + numberSize + numFacets * (numNumbersPerFacet * numberSize + 2)) {
        throw StlError(StlError::FORMAT, filename);
    }
}

StlError::StlError(const ErrorType errorType, const std::string& filename)
    : std::runtime_error([&]() -> std::string {
          switch (errorType) {
              case READ:
                  return "Failed to read from file `" + filename + "`";
              case WRITE:
                  return "Failed to write to file `" + filename + "`";
              case FORMAT:
                  return "Error reading file `" + filename + "`: not a valid STL";
              default:
                  return "";
          }
      }()) {}
