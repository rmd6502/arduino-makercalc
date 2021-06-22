#include <exception>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <stdint.h>
#include <vector>

class Unit;

using DimensionType = uint32_t;

enum UnitClass {
  UNITCLASS_SI_MKS,
  UNITCLASS_SI_CGS,
  UNITCLASS_IMPERIAL,
  UNITCLASS_OTHER,
};

// Represents a quantity in a specified unit
struct UnitQuantity
{
  DimensionType dimension;
  double value;
  std::shared_ptr<Unit> unit;
  UnitQuantity(std::shared_ptr<Unit> u, double q);
  
  // value is the converted value, so this returns
  // the value normalized to mks common units.
  double normalizedValue() const;
  UnitQuantity operator+(const UnitQuantity &other) const;
  UnitQuantity operator-(const UnitQuantity &other) const;
  UnitQuantity operator*(const UnitQuantity &other) const;
  UnitQuantity operator/(const UnitQuantity &other) const;
};

struct Unit : public std::enable_shared_from_this<Unit>
{
  using SPtr = std::shared_ptr<Unit>;
  std::string name;
  UnitClass unitClass;
  std::set<std::string> aliases;
  typedef std::map<std::string, SPtr> ConversionMapType;
  static ConversionMapType conversionMap;
  typedef std::map<DimensionType, std::vector<SPtr>> DimensionMapType;
  static DimensionMapType dimensionMap;
  Unit(std::string unitName, std::set<std::string> unitAliases, UnitClass uClass = UNITCLASS_SI_MKS) : name(unitName), aliases(unitAliases), unitClass(uClass)
  {
    conversionMap[unitName] = std::shared_ptr<Unit>(this);
    for (auto alias : unitAliases)
    {
      conversionMap[alias] = shared_from_this();
    }
  }

  virtual double factor() { return 0; }
  virtual DimensionType dimension() { return 0; }
  static SPtr findUnit(std::string unitName);
  static UnitQuantity createDimensioned(double value, std::string fromUnit);
  UnitQuantity createDimensioned(double value);
};

struct UnitConv
{
  static constexpr uint32_t LENGTH_UNIT = 1;
  static constexpr uint32_t TEMP_UNIT = 10;
  static constexpr uint32_t MASS_UNIT = 100;
  static constexpr uint32_t TIME_UNIT = 1000;
  static constexpr uint32_t BIAS = 5555;

  static UnitQuantity convertTo(UnitQuantity *const quantity, std::string toUnit);
  static Unit::SPtr unitForDimension(DimensionType dim, UnitClass uClass);
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
  SimpleUnit(std::string unitName, std::set<std::string> unitAliases, double factor, DimensionType dimensionType, UnitClass uClass = UNITCLASS_SI_MKS) : Unit(unitName, unitAliases, uClass), conversion(factor), dim(dimensionType) {
    dimensionMap[dim].push_back(shared_from_this());
  }
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
  CompoundUnit(std::string unitName, std::set<std::string> unitAliases, std::map<std::string, double> unitComponents, double constant = 1, UnitClass uClass = UNITCLASS_SI_MKS);
  DimensionType dimension();
  double factor();
};
