
#include <unordered_map>
#include <vector>
#include <string>

using std::string;
// class Type {


// };



struct TypeDescriptor {

  string name;
  std::vector<string> modifiers;
  bool isStruct;
  bool isTypedef;

};

struct Type {

  enum StorageClass {
    AUTO = 0,
    STATIC,
    TYPEDEF,
    EXTERN,
    REGISTER,
  };

  Type(string n, int ptr = 0, StorageClass s = StorageClass::AUTO) {
    name = n;
    pointerLevel = ptr;
    storage = s;
  }

  std::vector<string> type_specifiers;

  string name;
  StorageClass storage;
  int pointerLevel; // 0 = not a pointer, 1 = pointer to <name>, and so on
};