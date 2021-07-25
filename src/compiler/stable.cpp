#include "stable.h"

void SymbolTable::AddSymbol(Identifier id)
{ 
  Identifier other;
  try {
    // We only need to worry about colliding with local symbols
    other = GetLocalSymbol(id.name);
  } catch (int e) {
    symbols.push_back(id); 
    return;
  }

  // If it doesn't throw an exception, then
  // the identifier has already been 
  // at least declared
  if (id.type == Identifier::IdType::FUNCTION && other.type == Identifier::IdType::FUNCTION)
  {
    if (!other.initialized && id == other)
      symbols.push_back(id);
    else if (!other.initialized)
      throw 2;
    else
      throw 3;
  }
  else
    throw 1;
}

Identifier SymbolTable::GetLocalSymbol(string name)
{
  for (int i = symbols.size() - 1; i > -1; i--)
  {
    if (symbols[i].name == name)
      return symbols[i];
  }
  throw 1;
}

Identifier SymbolTable::GetSymbol(string name)
{
  // first check local copy
  try {
    return GetLocalSymbol(name);
  } catch (int e) {
    // then check parents up the tree
    if (parent == nullptr)
      throw 1;
    else
      return parent->GetSymbol(name);
  }
}