#include "stl.hpp"

#include <array>
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
    enum StateType {
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
                } catch (const std::invalid_argument& arg) { throw StlError(StlError::FORMAT, filename); }
                break;
            case FACET_NORMAL_1:
                try {
                    currentFacet.normals[1] = std::stof(word);
                    state = FACET_NORMAL_2;
                } catch (const std::invalid_argument& arg) { throw StlError(StlError::FORMAT, filename); }
                break;
            case FACET_NORMAL_2:
                try {
                    currentFacet.normals[2] = std::stof(word);
                    state = FACET_NORMAL_3;
                } catch (const std::invalid_argument& arg) { throw StlError(StlError::FORMAT, filename); }
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
                } catch (const std::invalid_argument& arg) { throw StlError(StlError::FORMAT, filename); }
                break;
            case OUTER_LOOP_VERTEX_0_1:
                try {
                    currentFacet.vertices[0].coordinates[1] = std::stof(word);
                    state = OUTER_LOOP_VERTEX_0_2;
                } catch (const std::invalid_argument& arg) { throw StlError(StlError::FORMAT, filename); }
                break;
            case OUTER_LOOP_VERTEX_0_2:
                try {
                    currentFacet.vertices[0].coordinates[2] = std::stof(word);
                    state = OUTER_LOOP_VERTEX_0_3;
                } catch (const std::invalid_argument& arg) { throw StlError(StlError::FORMAT, filename); }
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
                } catch (const std::invalid_argument& arg) { throw StlError(StlError::FORMAT, filename); }
                break;
            case OUTER_LOOP_VERTEX_1_1:
                try {
                    currentFacet.vertices[1].coordinates[1] = std::stof(word);
                    state = OUTER_LOOP_VERTEX_1_2;
                } catch (const std::invalid_argument& arg) { throw StlError(StlError::FORMAT, filename); }
                break;
            case OUTER_LOOP_VERTEX_1_2:
                try {
                    currentFacet.vertices[1].coordinates[2] = std::stof(word);
                    state = OUTER_LOOP_VERTEX_1_3;
                } catch (const std::invalid_argument& arg) { throw StlError(StlError::FORMAT, filename); }
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
                } catch (const std::invalid_argument& arg) { throw StlError(StlError::FORMAT, filename); }
                break;
            case OUTER_LOOP_VERTEX_2_1:
                try {
                    currentFacet.vertices[2].coordinates[1] = std::stof(word);
                    state = OUTER_LOOP_VERTEX_2_2;
                } catch (const std::invalid_argument& arg) { throw StlError(StlError::FORMAT, filename); }
                break;
            case OUTER_LOOP_VERTEX_2_2:
                try {
                    currentFacet.vertices[2].coordinates[2] = std::stof(word);
                    state = OUTER_LOOP_VERTEX_2_3;
                } catch (const std::invalid_argument& arg) { throw StlError(StlError::FORMAT, filename); }
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
                    facets.push_back(currentFacet);
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
    //
    // file.seekg(binaryHeaderSize);
}

StlError::StlError(const StlError::ErrorType errorType, const std::string& filename)
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

