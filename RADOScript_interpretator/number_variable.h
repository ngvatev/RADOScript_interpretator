#ifndef NUMBER_VARIABLE_H_
#define NUMBER_VARIABLE_H_

#include "variable.h"

template <class T>
class number_variable : public variable
{
private: // data
  T data;
private: // methods

  // helper for the operators
  variable* operator_helper (const char operation,
                             const variable &operand2_var) const;

  template <class V>
  variable* execute_operation (const char operation,
                               const variable &operand2) const;

public: //methods
  number_variable (T data);

  T get_data () const;

  variable* operator+ (const variable &var) const;
  variable* operator- (const variable &var) const;
  variable* operator* (const variable &var) const;
  variable* operator/ (const variable &var) const;
  variable* operator- () const;

  std::string to_string () const;
};

#endif /* NUMBER_VARIABLE_H_ */
