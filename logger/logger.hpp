#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <ostream>

struct Logger {
    class Streamer {
    public:
        Streamer() = default;
        Streamer(std::ostream& stream) : _stream(&stream) {}

        template<typename DataType>
        void operator<<(const DataType& data) {
            if (_stream) (*_stream) << data;
        }

    private:
        std::ostream* _stream {};
    };

    Logger(Streamer dbg, Streamer inf, Streamer err);

    std::ostream& dbg;
    std::ostream& inf;
    std::ostream& err;
};

#endif
