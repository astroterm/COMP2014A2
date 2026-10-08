#pragma once

#include "city.hpp"
#include "constants.hpp"
#include "vehicle.hpp"

#include <algorithm>
#include <fstream>
#include <vector>
#include <ostream>
#include <iostream>

class ChargingAllocation {
public:
    ChargingAllocation(std::string filename) : stations(make_stations()) {
        std::ifstream file { filename };
        std::string line;
        while (std::getline(file, line)) {
            vehicles.emplace_back(line);
        }

    };

    static std::array<ChargingStation, NUM_CITIES> make_stations() {
        return std::array {
            ChargingStation{0},
            ChargingStation{1},
            ChargingStation{2},
            ChargingStation{3},
            ChargingStation{4},
            ChargingStation{5},
            ChargingStation{6},
            ChargingStation{7},
            ChargingStation{8},
            ChargingStation{9},
            ChargingStation{10},
            ChargingStation{11}
        };
    }

    double calculate() const {
        return std::ranges::fold_left(
            stations | std::views::transform([](const ChargingStation& s) {
                return s.calculate();
            }), 0.0, std::plus{}
        ) / std::ranges::fold_left(
            stations | std::views::transform([](const ChargingStation& s) {
                return s.queue_length();
            }), 0, std::plus{}
        );
    }

    virtual void allocate() = 0;

    virtual ~ChargingAllocation() = default;

protected:
    std::vector<Vehicle> vehicles;
    std::array<ChargingStation, NUM_CITIES> stations;
    friend std::ostream& operator<<(std::ostream& os, const ChargingAllocation& ca);
};

inline std::ostream& operator<<(std::ostream& os, const ChargingAllocation& ca) {
    os
        << "+" << ("-" | astro::pad("-", 5)) << "+" // +---+
        << ("-" | astro::pad("-", 15)) << "+" // +----------------+
        << ("-" | astro::pad("-", 16)) << "+" // +----------------+
        << ("-" | astro::pad("-", 17)) << "+" // +-----------------+
        << ("-" | astro::pad("-", 15)) << "+"
        << '\n'
        
        << "|" << ( "#"               | astro::pad(" ",  5) | astro::colorise(astro::ansi::green) | astro::colorise(astro::ansi::bold) )
        << "|" << ( "Destination"     | astro::pad(" ", 15) | astro::colorise(astro::ansi::green) | astro::colorise(astro::ansi::bold) ) 
        << "|" << ( "Capacity Range"  | astro::pad(" ", 16) | astro::colorise(astro::ansi::green) | astro::colorise(astro::ansi::bold) )
        << "|" << ( "Remaining Range" | astro::pad(" ", 17) | astro::colorise(astro::ansi::green) | astro::colorise(astro::ansi::bold) )
        << "|" << ( "Avg Wait Time"   | astro::pad(" ", 15) | astro::colorise(astro::ansi::green) | astro::colorise(astro::ansi::bold) )

        << "|\n";
    os
        << "+" << ("-" | astro::pad("-", 5)) << "+" // +---+
        << ("-" | astro::pad("-", 15)) << "+" // +----------------+
        << ("-" | astro::pad("-", 16)) << "+" // +----------------+
        << ("-" | astro::pad("-", 17)) << "+" // +-----------------+
        << ("-" | astro::pad("-", 15)) << "+"
        << '\n';
    for (const Vehicle& v : ca.vehicles) {
        os << v << '\n';
    }
    os
        << "+" << ("-" | astro::pad("-", 5)) << "+" // +---+
        << ("-" | astro::pad("-", 15)) << "+" // +----------------+
        << ("-" | astro::pad("-", 16)) << "+" // +----------------+
        << ("-" | astro::pad("-", 17)) << "+" // +-----------------+
        << ("-" | astro::pad("-", 15)) << "+"
        << '\n';
    
    return os;
}

class BasicAllocation : public ChargingAllocation {
public:
    BasicAllocation(std::string filename) : ChargingAllocation(filename) {}

    void allocate() override {

        while (true) {
            for (Vehicle& v : vehicles) {
                stations[v.move(v.furthest())].charge_if(v, [](Vehicle& vc) {
                    return vc.destination_id() != vc.current_city_id();
                });
            }
            if (std::ranges::all_of(vehicles, [](const Vehicle& v) {
                return v.destination_id() == v.current_city_id();
            })) break;
        }

        std::cout << "BasicAllocation: " << calculate() << '\n';
    }
};

class AuraAllocation : public ChargingAllocation {
public:
    AuraAllocation(std::string filename) : ChargingAllocation(filename) {}

    void allocate() override {
        int preferred = vehicles.size() / NUM_CITIES;
        while (true) {
            for (Vehicle& v : vehicles | std::views::filter([](const Vehicle& vc) {
                return vc.furthest() == vc.destination_id();
            })) v.move(v.furthest());

            for (Vehicle& v : vehicles | std::views::filter([](const Vehicle& vc) {
                return vc.remaining_range() + vc.capacity_range()
                    < sydney_distance(vc.destination_id()) - sydney_distance(vc.current_city_id());
            })) {
                for (int i = v.furthest(); i > v.current_city_id(); i--) {
                    if (stations[i].queue_length() <= preferred) {
                        v.move(i);
                        stations[i].charge(v);
                        break;
                    }
                }
            }

            for (Vehicle& v : vehicles | std::views::filter([](Vehicle& vc){
                return  vc.furthest() < vc.destination_id()
                    &&  vc.remaining_range() + vc.capacity_range()
                        >= sydney_distance(vc.destination_id()) - sydney_distance(vc.current_city_id());
            })) {
                bool success = false;
                for (int i = v.closest(); i <= v.furthest(); i++) {
                    if (stations[i].queue_length() <= preferred) {
                        v.move(i);
                        stations[i].charge(v);
                        success = true;
                        break;
                    }
                }
                if (success) break;
                stations[v.move(v.closest())].charge(v);
            }

            if (std::ranges::all_of(vehicles, [](const Vehicle& vc) {
                return vc.destination_id() == vc.current_city_id();
            })) break;
        }

        std::cout << "AuraAllocation: " << calculate() << '\n';
    }
};