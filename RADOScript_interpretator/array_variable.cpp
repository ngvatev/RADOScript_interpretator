#include "array_variable.h"
#include "number_variable.h"
array_variable::array_variable (const array_t &data_array):
  data ()
{
  array_t::const_iterator it = data_array.begin (),
                                          it_e = data_array.end ();

  for (; it != it_e; ++it)
    {
      this->data.push_back(variable::create (**it));
    }

  this->type = variable::RADO_array;
}

const array_variable::array_t&
array_variable::get_data () const
{
  return this->data;
}

variable*
array_variable::operator+ (const variable &operand2_var) const
{
  if (operand2_var.type != variable::RADO_array)
    this->operator_error ("unsuported operation types, operator+",
                          *this,
                          operand2_var);


  const array_variable &opernad2_array =
      dynamic_cast<const array_variable &> (operand2_var);


  array_t result_vector (opernad2_array.get_data ());
  result_vector.insert (result_vector.end (),
                        opernad2_array.get_data ().begin (),
                        opernad2_array.get_data ().end ());

  return variable::create (result_vector);
;
}

variable*
array_variable::operator- (const variable &operand2_var) const
{
  if (operand2_var.type != variable::RADO_int)
    this->operator_error ("unsuported operation types, operator-",
                          *this,
                          operand2_var);

  const number_variable<int> &opernad2_int =
      dynamic_cast<const number_variable<int> &> (operand2_var);

  int n_to_remove = opernad2_int.get_data ();
  if (n_to_remove < 0 || n_to_remove > static_cast<int> (this->data.size ()))
    this->operator_error ("deleting more variables than array has, operator-",
                          *this,
                          operand2_var);


  array_t::const_iterator it_b = this->data.begin(),
      it_e = this->data.end ();
  std::advance (it_e, -n_to_remove);

  array_t result_vector (it_b, it_e);

  return variable::create (result_vector);
}


variable**
array_variable::operator[] (const variable &index)
{
  if (index.type != variable::RADO_int)
    this->operator_error ("unsuported operation types, operator[]",
                          *this,
                          index);

  const number_variable<int> &var_int =
      dynamic_cast<const number_variable<int> &> (index);
  int index_int = var_int.get_data ();

  if (index_int >= this->data.size ())
    this->operator_error ("out of range, operator[]",
                          *this,
                          index);

  variable **ptr = &this->data[index_int];
  return ptr;
}

std::string
array_variable::to_string () const
{
  std::string result;

  array_t::const_iterator it = this->data.begin (),
      it_e = this->data.end ();
  result.assign ("[");
  if (it != it_e)
    {
      result.append ((*it)->to_string ());
      ++it;
    }
  for (; it != it_e; ++it)
    {
      result.append (", ");
      result.append ((*it)->to_string ());
    }
  result.append ("]");
  return result;
}


array_variable::~array_variable ()
{
  array_t::const_iterator it = this->data.begin (),
      it_e = this->data.end ();
  for (; it != it_e; ++it)
    {
      delete *it;
    }
}
