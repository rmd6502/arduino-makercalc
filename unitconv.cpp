#include "unitconv.h"
#include <map>
#include <string>
#include <stdint.h>

Unit::ConversionMapType Unit::conversionMap = {};

DimensionedQuantity Unit::createDimensioned(double value) {
  return std::make_pair(value, dimension());
}

DimensionedQuantity UnitConv::convert(DimensionedQuantity quantity, std::string unit) {
  
}
