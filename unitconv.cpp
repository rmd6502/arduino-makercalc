#include "unitconv.h"
#include <map>
#include <string>
#include <stdint.h>

Unit::ConversionMapType Unit::conversionMap = {};
static SimpleUnit *inch = new SimpleUnit("inch", {"in", "inches"}, 0.0254, UnitConv::LENGTH_UNIT + UnitConv::BIAS);
static SimpleUnit *foot = new SimpleUnit("foot", {"ft", "feet"}, 0.0254 * 12, UnitConv::LENGTH_UNIT + UnitConv::BIAS);
static SimpleUnit *yard = new SimpleUnit("yard", {"yd", "yards"}, 0.0254 * 36, UnitConv::LENGTH_UNIT + UnitConv::BIAS);
static SimpleUnit *mile = new SimpleUnit("mile", {"mi", "miles"}, 0.0254 * 5280, UnitConv::LENGTH_UNIT + UnitConv::BIAS);
static CompoundUnit *sqin = new CompoundUnit("sqin",std::set<std::string>{"in2","in²","square inches", "square inch"}, std::map<std::string, double>{{"inch", 2}});

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
  printf("qty %lf tounit factor %lf\n", quantity.second, unit->factor());
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
  return 0;
}
double CompoundUnit::factor() {
  return 0;
}