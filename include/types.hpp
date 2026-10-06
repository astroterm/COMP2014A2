#pragma once

enum class CityId {
    Sydney, Campbelltown, Mittagong,
    Goulburn, Yass, Gundagai,
    Holbrook, Albury, Wangaratta,
    Euroa, Wallan, Melbourne
};

struct City {
    CityId city;
    int chargers;
    
};


