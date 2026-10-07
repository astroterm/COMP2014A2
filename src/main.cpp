#include "allocator.hpp"
#include "demands.hpp"
#include <iostream>
#include <string>


int main() {
    DemandGenerator dg;
    dg.write("data/outfile.txt");
    BasicAllocation ca("data/outfile.txt");
    ca.allocate();
    
    std::cout << ca;

}