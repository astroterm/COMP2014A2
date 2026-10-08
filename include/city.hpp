#pragma once
#include "vehicle.hpp"
#include <algorithm>
#include <functional>
#include "constants.hpp"

class ChargingStation {
public:
    ChargingStation(int c) :
        city(c), 
        chargers(CHARGER_COUNTS[c]),
        d_last_city(distance_to_last_city()),
        name_(name())
    {}

    int distance_from_sydney() const {
        return sydney_distance(city);
    }

    std::string name() const { return CITY_NAMES[city]; }

    int distance_to_last_city() {
        if (city == 0) return 0;
        return distance_from_sydney() - sydney_distance(city - 1);
    }

    void charge(Vehicle& v) {
        vehicles.push_back(v);
        v.charge();
    };

    template<typename F>
    requires std::predicate<F, Vehicle&>
    void charge_if(Vehicle& v, F f) {
        if (std::invoke(f, v)) charge(v);
    }

    double calculate() const {
        double avg_wait_time = 0.5 * vehicles.size() / chargers;
        std::ranges::for_each(vehicles, [
            avg_wait_time = avg_wait_time
        ](Vehicle& v) {
            v.wait(avg_wait_time);
        });
        return avg_wait_time * vehicles.size();
    }

    int queue_length() const { return vehicles.size(); }

private:
    int city;
    int chargers;
    int d_last_city;
    std::string name_;
    std::vector<std::reference_wrapper<Vehicle>> vehicles;
    friend std::ostream& operator<<(std::ostream& os, ChargingStation& cs);
};

inline std::ostream& operator<<(std::ostream& os, ChargingStation& cs) {
    os
        << '|' << ( std::to_string(cs.city)             | astro::pad(" ",  5)
            |       astro::colorise(astro::ansi::green) | astro::colorise(astro::ansi::bold) )
        << '|' << ( cs.name_                            | astro::pad(" ", 15) )
        << '|' << ( std::to_string(cs.chargers)         | astro::pad(" ", 5) )
        << '|';

    return os;
}
