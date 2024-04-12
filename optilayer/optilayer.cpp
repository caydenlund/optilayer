#include "logger/logger.hpp"
#include "options/options.hpp"

#include <iostream>
#include <string>

int main(int argc, char** argv) {
    bool debug;
    std::string optionFile;
    for (int argInd = 0; argInd < argc; ++argInd) {
        std::string arg = argv[argInd];
        if (arg == "-d" || arg == "--debug") {
            debug = true;
        } else {
            optionFile = arg;
        }
    }

    Logger log {
            debug ? std::cout : Logger::Streamer {},  // Debug output stream.
            std::cout,                                // Regular output stream.
            std::cerr                                 // Error output stream.
    };

    try {
        if (optionFile.empty()) throw std::runtime_error("No option file given");

        Options options(optionFile);
    } catch (std::runtime_error& err) { log.err << "Unrecoverable error: `" << err.what() << "`\n"; }
}
