/*
I hold a copy of this assignment that I can produce if the original is lost or damaged.
I hereby certify that no part of this assignment has been copied from any other student’s work or
from any other source except where due acknowledgement is made in the assignment. No part
of this assignment has been written/produced for me by another person except where such
collaboration has been authorised by the subject lecturer concerned.
*/

/*
Compiler: Apple Clang with flags: "-std=c++26 -Wall -Wextra -pedantic"
Editor: Visual Studio Code
*/

/*
Instructions to compile and run:
    $ c++ -std=c++26 -Wall -Wextra -pedantic src/*.cpp -Iinclude -o "target/main"
    $ target/main

*/

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

    std::cout << aa;
}