#include "unitconv.h"

#include <iostream>

int main(void) {
    auto unitIt = Unit::conversionMap.find("in");
    if (unitIt != Unit::conversionMap.end()) {
        auto unit = unitIt->second;
        std::cout << "inches " << unit->dimension() << ", " << unit->factor() << std::endl;
    }
    DimensionedQuantity d = Unit::createDimensioned(5.0, "feet");
    std::cout << "5 ft " << d.first << ", " << d.second << std::endl;
    DimensionedQuantity d2 = UnitConv::convertTo(d, "in");
    std::cout << "5 ft->in " << d2.first << ", " << d2.second << std::endl;

    d = Unit::createDimensioned(10, "sqyd");
    std::cout << "10 sqyd " << d.first << ", " << d.second << std::endl;
    d2 = UnitConv::convertTo(d, "acre");
    std::cout << "10 sqyd->acre " << d2.first << ", " << d2.second << std::endl;
}
