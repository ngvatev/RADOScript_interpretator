#include "number_variable.h"

#include <typeinfo>

template <class T>
number_variable<T>::number_variable (T data)
{
  this->data = data;

  size_t data_hash_code = typeid (T).hash_code ();

  if (data_hash_code == typeid (variable::int_t).hash_code ())
    {
      this->type = variable::RADO_int;
    }
  else if (data_hash_code == typeid (variable::float_t).hash_code ())
    {
      this->type = variable::RADO_float;
    }
  else
    {
      this->type = variable::RADO_undefined;
    }
}

template <class T>
T
number_variable<T>::get_data () const
{
  return this->data;
}

template <class T>
variable*
number_variable<T>::operator+ (const variable &operand2_var) const
{
  return this->operator_helper ('+', operand2_var);
}

template <class T>
variable*
number_variable<T>::operator- (const variable &operand2_var) const
{
  return this->operator_helper ('-', operand2_var);
}

template <class T>
variable*
number_variable<T>::operator* (const variable &operand2_var) const
{
  return this->operator_helper ('*', operand2_var);
}

template <class T>
variable*
number_variable<T>::operator/ (const variable &operand2_var) const
{
  return this->operator_helper ('/', operand2_var);
}


template <class T>
variable*
number_variable<T>::operator- () const
{
  variable *result = variable::create (-(this->get_data()));
  return result;
}

template <class T>
std::string
number_variable<T>::to_string () const
{
  return std::to_string (this->get_data ());
}

template <class T>
template <class V>
variable*
number_variable<T>::execute_operation (const char operation,
                                       const variable &operand2_var) const
{
  T operand1 = this->get_data ();
  V operand2 =
      (dynamic_cast<const number_variable<V> &> (operand2_var)).get_data ();


  switch (operation)
    {
    case '+':
      return variable::create (operand1 + operand2);
    case '-':
      return variable::create (operand1 - operand2);
    case '*':
      return variable::create (operand1 * operand2);
    case '/':
      if (operand2 == 0)
        this->operator_error ("division by 0, operator/",
                              *this,
                              operand2_var);
      return variable::create (operand1 / operand2);
    }
  return NULL;
}


template <class T>
variable*
number_variable<T>::operator_helper (const char operation,
                                     const variable &operand2_var) const
{
  variable* result = NULL;
  switch (operand2_var.type)
    {
      case variable::RADO_int:
        {
          return this->execute_operation<int_t> (operation,
                                                 operand2_var);
        }
      case variable::RADO_float:
        {
          return this->execute_operation<float_t> (operation,
                                                   operand2_var);
        }
      default:
        {
          std::string error_message ("unsuported operation types, operator");
          error_message.push_back (operation);
          this->operator_error (error_message,
                                *this,
                                operand2_var);
        }
    }
  return result;
}


template class number_variable<variable::int_t>;
template class number_variable<variable::float_t>;
