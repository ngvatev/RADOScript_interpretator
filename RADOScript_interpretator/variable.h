#ifndef RADO_var_H
#define RADO_var_H

#include <vector>
#include <string>
#include <memory>

class variable
{
private: // data
  static const char* RADO_type_str[];

public: // data
  enum RADO_type
    {
      RADO_string = 0,
      RADO_int,
      RADO_float,
      RADO_array,
      RADO_undefined
    };
  enum RADO_type type;

  typedef int int_t;
  typedef float float_t;
  typedef std::string string_t;
  typedef std::vector<variable *> array_t;

protected: //methods
  // throws runtime exception
  void operator_error (const std::string &op,
                       const variable &operand1,
                       const variable &operand2) const;

public: // methods

  static variable* create (int_t var_int);
  static variable* create (float_t var_float);
  static variable* create (const string_t &var_string);

  static variable* create (const array_t &var_array);
  //static variable* create (
//      const std::vector<std::unique_ptr<variable>> &var_array);

  static variable* create (const variable &var);

  //some operators are not allowed for some variables

  // creates a new variable object that needs to be deleted
  virtual variable* operator+ (const variable &operand2_var) const;

  // creates a new variable object that needs to be deleted
  virtual variable* operator- (const variable &operand2_var) const;

  // creates a new variable object that needs to be deleted
  virtual variable* operator* (const variable &operand2_var) const;

  // creates a new variable object that needs to be deleted
  virtual variable* operator/ (const variable &operand2_var) const;

  // creates a new variable object that needs to be deleted
  virtual variable* operator- () const;

  // returns pointer to pointer pointing to
  // the pointer pointing to the variable with the wanted index
  virtual variable** operator[] (const variable &index);

  std::string info () const;
  virtual std::string to_string () const = 0;

  virtual ~variable () {};
};


#endif
