#include "options.hpp"

#include <fstream>
#include <functional>

Options::Options(Logger& log, const std::string& filename) {
    // Helper functions
    const auto isWhitespace = [](const char cha) -> bool { return cha == ' ' || cha == '\t' || cha == '\n'; };
    const auto trim = [&](const std::string& string) -> std::string {
        //
        std::size_t begin = 0;
        std::size_t end = string.size();
        while (begin < end && isWhitespace(string[begin])) ++begin;
        while (end > begin && isWhitespace(string[end - 1])) --end;
        if (begin == end) return {};
        return string.substr(begin, end - begin);
    };
    const auto beginsWith = [](const std::string& string, const std::string& pat) -> bool {
        //
        if (string.size() < pat.size()) return false;
        for (std::size_t ind = 0; ind < pat.size(); ++ind) {
            if (string[ind] != pat[ind]) return false;
        }
        return true;
    };
    const auto toLower = [](const std::string& string) -> std::string {
        std::string result {string};
        for (char& cha : result) cha = (char)std::tolower(cha);
        return result;
    };
    // TODO: Doesn't account for end-of-line comments...
    const auto isComment = [&](const std::string& string) {
        return beginsWith(string, "//") || beginsWith(string, "#");
    };
    const auto syntaxError = [&](const std::size_t lineNum) {
        throw std::runtime_error("Syntax error in `" + filename + "` on line " + std::to_string(lineNum));
    };
    const auto getInt = [&](const std::string& value, const std::size_t lineNum) -> IntType {
        try {
            return std::stoll(value);
        } catch (...) { syntaxError(lineNum); }
        return 0;
    };
    const auto getUInt = [&](const std::string& value, const std::size_t lineNum) -> UIntType {
        try {
            return std::stoull(value);
        } catch (...) { syntaxError(lineNum); }
        return 0;
    };
    const auto getFloat = [&](const std::string& value, const std::size_t lineNum) -> FloatType {
        try {
            return std::stod(value);
        } catch (...) { syntaxError(lineNum); }
        return 0;
    };
    const auto getElementIndex = [&](const std::string& value, const std::size_t lineNum) -> ElementIndexType {
        if (value.size() < 8) syntaxError(lineNum);
        if (value[0] != '(') syntaxError(lineNum);
        if (value[value.size() - 1] != ')') syntaxError(lineNum);
        // TODO: ...
        return {};
    };
    const auto getLoad = [&](const std::string& value, const std::size_t lineNum) -> ElementIndexType {
        // TODO: ...
        return {};
    };


    // Begin
    log.dbg << "Reading options file `filename`\n";

    std::ifstream file(filename);
    if (!file.is_open()) throw std::runtime_error("Unable to open file `" + filename + "` for reading");

    std::string line;
    std::size_t lineNum = 0;
    while (std::getline(file, line)) {
        ++lineNum;
        // Trim leading/trailing whitespace
        line = trim(line);
        if (line.empty()) continue;
        if (isComment(line)) continue;

        // Extract key & value
        std::size_t splitInd;
        for (splitInd = 0;; ++splitInd) {
            if (splitInd == line.size()) syntaxError(lineNum);
            if (line[splitInd] == ':' || line[splitInd] == '=') break;
        }

        const std::string key = toLower(trim(line.substr(0, splitInd)));
        const std::string value = (splitInd + 1 < line.size()) ? line.substr(splitInd + 1) : "";

        const std::function<void()> parseLine = [&]() {
            if (key == "volume_fraction") {
                this->volumeFraction = getFloat(value, lineNum);
            } else if (key == "filter_radius") {
                this->filterRadius = getFloat(value, lineNum);
            } else if (key == "penalty_factor_start") {
                this->penaltyFactorStart = getFloat(value, lineNum);
            } else if (key == "penalty_factor_end") {
                this->penaltyFactorEnd = getFloat(value, lineNum);
            } else if (key == "penalty_factor_step") {
                this->penaltyFactorStep = getFloat(value, lineNum);
            } else if (key == "num_elements_x") {
                this->numElements[0] = getUInt(value, lineNum);
            } else if (key == "num_elements_y") {
                this->numElements[1] = getUInt(value, lineNum);
            } else if (key == "num_elements_z") {
                this->numElements[2] = getUInt(value, lineNum);
            } else if (key == "max_iterations") {
                this->maxIterations = getUInt(value, lineNum);
            } else if (key == "objective_convergence") {
                this->objectiveConvergence = getFloat(value, lineNum);
            } else if (key == "fixpoints") {
                while (std::getline(file, line)) {
                    ++lineNum;
                    line = trim(line);
                    if (line.empty()) continue;
                    if (isComment(line)) continue;
                    if (beginsWith(line, "-")) {
                        this->fixpoints.push_back(getElementIndex(value, lineNum));
                    } else {
                        parseLine();
                    }
                }
            } else if (key == "loads") {
                while (std::getline(file, line)) {
                    ++lineNum;
                    line = trim(line);
                    if (line.empty()) continue;
                    if (isComment(line)) continue;
                    if (beginsWith(line, "-")) {
                        this->fixpoints.push_back(getElementIndex(value, lineNum));
                    } else {
                        parseLine();
                    }
                }
            }
        };
    }
}
