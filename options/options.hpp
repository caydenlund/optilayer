#ifndef OPTIONS_HPP
#define OPTIONS_HPP

#include <array>
#include <cstdint>

class Options {
public:
    using FloatType = double;
    using IntType = std::int64_t;
    using UIntType = std::uint64_t;

    FloatType volumeFraction;
    FloatType filterRadius;
    FloatType penalization;

    std::array<UIntType, 3> numElements;

    UIntType numIterations;

private:
};

#endif
