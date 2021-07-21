#include "utils.h"

#include <unordered_map>
#include <vector>
#include <string>

using std::string;
// class Type {


// };

// We can have multiple qualifiers, so they can be
// or'd together
enum Qualifier {
  NONE = 0,
  CONST = 1,
  RESTRICT = 2,
  VOLATILE = 4,
};

struct TypeDescriptor {

  TypeDescriptor(std::vector<string> s, int w)
  {
    specifiers = s;
    words = w;
  }

  std::vector<string> specifiers;
  int words;

  // NOTE -- it would almost certainly be more
  // performant to organize the specifiers in a
  // standard way, making checking much faster
  bool operator==(TypeDescriptor other) {
    return MatchingVector(specifiers, other.specifiers);
  }

};

std::vector<TypeDescriptor> StandardTypes;

class Pointer {
  public:
    Pointer(int l = 0, int q = Qualifier::NONE) {
      level = l;
      qualifiers = q;
    }
    ~Pointer() {}

    bool operator==(Pointer other) {
      return qualifiers == other.qualifiers && level == other.level;
    }

    int qualifiers;
    // Zero indicates no pointer present
    int level;
    // Pointers will always be one word for my cpu
    static constexpr int words = 1;
};

struct Type {

  enum StorageClass {
    AUTO = 0,
    STATIC,
    TYPEDEF,
    EXTERN,
    REGISTER,
  };

  enum FunctionSpecifier {
    NONE = 0,
    INLINE = 1,
  };

  Type(string n, StorageClass s = StorageClass::AUTO, int q = Qualifier::NONE) {
    name = n;
    storage = s;
    qualifiers = q;
  }

  bool operator==(Type other) {
    bool matching = true;
    if (!MatchingVector(type_specifiers, other.type_specifiers))
      return false;
    if (qualifiers != other.qualifiers || storage != other.storage || function != other.function)
      return false;
    if (!MatchingVector(pointers, other.pointers))
      return false;
    
    return true;
  }

  Type copy()
  {
    Type newtype(name, storage, qualifiers);
    newtype.type_specifiers = type_specifiers;
    newtype.function = function;
    newtype.pointers = pointers;
    return newtype;
  }

  bool isConst()
  {
    return qualifiers & (Qualifier::CONST);
  }

  bool isPointer()
  {
    return pointers.size() > 0;
  }

  Type pointsTo()
  {
    if (!isPointer())
      throw(1);
    
    Type pointed = copy();
    pointed.pointers.erase(0);
    return pointed;
  }

  string name;
  std::vector<string> type_specifiers;
  int qualifiers;
  StorageClass storage;
  FunctionSpecifier function;
  // each star can have its own type qualifiers
  std::vector<Pointer> pointers;
};

// TODO -- now we need struct, array, and union classes