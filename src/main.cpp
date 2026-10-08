#include "allocator.hpp"
#include "demands.hpp"
#include <iostream>
#include <string>


int main() {
    DemandGenerator dg;
    dg.write("data/outfile.txt");

    BasicAllocation ba("data/outfile.txt");
    ba.allocate();
    
    AuraAllocation aa("data/outfile.txt");
    aa.allocate();
}