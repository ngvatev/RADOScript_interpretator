#include "variable.h"
#include "number_variable.h"
#include "string_variable.h"
#include "array_variable.h"

#include <iostream>
#include <exception>

const char* variable::RADO_type_str[] =
  {
    "RADO_string",
    "RADO_int",
    "RADO_float",
    "RADO_array",
    "RADO_undefined"
  };


variable*
variable::create (int_t var_int)
{
  return new number_variable<int> (var_int);
}


variable*
variable::create (float_t var_float)
{
  return new number_variable<float> (var_float);
}


variable*
variable::create (const string_t &var_string)
{
  return new string_variable (var_string);
}


variable*
variable::create (const array_t &var_array)
{
  return new array_variable (var_array);
}


variable*
variable::create (const variable &var)
{
  variable *result;
  switch (var.type)
    {
      case variable::RADO_int:
        {
          const number_variable<int> &var_int =
              dynamic_cast<const number_variable<int> &> (var);

          result = variable::create (var_int.get_data());
          break;
        }
      case variable::RADO_float:
        {
          const number_variable<float> &var_float=
              dynamic_cast<const number_variable<float> &> (var);

          result = variable::create (var_float.get_data());
          break;
        }
      case variable::RADO_string:
        {
          const string_variable &var_string =
              dynamic_cast<const string_variable &> (var);

          result = variable::create (var_string.get_data());
          break;
        }
      case variable::RADO_array:
        {
          const array_variable &var_array =
              dynamic_cast<const array_variable &> (var);

          result = variable::create (var_array.get_data());
          break;
        }
      default:
        break;
    }
  return result;
}


variable*
variable::operator+ (const variable &operand2_var) const
{
  this->operator_error ("unsupported, operator+",
                        *this,
                        operand2_var);
  return NULL;
}


variable*
variable::operator- (const variable &operand2_var) const
{
  this->operator_error ("unsupported, operator-",
                        *this,
                        operand2_var);
  return NULL;
}


variable*
variable::operator* (const variable &operand2_var) const
{
  this->operator_error ("unsupported, operator*",
                        *this,
                        operand2_var);
  return NULL;
}


variable*
variable::operator/ (const variable &operand2_var) const
{
  this->operator_error ("unsupported, operator/",
                        *this,
                        operand2_var);
  return NULL;
}


variable*
variable::operator- () const
{
  std::string error ("unsupported, operator-()");
  error.append ("\noperand:").
      append (this->info ());
  throw (std::runtime_error (error));
}


variable**
variable::operator[] (const variable &index)
{
  this->operator_error ("unsupported, operator[]",
                        *this,
                        index);
  return NULL;
}


std::string
variable::info () const
{
  std::string info;
  info.assign ("type:\"").
       append (this->RADO_type_str[this->type]).
       append ("\" ").
       append ("value:\"").
       append (this->to_string ()).
       append ("\"");
  return info;
}


void
variable::operator_error (const std::string &op,
                          const variable &operand1,
                          const variable &operand2) const
{
  std::string error (op);
  error.append ("\noperand1:").
      append (operand1.info ()).
      append ("\noperand2:").
      append (operand2.info());
  throw (std::runtime_error (error));
}
