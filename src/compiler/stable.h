
#include <vector>
#include <string>

#include "identifier.h"

using std::string;

struct SymbolTable {

  enum Scope {
    GLOBAL = 0,
    FUNCTION,
    LOCAL,
  };

  SymbolTable(SymbolTable* p, Scope s)
  {
    parent = p;
    scope = s;
  }

  // TODO -- this needs to check if the symbol already exists. 
  // If it's a defined function or a declared variable, then
  // add an error or throw an exception or something
  void AddSymbol(Identifier id);

  /** Find a local symbol with the identifier `name`
   * 
   */
  Identifier GetLocalSymbol(string name);

  /** Search the symbol table tree for a symbol with the identifier `name`
   * 
   */
  Identifier GetSymbol(string name);

  SymbolTable* parent;
  // Doesn't need to know child, since we only
  // need to walk up the tree
  // std::vector<SymbolTable> children;

  Scope scope;
  // string identifier;
  // std::vector<string> symbols;
  std::vector<Identifier> symbols;

};