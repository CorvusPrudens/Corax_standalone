
#ifndef ERROR_H
#define ERROR_H

#include <string>
#include <vector>

using std::string;

class Error {

  public:
    Error() {}
    ~Error() {}

    struct item {
      string message;
      int line;
      string file;
      int code;
    };

    void AddError(string message, int line, string file, int code, bool fatal = false);
    void AddWarning(string message, int line, string file, int code);

    void Report();

  private:

    void PrintErrors();
    void PrintWarnings();

    static constexpr int max_errors = 10;
    std::vector<item> errors;
    std::vector<item> warnings;

};

#endif // ERROR_H