#include <exception>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <stdint.h>
#include <vector>

class Unit;

using DimensionType = uint32_t;

// Represents a quantity normalized to the mks system
struct DimensionedQuantity
{
  DimensionType dimension;
  double value;
  DimensionedQuantity(DimensionType dim, double q) : dimension(dim), value(q) {}
  explicit DimensionedQuantity(double q) : dimension(0), value(q) {}
  virtual double normalizedValue() { return value; }
  DimensionedQuantity operator+(const DimensionedQuantity &other) const;
  DimensionedQuantity operator-(const DimensionedQuantity &other) const;
  DimensionedQuantity operator*(const DimensionedQuantity &other) const;
  DimensionedQuantity operator/(const DimensionedQuantity &other) const;
};

enum UnitClass {
  UNITCLASS_SI,
  UNITCLASS_IMPERIAL,
  UNITCLASS_OTHER,
};

struct Unit
{
  std::string name;
  UnitClass unitClass;
  std::set<std::string> aliases;
  typedef std::map<std::string, std::shared_ptr<Unit>> ConversionMapType;
  static ConversionMapType conversionMap;
  Unit(std::string unitName, std::set<std::string> unitAliases, UnitClass uClass = UNITCLASS_SI) : name(unitName), aliases(unitAliases), unitClass(uClass)
  {
    std::shared_ptr<Unit> sharedSelf(this);
    conversionMap[unitName] = sharedSelf;
    for (auto alias : unitAliases)
    {
      conversionMap[alias] = sharedSelf;
    }
  }

  static DimensionedQuantity createDimensioned(double value, std::string fromUnit);
  DimensionedQuantity createDimensioned(double value);
  virtual double factor() { return 0; }
  virtual DimensionType dimension() { return 0; }
  static std::shared_ptr<Unit> findUnit(std::string unitName);
};

// Represents a quantity in a specified unit
struct UnitQuantity : public DimensionedQuantity
{
  std::shared_ptr<Unit> unit;
  UnitQuantity(std::shared_ptr<Unit> u, double q) : DimensionedQuantity(q)
  {
    dimension = u->dimension();
  }
  // value is the converted value, so this returns
  // the value normalized to mks common units.
  double normalizedValue() { return value / unit->factor(); }
};

struct UnitConv
{
  static constexpr uint32_t LENGTH_UNIT = 1;
  static constexpr uint32_t TEMP_UNIT = 10;
  static constexpr uint32_t MASS_UNIT = 100;
  static constexpr uint32_t TIME_UNIT = 1000;
  static constexpr uint32_t BIAS = 5555;

  static UnitQuantity convertTo(DimensionedQuantity *const quantity, std::string toUnit);
};

struct ConstantUnit : Unit
{
  double value;
  ConstantUnit(std::string unitName, double unitValue) : Unit(unitName, {}), value(unitValue) {}
  virtual double factor() { return value; }
  DimensionType dimension() { return UnitConv::BIAS; }
};

struct SimpleUnit : Unit
{
  double conversion;
  DimensionType dim;
  SimpleUnit(std::string unitName, std::set<std::string> unitAliases, double factor, DimensionType dimensionType, UnitClass uClass = UNITCLASS_SI) : Unit(unitName, unitAliases, uClass), conversion(factor), dim(dimensionType) {}
  virtual double factor() { return conversion; }
  virtual DimensionType dimension() { return dim; }
};

struct CompoundUnit : Unit
{
  // pair<Unit, power> - power can be negative
  // for instance kg m / s^2
  // would be [<kg,1>,<m,1>,<s,-2>]
  double constantMultiplier;
  std::map<std::string, std::pair<std::shared_ptr<Unit>, double>> components;
  CompoundUnit(std::string unitName, std::set<std::string> unitAliases, std::map<std::string, double> unitComponents, double constant = 1, UnitClass uClass = UNITCLASS_SI);
  DimensionType dimension();
  double factor();
};
