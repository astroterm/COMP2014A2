#include "constants.hpp"
#include "demands.hpp"
#include "astrolib.hpp"
#include <string>

Demand::Demand(int vid, int did, int cr, int rr) : vehicle_id_(vid), destination_id_(did), capacity_range_(cr), remaining_range_(rr) {}

std::ofstream& operator<<(std::ofstream& ofs, const Demand& demand) {
    ofs
        << '[' << demand.vehicle_id() 
        << ',' << static_cast<int>(demand.destination_id())
        << ',' << demand.capacity_range()
        << ',' << demand.remaining_range()
        << ']' << '\n';
    
    return ofs;
}

void DemandGenerator::write(std::string f) {
    astro::Random random;
    std::ofstream file { f };
    int demandnum = random.range(MIN_DEMANDS, MAX_DEMANDS + 1);
    for (int i = 0; i < demandnum; i++) {
        int capacity = random.range(MIN_CAPACITY, MAX_CAPACITY + 1);
        file << Demand {
            100 + i,
            random.range(1, NUM_CITIES),
            capacity,
            random.range(MIN_REMAIN_RANGE, capacity + 1)
        };
    }
}
