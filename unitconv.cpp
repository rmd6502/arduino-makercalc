#include "unitconv.h"
Unit::ConversionMapType Unit::conversionMap = {};
#include "units.h"

#include <map>
#include <math.h>
#include <string>
#include <stdint.h>

std::shared_ptr<Unit> Unit::findUnit(std::string unitName)
{
  auto unitIt = Unit::conversionMap.find(unitName);
  if (unitIt == Unit::conversionMap.end())
  {
    return nullptr;
  }
  return unitIt->second;
}

DimensionedQuantity Unit::createDimensioned(double value, std::string fromUnit)
{
  auto unit = findUnit(fromUnit);
  return unit->createDimensioned(value);
}

DimensionedQuantity Unit::createDimensioned(double value)
{
  return DimensionedQuantity(dimension(), value * factor());
}

UnitQuantity UnitConv::convertTo(DimensionedQuantity *const quantity, std::string toUnit)
{
  auto unit = Unit::findUnit(toUnit);
  if (!unit)
  {
    return UnitQuantity(0, 0);
  }
  printf("qty %.10lf tounit factor %.10lf\n", quantity->normalizedValue(), unit->factor());
  return UnitQuantity(unit, quantity->normalizedValue() / unit->factor());
}

CompoundUnit::CompoundUnit(std::string unitName, std::set<std::string> unitAliases, std::map<std::string, double> unitComponents, double constant, UnitClass uClass) : Unit(unitName, unitAliases, uClass), constantMultiplier(constant)
{
  for (auto comp : unitComponents)
  {
    auto unit = findUnit(comp.first);
    if (unit)
    {
      components[comp.first] = std::make_pair(unit, comp.second);
    }
  }
}

DimensionType CompoundUnit::dimension()
{
  DimensionType ret = UnitConv::BIAS;
  for (auto comp : components)
  {
    auto unit = comp.second.first;
    auto power = comp.second.second;
    //printf("power %lf\n", power);
    //printf("unit %s dim %u\n", unit->name.c_str(), unit->dimension());
    ret += (unit->dimension() - UnitConv::BIAS) * power;
  }
  return ret;
}
double CompoundUnit::factor()
{
  double ret = 1;
  for (auto comp : components)
  {
    auto unit = comp.second.first;
    auto power = comp.second.second;
    ret *= pow(unit->factor(), power);
  }
  return ret * constantMultiplier;
}

DimensionedQuantity DimensionedQuantity::operator+(const DimensionedQuantity &other) const
{
  DimensionType dim = 0;
  if (other.dimension == dimension)
  {
    dim = dimension;
  }
  return DimensionedQuantity(dim, value + other.value);
}
DimensionedQuantity DimensionedQuantity::operator-(const DimensionedQuantity &other) const
{
  DimensionType dim = 0;
  if (other.dimension == dimension)
  {
    dim = dimension;
  }
  return DimensionedQuantity(dim, value - other.value);
}
DimensionedQuantity DimensionedQuantity::operator*(const DimensionedQuantity &other) const
{
  DimensionType dim = dimension + other.dimension - UnitConv::BIAS;
  return DimensionedQuantity(dim, value * other.value);
}
DimensionedQuantity DimensionedQuantity::operator/(const DimensionedQuantity &other) const
{
  DimensionType dim = dimension + UnitConv::BIAS - other.dimension;
  return DimensionedQuantity(dim, value / other.value);
}