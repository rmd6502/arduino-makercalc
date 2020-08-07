#include <exception>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <stdint.h>
#include <vector>

class Unit;

using DimensionType = uint32_t;
using DimensionedQuantity = std::pair<DimensionType, double>;
using UnitQuantity = std::pair<std::shared_ptr<Unit>, double>;
  
struct UnitConv {
  static constexpr uint32_t LENGTH_UNIT = 1;
  static constexpr uint32_t TEMP_UNIT = 10;
  static constexpr uint32_t MASS_UNIT = 100;
  static constexpr uint32_t TIME_UNIT = 1000;
  static constexpr uint32_t BIAS = 5555;

  static DimensionedQuantity convertTo(DimensionedQuantity quantity, std::string unit);
};

struct Unit {
  std::string name;
  std::set<std::string> aliases;
  typedef std::map<std::string, std::shared_ptr<Unit>> ConversionMapType;
  static ConversionMapType conversionMap;
  Unit(std::string unitName, std::set<std::string> unitAliases) : name(unitName), aliases(unitAliases) {
    std::shared_ptr<Unit> sharedSelf(this);
    conversionMap[unitName] = sharedSelf;
    for (auto alias : unitAliases) {
      conversionMap[alias] = sharedSelf;
    }
  }

  static DimensionedQuantity createDimensioned(double value, std::string fromUnit);
  DimensionedQuantity createDimensioned(double value);
  virtual double factor() { return 0; }
  virtual DimensionType dimension() { return 0; }
  static std::shared_ptr<Unit> findUnit(std::string unitName);
};

struct ConstantUnit : Unit {
  double value;
  ConstantUnit(std::string unitName, double unitValue) : Unit(unitName, {}), value(unitValue) {}
  virtual double factor() { return value; }
  DimensionType dimension() { return UnitConv::BIAS; }
};

struct SimpleUnit : Unit {
  double conversion;
  DimensionType dim;
  SimpleUnit(std::string unitName, std::set<std::string> unitAliases, double factor, DimensionType dimensionType) : Unit(unitName, unitAliases), conversion(factor), dim(dimensionType) {}
  virtual double factor() { return conversion; }
  virtual DimensionType dimension() { return dim; }
};

struct CompoundUnit : Unit {
  // pair<Unit, power> - power can be negative
  // for instance kg m / s^2
  // would be [<kg,1>,<m,1>,<s,-2>]
  std::map<std::string, std::pair<std::shared_ptr<Unit>, double>> components;
  CompoundUnit(std::string unitName, std::set<std::string> unitAliases, std::map<std::string, double> unitComponents);
  DimensionType dimension();
  double factor();
};
