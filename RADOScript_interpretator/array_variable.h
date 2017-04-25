#ifndef ARRAY_VARIABLE_H_
#define ARRAY_VARIABLE_H_

#include "variable.h"

#include <vector>

class array_variable : public variable
{
private: // data
  array_t data;
public: // methods


  // constructs a copy of the data in data_array
  array_variable (const array_t &data_array);


  const array_t& get_data () const;

  // creates new variable that is concatenation 2 arrays
  // operand2_var is required to be array_variable
  variable* operator+ (const variable &operand2_var) const;

  // creates new variable with
  // the data of this variable minus the last var elements
  // operand2_var is required to be number_variable<int>
  variable* operator- (const variable &operand2_var) const;


  variable** operator[] (const variable &index);

  std::string to_string () const;

  ~array_variable ();
};

#endif /* ARRAY_VARIABLE_H_ */
