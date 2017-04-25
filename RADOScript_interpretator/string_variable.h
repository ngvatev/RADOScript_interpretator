#ifndef STRING_VARIABLE_H_
#define STRING_VARIABLE_H_

#include <string>

#include "variable.h"


class string_variable : public variable
{
private: // data
  std::string data;
private: //methods
public: //methods

  string_variable (const string_t &var_string);

  const string_t& get_data () const;

  // concatenates 2 strings_variables and returns a NEW variable
  variable* operator+ (const variable &var) const;

  std::string to_string () const;
};

#endif /* STRING_VARIABLE_H_ */
