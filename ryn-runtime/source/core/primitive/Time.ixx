module;
#include "Prelude.h"
#include <chrono>
#include <cstdint>

export module Ryntime.Core.Primitive:Time;

export namespace ryn {
    class Timer {
    public:
        inline Timer() noexcept : m_Start(now()) {}

        [[nodiscard]] static inline double now() noexcept {
            auto now = std::chrono::steady_clock::now().time_since_epoch();
            return std::chrono::duration<double>(now).count();
        }

        [[nodiscard]] inline double elapsed() const noexcept { return now() - m_Start; }

        inline void reset() noexcept { m_Start = now(); }

    private:
        double m_Start;
    };

    struct Time {
        double   Total      = 0.0;
        double   Delta      = 0.0;
        uint64_t FrameCount = 0;

        [[nodiscard]] inline float dt() const noexcept { return static_cast<float>(Delta); }
    };
}
