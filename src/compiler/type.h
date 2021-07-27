#ifndef TYPE_H
#define TYPE_H

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

// std::vector<TypeDescriptor> StandardTypes;

class Pointer {
  public:
    Pointer(int q = Qualifier::NONE) {
      qualifiers = q;
    }
    ~Pointer() {}

    void setQualifier(string q) 
    {
      int temp = qualifiers;
      if (q == "const") {
        temp |= Qualifier::CONST;
      } else if (q == "restrict") {
        temp |= Qualifier::RESTRICT;
      } else if (q == "volatile") {
        temp |= Qualifier::VOLATILE;
      }
      qualifiers = temp;
    }

    void setQualifiers(std::vector<string> quals)
    {
      int temp = qualifiers;
      for (auto q : quals) {
        if (q == "const") {
          if (temp & (int) Qualifier::CONST) throw 1;
          temp |= Qualifier::CONST;
        } else if (q == "restrict") {
          if (temp & (int) Qualifier::RESTRICT) throw 1;
          temp |= Qualifier::RESTRICT;
        } else if (q == "volatile") {
          if (temp & (int) Qualifier::VOLATILE) throw 1;
          temp |= Qualifier::VOLATILE;
        }
      }
      qualifiers = temp;
    }

    bool operator==(Pointer other) {
      return qualifiers == other.qualifiers;
    }

    bool operator!=(Pointer other) {
      return qualifiers != other.qualifiers;
    }

    int qualifiers;
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

  Type() {
    storageSet = false;
    name = "";
    storage = StorageClass::AUTO;
    function = FunctionSpecifier::NONE;
    qualifiers = 0;
  }

  // Type(string n, StorageClass s = StorageClass::AUTO, int q = Qualifier::NONE) {
  //   name = n;
  //   storage = s;
  //   qualifiers = q;
  // }

  bool operator==(Type& other) {
    return equal(other);
  }

  bool operator!=(Type& other) {
    return !equal(other);
  }

  Type copy()
  {
    // Type newtype(name, storage, qualifiers);
    Type newtype;
    newtype.storageSet = storageSet;
    newtype.storage = storage;
    newtype.qualifiers = qualifiers;
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
    pointed.pointers.erase(pointed.pointers.begin());
    return pointed;
  }

  void setStorage(string s)
  {
    if (s == "typedef") {
      storage = StorageClass::TYPEDEF;
    } else if (s == "auto") {
      storage = StorageClass::AUTO;
    } else if (s == "static") {
      storage = StorageClass::STATIC;
    } else if (s == "extern") {
      storage = StorageClass::EXTERN;
    } else if (s == "register") {
      storage = StorageClass::REGISTER;
    }
    storageSet = true;
  }

  string name;
  std::vector<string> type_specifiers;
  int qualifiers;
  StorageClass storage;
  FunctionSpecifier function;
  bool storageSet;
  // each star can have its own type qualifiers
  std::vector<Pointer> pointers;

  private:
    bool equal(Type& other)
    {
      bool matching = true;
      if (!MatchingVector(type_specifiers, other.type_specifiers))
        return false;
      if (qualifiers != other.qualifiers || storage != other.storage || function != other.function)
        return false;
      if (!EqualVectors(pointers, other.pointers))
        return false;
      
      return true;
    }

};

#endif

// TODO -- now we need struct, array, and union classes