#include "type.h"

// each individual list is sorted alphabetically
vector<TypeDescriptor> StandardTypes = {
  TypeDescriptor({{"double", "long"}}, 16),
  TypeDescriptor({{"double"}}, 8),
  TypeDescriptor({{"float"}}, 4),
  TypeDescriptor({{"long", "long", "unsigned"}, {"int", "long", "long", "unsigned"}}, 16),
  TypeDescriptor({{"long", "long"}, {"long", "long", "signed"}, {"int", "long", "long"}, {"int", "long", "long", "signed"}}, 16),
  TypeDescriptor({{"long", "unsigned"}, {"int", "long", "unsigned"}}, 8),
  TypeDescriptor({{"long"}, {"long", "signed"}, {"int", "long"}, {"int", "long", "signed"}}, 8),
  TypeDescriptor({{"unsigned"}, {"int", "unsigned"}}, 4),
  TypeDescriptor({{"int"}, {"signed"}, {"int", "signed"}}, 4),
  TypeDescriptor({{"short", "unsigned"}, {"int", "short", "unsigned"}}, 2),
  TypeDescriptor({{"short"}, {"short", "signed"}, {"int", "short"}, {"int", "short", "signed"}}, 2),
  TypeDescriptor({{"char", "unsigned"}}, 1),
  TypeDescriptor({{"char", "signed"}}, 1),
  TypeDescriptor({{"char"}}, 1),
  TypeDescriptor({{"void"}}, 1), // it'll be easier to consider this 1
};