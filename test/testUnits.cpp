#include "unitconv.h"
#include <mcheck.h>

#include <iostream>

int main(void) {
    mtrace();
    
    auto unitIt = Unit::conversionMap.find("in");
    if (unitIt != Unit::conversionMap.end()) {
        auto unit = unitIt->second;
        std::cout << "inches " << unit->dimension() << ", " << unit->factor() << std::endl;
    }
    UnitQuantity d = Unit::createDimensioned(5.0, "feet");
    std::cout << "5 ft " << d.dimension << ", " << d.value << std::endl;
    UnitQuantity d2 = UnitConv::convertTo(&d, "in");
    std::cout << "5 ft->in " << d2.dimension << ", " << d2.value << d2.unit->name << std::endl;

    d = Unit::createDimensioned(10, "sqyd");
    std::cout << "10 sqyd " << d.dimension << ", " << d.value << std::endl;
    d2 = UnitConv::convertTo(&d, "acre");
    std::cout << "10 sqyd->acre " << d2.dimension << ", " << d2.value << d2.unit->name << std::endl;

    d = Unit::createDimensioned(10, "liter");
    std::cout << "10 l " << d.dimension << ", " << d.value << std::endl;
    d2 = Unit::createDimensioned(2, "inch");
    std::cout << "2 in " << d2.dimension << ", " << d2.value << std::endl;
    UnitQuantity d3 = d / d2;
    std::cout << "10l / 2 in " << d3.dimension << ", " << d3.value << d3.unit->name << std::endl;

    d = Unit::createDimensioned(10, "gal");
    d3 = d / d2;
    std::cout << "10g / 2 in " << d3.dimension << ", " << d3.value << std::endl;
    d3 = UnitConv::convertTo(&d3, "sqin");
    std::cout << "10g / 2 in => sqin" << d3.dimension << ", " << d3.value << d3.unit->name << std::endl;

    d2 = Unit::createDimensioned(5, "l");
    d3 = d + d2;
    std::cout << "10g + 5l" << d3.dimension << ", " << d3.value << d3.unit->name << std::endl;
}
