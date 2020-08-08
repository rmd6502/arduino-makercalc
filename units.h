#ifndef __UNITS_H__
#define __UNITS_H__

static SimpleUnit *inch = new SimpleUnit("inch", {"in", "inches", "\""}, 0.0254, UnitConv::LENGTH_UNIT + UnitConv::BIAS);
static SimpleUnit *foot = new SimpleUnit("foot", {"ft", "feet", "'"}, 0.0254 * 12, UnitConv::LENGTH_UNIT + UnitConv::BIAS);
static SimpleUnit *yard = new SimpleUnit("yard", {"yd", "yards"}, 0.0254 * 36, UnitConv::LENGTH_UNIT + UnitConv::BIAS);
static SimpleUnit *mile = new SimpleUnit("mile", {"mi", "miles"}, 0.0254 * 5280, UnitConv::LENGTH_UNIT + UnitConv::BIAS);
static SimpleUnit *mm = new SimpleUnit("mm", {"millimeter", "millimeters"}, 0.001, UnitConv::LENGTH_UNIT + UnitConv::BIAS);
static SimpleUnit *cm = new SimpleUnit("cm", {"centimeter", "centimeters"}, 0.01, UnitConv::LENGTH_UNIT + UnitConv::BIAS);
static SimpleUnit *m = new SimpleUnit("m", {"meter", "meters"},1, UnitConv::LENGTH_UNIT + UnitConv::BIAS);


static CompoundUnit *sqin = new CompoundUnit("sqin",std::set<std::string>{"in2","in²","square inches", "square inch"},
  std::map<std::string, double>{{"inch", 2}});
static CompoundUnit *sqyd = new CompoundUnit("sqyd",std::set<std::string>{"yd2","yd²","square yards", "square yard"},
  std::map<std::string, double>{{"yard", 2}});
static CompoundUnit *sqcm = new CompoundUnit("sqcm",std::set<std::string>{"cm2","cm²","square centimeter", "square centimeters"},
  std::map<std::string, double>{{"cm", 2}});
static CompoundUnit *sqm = new CompoundUnit("sqm",std::set<std::string>{"m2","m²","square meters", "square meter"},
  std::map<std::string, double>{{"m", 2}});
static CompoundUnit *cucm = new CompoundUnit("cucm",std::set<std::string>{"cm3","cm³","cubic centimeter", "cubic centimeters"},
  std::map<std::string, double>{{"cm", 3}});
static CompoundUnit *l = new CompoundUnit("l", std::set<std::string>{"liter", "liters"},
    std::map<std::string, double>{{"cm", 3}}, 1000);

static CompoundUnit *quart = new CompoundUnit("quart", std::set<std::string>{"quart", "quarts"},
    std::map<std::string, double>{{"cm", 3}}, 946);
static CompoundUnit *oz = new CompoundUnit("ounce av", std::set<std::string>{"oz av"},
    std::map<std::string, double>{{"quart", 1}}, 1./32.);
static CompoundUnit *gal = new CompoundUnit("gallon", std::set<std::string>{"gal", "gallons"},
    std::map<std::string, double>{{"quart", 1}}, 4);


static CompoundUnit *acre = new CompoundUnit("acre",std::set<std::string>{},
  std::map<std::string, double>{{"sqyd", 1}}, 4840);

#endif