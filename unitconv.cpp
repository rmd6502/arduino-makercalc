#include "unitconv.h"
#include "units.h"

#include <map>
#include <math.h>
#include <string>
#include <stdint.h>

Unit::ConversionMapType Unit::conversionMap = {};

std::shared_ptr<Unit> Unit::findUnit(std::string unitName) {
  auto unitIt = Unit::conversionMap.find(unitName);
  if (unitIt == Unit::conversionMap.end()) {
    return nullptr;
  }
  return unitIt->second;
}

DimensionedQuantity Unit::createDimensioned(double value, std::string fromUnit) {
  auto unit = findUnit(fromUnit);
  return unit->createDimensioned(value);
}

DimensionedQuantity Unit::createDimensioned(double value) {
  return std::make_pair(dimension(), value * factor());
}

DimensionedQuantity UnitConv::convertTo(DimensionedQuantity quantity, std::string toUnit) {
  auto unit = Unit::findUnit(toUnit);
  if (!unit) {
    return std::make_pair(0,0);
  }
  printf("qty %.10lf tounit factor %.10lf\n", quantity.second, unit->factor());
  return std::make_pair(unit->dimension(), quantity.second / unit->factor());
}

CompoundUnit::CompoundUnit(std::string unitName, std::set<std::string> unitAliases, std::map<std::string, double> unitComponents) : Unit(unitName, unitAliases) {
  for (auto comp : unitComponents) {
    auto unit = findUnit(comp.first);
    if (unit) {
      components[comp.first] = std::make_pair(unit, comp.second);
    }
  }
}

DimensionType CompoundUnit::dimension() {
  DimensionType ret = UnitConv::BIAS;
  for (auto comp : components) {
    auto unit = comp.second.first;
    auto power = comp.second.second;
    //printf("power %lf\n", power);
    //printf("unit %s dim %u\n", unit->name.c_str(), unit->dimension());
    ret += (unit->dimension() - UnitConv::BIAS) * power;
  }
  return ret;
}
double CompoundUnit::factor() {
  double ret = 1;
  for (auto comp : components) {
    auto unit = comp.second.first;
    auto power = comp.second.second;
    ret *= pow(unit->factor(), power);
  }
  return ret;
}