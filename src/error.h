
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
      int colstart;
      int colend;
    };

    void AddError(
      string message, int line, 
      string file, int code = 1, 
      int colstart = -1, int colend = -1, 
      bool fatal = false
    );
    void AddWarning(
      string message, int line, 
      string file, int code = 2, 
      int colstart = -1, int colend = -1
    );

    void Report();

  private:

    void PrintErrors();
    void PrintWarnings();

    static constexpr int max_errors = 10;
    std::vector<item> errors;
    std::vector<item> warnings;

};

#endif // ERROR_H