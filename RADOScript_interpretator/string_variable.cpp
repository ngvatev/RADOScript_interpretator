#include "string_variable.h"

string_variable::string_variable (const string_t &var_string):
  data (var_string)
{
  this->type = variable::RADO_string;
}


const string_variable::string_t&
string_variable::get_data () const
{
  return this->data;
}


variable*
string_variable::operator+ (const variable& operand2_var) const
{
  if (operand2_var.type != variable::RADO_string)
    this->operator_error ("unsuported operation types, operator+",
                          *this,
                          operand2_var);

  const string_variable& operand2_str_var =
      dynamic_cast<const string_variable &> (operand2_var);

  return variable::create (this->data + operand2_str_var.get_data ());
}


std::string
string_variable::to_string () const
{
  std::string result;
  result.assign ("\"").
         append (this->get_data ()).
         append ("\"");
  return result;
}

