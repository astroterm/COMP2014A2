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
        d_last_city(distance_from_sydney()),
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

    void calculate() {
        
        std::ranges::for_each(vehicles, [
            avg_wait_time = 0.5 * vehicles.size() / chargers
        ](Vehicle& v) {
            v.wait(avg_wait_time);
        });
    }


private:
    int city;
    int chargers;
    int d_last_city;
    std::string name_;
    std::vector<std::reference_wrapper<Vehicle>> vehicles;
};
