#ifndef OPTIONS_HPP
#define OPTIONS_HPP

#include "logger/logger.hpp"

#include <array>
#include <cstdint>
#include <string>
#include <vector>

class Options {
public:
    Options(Logger& log, const std::string& filename);

    using FloatType = double;
    using IntType = std::int64_t;
    using UIntType = std::uint64_t;
    using ElemIndexType = std::array<UIntType, 3>;
    using LoadType = std::tuple<ElemIndexType, FloatType>;

    // Any option here without a default value (that's not 0)
    // must be specified in the config file.

    FloatType volumeFraction {0};
    FloatType filterRadius {0};

    FloatType penaltyFactorStart {3};
    FloatType penaltyFactorEnd {4};
    FloatType penaltyFactorStep;

    std::array<UIntType, 3> numElements {0, 0, 0};

    // Only one of these stop conditions needs to be specified.
    UIntType maxIterations {0};
    FloatType objectiveConvergence {0};  // Once the objective function changes
                                         // by less than this, return.

    // At least one of each of these is necessary.
    std::vector<ElemIndexType> fixpoints {};
    std::vector<LoadType> loads {};
};

#endif
