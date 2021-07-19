
#include <vector>
#include <string>

using std::string;

struct SymbolTable {

  SymbolTable* parent;
  std::vector<SymbolTable> children;

  enum Type {
    GLOBAL = 0,
    FUNCTION,
    LOCAL,
  };

  Type type;
  string identifier;
  std::vector<string> symbols;


};