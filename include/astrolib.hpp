#pragma once

#include <random>
#include <ranges>
#include <sstream>

namespace astro {
    class Random {
    public:
        Random() : rd(), gen(rd()) {}
        Random(int seed) : gen(seed) {}

        int range(int start, int end) {
            std::uniform_int_distribution dist(start, end - 1);
            return dist(gen);
        }

    private:
        std::random_device rd;
        std::mt19937 gen;
    };
    namespace ranges {
        template<typename T>
        concept Summable = requires(T a, T b) {
            { a + b } -> std::convertible_to<T>;
        };

        struct fold_until_fn {
            template<std::ranges::input_range In, Summable T, typename F, typename C>
            requires std::invocable<F, T, std::ranges::range_reference_t<In>>
            && std::predicate<C, T>
            constexpr auto operator()(In&& range, T acc, F f, C condition) const {
                for (auto it = range.begin(); it != range.end(); ++it) {
                    auto&& x = *it;
                    auto temp = std::invoke(f, acc, x);
                    if (std::invoke(condition, temp)) {
                        return acc;
                    }
                    acc = std::move(temp);
                }
                return acc;
            }
        };

        struct fold_until_index_fn {
            template<std::ranges::input_range In, Summable T, typename F, typename C>
            requires std::invocable<F, T, std::ranges::range_reference_t<In>>
            && std::predicate<C, T>
            constexpr std::optional<std::size_t> operator()(In&& range, T acc, F f, C condition) const {
                int i = 0;
                for (auto it = range.begin(); it != range.end(); ++it, ++i) {
                    auto&& x = *it;
                    acc = std::invoke(f, std::move(acc), x);
                    if (std::invoke(condition, acc)) {
                        if (i == 0) return std::nullopt;

                        return i - 1;
                    }
                    
                }
                if (i == 0) return std::nullopt;
                return i - 1;
            }
        };

        inline constexpr fold_until_fn fold_until{};
        inline constexpr fold_until_index_fn fold_until_index{};

    }


    namespace ansi {
        constexpr std::string_view reset     = "\033[0m";
        constexpr std::string_view bold      = "\033[1m";
        constexpr std::string_view underline = "\033[4m";

        constexpr std::string_view black   = "\033[30m";
        constexpr std::string_view red     = "\033[31m";
        constexpr std::string_view green   = "\033[32m";
        constexpr std::string_view yellow  = "\033[33m";
        constexpr std::string_view blue    = "\033[34m";
        constexpr std::string_view magenta = "\033[35m";
        constexpr std::string_view cyan    = "\033[36m";
        constexpr std::string_view white   = "\033[37m";
    }
    class pad {
    public:
        pad(std::string_view strv, std::size_t n) : strv(strv), n(n) {}
        std::string padding(std::string_view other) const {
            if (other.size() >= n) return std::string { other };
            std::size_t remaining = n - other.size();
            std::size_t left = remaining / 2;
            std::size_t right = remaining - left;
            std::ostringstream os;
            for (std::size_t i = 0; i < left; i++) {
                os << strv;
            }
            os << other;
            for (std::size_t i = 0; i < right; i++) {
                os << strv;
            }
            return os.str();
        }
        std::string_view strv;
        std::size_t n;
    };

    inline std::string operator|(std::string str, const pad& p) {
        return p.padding(str);
    }

    class colorise {
    public:
        colorise(std::string_view color) : color(color) {}
        std::string_view color;
    };

    inline std::string operator|(std::string str, const colorise& c) {
        return std::string {c.color}
            +  std::string {str}
            +  std::string {astro::ansi::reset};
    }
}