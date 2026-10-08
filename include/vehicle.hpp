#pragma once

#include "constants.hpp"
#include "astrolib.hpp"

#include <cstddef>
#include <string>
#include <sstream>

class Vehicle {
public:
    Vehicle (std::string line) : current_city_id_(0), avg_wait_time_(0) {
        std::istringstream stream{line};
        char delim;

        stream
            >> delim
            >> vehicle_id_
            >> delim
            >> destination_id_
            >> delim
            >> capacity_range_
            >> delim
            >> remaining_range_
            >> delim;
    }
    int vehicle_id()      const { return vehicle_id_; }
    int destination_id()  const { return destination_id_; }
    int capacity_range()  const { return capacity_range_; }
    int remaining_range() const { return remaining_range_; }
    int current_city_id() const { return current_city_id_; }

    double avg_wait_time() const { return avg_wait_time_; }

    int furthest() const {
        return astro::ranges::fold_until_index(
            DISTANCE_MAP
                | std::views::drop(current_city_id_ + 1)
                | std::views::take(destination_id_ - current_city_id_),
            0, std::plus{},
            [
                max = remaining_range_
            ](int in) { return in > max; }
        ).transform([cc = current_city_id_](const std::size_t i) {
            return i + cc + 1;
        }).value_or(current_city_id_);
    }

    int closest() const {
        return astro::ranges::fold_until_index(
            DISTANCE_MAP
                | std::views::drop(current_city_id_ + 1)
                | std::views::take(destination_id_ - current_city_id_)
                | std::views::reverse,
            0, std::plus{},
            [
                max = capacity_range_
            ](int in) { return in > max; }
        ).transform([d = destination_id_](const std::size_t i) {
            return d - i;
        }).value_or(current_city_id_);
    }

    int move(int i) {
        remaining_range_ -= sydney_distance(i) - sydney_distance(current_city_id_);
        current_city_id_ = i;
        return i;
    }

    void wait(double time) { avg_wait_time_ += time; }

    void charge() { remaining_range_ = capacity_range_; }

private:
    int vehicle_id_;
    int destination_id_;
    int capacity_range_;
    int remaining_range_;
    int current_city_id_;

    double avg_wait_time_;
};

inline std::ostream& operator<<(std::ostream& os, const Vehicle& v) {
    os
        << '|' << ( std::to_string(v.vehicle_id())      | astro::pad(" ",  5)
            |       astro::colorise(astro::ansi::green) | astro::colorise(astro::ansi::bold) )
        << '|' << ( CITY_NAMES[v.destination_id()]      | astro::pad(" ", 15) )
        << '|' << ( std::to_string(v.capacity_range())  | astro::pad(" ", 16) )
        << '|' << ( std::to_string(v.remaining_range()) | astro::pad(" ", 17) )
        << '|' << ( std::to_string(v.avg_wait_time())   | astro::pad(" ", 15) )
        << '|';
    
    return os;
}