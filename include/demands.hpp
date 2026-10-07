#pragma once

#include <vector>
#include <fstream>

class Demand {
public:
    explicit Demand(int vid, int did, int cr, int rr);

    int vehicle_id() const { return vehicle_id_; }
    int destination_id() const { return destination_id_; }
    int capacity_range() const { return capacity_range_; }
    int remaining_range() const { return remaining_range_; }

private:
    int vehicle_id_;
    int destination_id_;
    int capacity_range_;
    int remaining_range_;
};

std::ostream& operator<<(std::ostream& os, const Demand& demand);
std::ofstream& operator<<(std::ofstream& ofs, const Demand& demand);

class DemandGenerator {
public:
    void write(std::string f);
    friend std::ostream& operator<<(std::ostream& os, const DemandGenerator demandgenerator);
private:
    std::vector<Demand> demands;
};