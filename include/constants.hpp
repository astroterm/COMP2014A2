#pragma once

#include <algorithm>
#include <array>
#include <string>
#include <ranges>

const int NUM_CITIES = 12;

const int MIN_DEMANDS = 150;
const int MAX_DEMANDS = 200;

const int MIN_CAPACITY = 350;
const int MAX_CAPACITY = 550;

const int MIN_REMAIN_RANGE = 300;

constexpr std::array<int, NUM_CITIES> DISTANCE_MAP = { 0, 57, 60, 83, 86, 99, 115, 62, 74, 87, 106, 62 };
constexpr std::array<int, NUM_CITIES> CHARGER_COUNTS = { 10, 4, 3, 4, 2, 3, 2, 4, 3, 3, 2, 8 };

constexpr std::array<std::string, NUM_CITIES> CITY_NAMES = {
    "Sydney", "Campbelltown", "Mittagong", "Goulburn",
    "Yass", "Gundagai", "Holbrook", "Albury",
    "Wangaratta", "Euroa", "Wallan", "Melbourne"
};


constexpr int sydney_distance(int city) {
    return std::ranges::fold_left(
        DISTANCE_MAP | std::views::take(city + 1),
        0, std::plus{}
    );
}