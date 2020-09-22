#include "unitconv.h"

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

UnitQuantity::UnitQuantity(std::shared_ptr<Unit> u, double q) : unit(u), value(q)
{
  dimension = unit->dimension();
}

double UnitQuantity::normalizedValue() const { return value * unit->factor(); }

UnitQuantity Unit::createDimensioned(double value, std::string fromUnit)
{
  auto unit = findUnit(fromUnit);
  if (unit) {
    return unit->createDimensioned(value);
  }
  return UnitQuantity(nullptr,0);
}

UnitQuantity Unit::createDimensioned(double value)
{
  return UnitQuantity(shared_from_this(), value);
}

UnitQuantity UnitConv::convertTo(UnitQuantity *const quantity, std::string toUnit)
{
  auto unit = Unit::findUnit(toUnit);
  if (!unit)
  {
    return UnitQuantity(nullptr, 0);
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

UnitQuantity UnitQuantity::operator+(const UnitQuantity &other) const
{
  DimensionType dim = 0;
  double val = other.value;
  if (other.dimension == dimension)
  {
    dim = dimension;
    if (other.unit != unit) {
      val = other.normalizedValue() * unit->factor();
    }
  }
  return UnitQuantity(unit, value + val);
}
UnitQuantity UnitQuantity::operator-(const UnitQuantity &other) const
{
  DimensionType dim = 0;
  double val = other.value;
  if (other.dimension == dimension)
  {
    dim = dimension;
    if (other.unit != unit) {
      val = other.normalizedValue() * unit->factor();
    }
  }
  return UnitQuantity(unit, value - val);
}
UnitQuantity UnitQuantity::operator*(const UnitQuantity &other) const
{
  DimensionType dim = dimension + other.dimension - UnitConv::BIAS;
  return UnitQuantity(unit, value * other.value);
}
UnitQuantity UnitQuantity::operator/(const UnitQuantity &other) const
{
  DimensionType dim = dimension + UnitConv::BIAS - other.dimension;
  return UnitQuantity(unit, value / other.value);
}
