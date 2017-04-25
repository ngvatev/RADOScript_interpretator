#include "RADOScriptinterpretator.h"

#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stack>
#include <memory>

// the new line is a delimiter that has the same use
// as ; in c++
const std::string RADOScript_interpretator::script_delimiters = "=#\n+-*/*[],.\"";

RADOScript_interpretator::RADOScript_interpretator ():
  script_tokenizer (RADOScript_interpretator::script_delimiters)
{

}


bool
RADOScript_interpretator::execute (const std::string &file_name)
{
  // open file
  std::ifstream ifs (file_name);
  if (!ifs.is_open())
    {
      throw (std::runtime_error ("script not found"));
      return false;
    }
  std::string row;


  // get the script from the file into a string
  std::string script;
  ifs.seekg(0, std::ios::end);
  script.resize(ifs.tellg());

  ifs.seekg(0, std::ios::beg);
  ifs.read(&script[0], script.size());


  if (this->parse_text (script) == false)
    { // script has syntax errors
      throw (std::runtime_error ("script has syntax errors"));
      return false;
    }

  return true;
}

void
RADOScript_interpretator::clear ()
{
  std::map<std::string, variable*>::iterator variables_it = variables.begin (),
                                             variables_it_e = variables.end ();
  for (; variables_it != variables_it_e; ++variables_it)
    {
      delete variables_it->second;
    }
  variables.clear ();
}

RADOScript_interpretator::~RADOScript_interpretator ()
{
  this->clear ();
}

bool
RADOScript_interpretator::parse_text (const std::string &text)
{
  this->script_tokenizer.set_text (text);

  // parse until end
  // the interpretator currently supports only
  // printing # or assignment
  while (this->script_tokenizer.end () == false)
    {
      if (this->script_tokenizer.next () == false)
        break;
      // we have assignment or outputing
      if (this->script_tokenizer.token ().compare ("#") == 0)
        {
          if (this->print_expression () == false)
            return false;
        }
      else
        if (this->variable_assignment () == false)
          return false;
    }
  return true;
}


bool
RADOScript_interpretator::print_expression ()
{
  if (this->script_tokenizer.token ().compare ("#") != 0)
    return false;

  if (this->script_tokenizer.next () == false)
    return false;

  variable *tmp = this->evaluate_expression ();
  std::cout<<tmp->to_string()<<std::endl;
  delete tmp;
  return true;
}


bool
RADOScript_interpretator::variable_assignment ()
{
  variable **var = parse_variable ();
  if (var == NULL)
    return false;

  if (this->script_tokenizer.token ().compare ("=") != 0 ||
      this->script_tokenizer.next () == false)
    return false;

  variable* new_value = this->evaluate_expression ();
  delete *var;
  *var = new_value;
  if (*var == NULL)
    return false;
  return true;
}


variable**
RADOScript_interpretator::parse_variable ()
{
  if (this->script_tokenizer.isdelimiter ())
    return NULL;

  std::string key = this->script_tokenizer.token();

  std::pair<std::string, variable*> pair (key, NULL);
  this->variables.insert (pair);

  variable** var = &this->variables.find (key)->second;


  while (this->script_tokenizer.next () != false &&
         this->script_tokenizer.token ().compare ("[") == 0)
    {
      std::unique_ptr<variable> index (this->parse_index ());
      if (!index)
        return NULL;
      var = (**var)[*index];
      if (var == NULL)
        return NULL;
    }

  return var;
}

variable*
RADOScript_interpretator::parse_index ()
{
  if (this->script_tokenizer.token ().compare ("[") != 0)
    { // we have index
       return NULL;
    }
  if (this->script_tokenizer.next () == false)
    return NULL;
  variable* index = this->evaluate_expression ();
  if (this->script_tokenizer.token ().compare ("]") != 0)
    return NULL;

  return index;
}

variable*
RADOScript_interpretator::parse_string ()
{
  if (this->script_tokenizer.token ().compare ("\"") != 0 ||
      this->script_tokenizer.next () == false ||
      this->script_tokenizer.isdelimiter ())
    return NULL;

  variable* string_var =
      variable::create (this->script_tokenizer.token ());

  if (this->script_tokenizer.next () == false ||
      this->script_tokenizer.token ().compare ("\"") != 0)
    {
      delete string_var;
      return NULL;
    }
  this->script_tokenizer.next ();
  return string_var;
}

variable*
RADOScript_interpretator::parse_number ()
{
  std::string number;
  if (this->script_tokenizer.token ().compare ("-") == 0)
    {
      number.append ("-");
      if (this->script_tokenizer.next () == 0)
        return NULL;
    }

  if (this->script_tokenizer.isdelimiter ())
    return NULL;

  number.append (this->script_tokenizer.token ());

  // TODO number may be a nonvalid number after the parse
  // ex. 123ASD
  variable *number_var = NULL;
  if (this->script_tokenizer.next () != false &&
      this->script_tokenizer.token ().compare (".") == 0)
    {
      if (this->script_tokenizer.next () == false ||
          this->script_tokenizer.isdelimiter ())
        return NULL;
      number.append (".").append (this->script_tokenizer.token ());

      this->script_tokenizer.next ();

      float var_float = strtof (number.c_str (), NULL);
      number_var = variable::create (var_float);
    }
  else
    {
      int var_int = strtol (number.c_str (), NULL, 10);
      number_var = variable::create (var_int);
    }
  return number_var;
}

variable*
RADOScript_interpretator::parse_array ()
{
  if (this->script_tokenizer.token ().compare ("[") != 0)
    return NULL;

  // TODO replace variable* with unique_ptr<variable>
  variable::array_t array_vector;
  bool bad_array = false;

  while (this->script_tokenizer.next () != 0)
    {
      variable *tmp  = this->evaluate_expression ();
      if (tmp == NULL)
        {
          bad_array = true;
          break;
        }
      array_vector.push_back (tmp);

      if (this->script_tokenizer.token ().compare (",") != 0)
        break;
    }

  if (this->script_tokenizer.token ().compare ("]") != 0)
    bad_array = true;

  variable *array_var = NULL;
  if (!bad_array)
    {
      array_var = variable::create (array_vector);
    }

  variable::array_t::iterator it = array_vector.begin (),
                              it_e = array_vector.end ();
  for (; it != it_e; ++it)
    {
      delete *it;
    }

  this->script_tokenizer.next ();
  return array_var;
}




variable*
RADOScript_interpretator::evaluate_expression ()
{
  std::stack <variable*> expression_variables;
  std::stack <std::string> expression_signs;
  temp_vars_t temp_vars;
  while (1)
    {
      variable *var = this->parse_operand (temp_vars);
      if (var == NULL)
        {
          return NULL;
        }
      expression_variables.push (var);

      execute_operators (temp_vars, expression_variables, expression_signs, true);

      if (!this->is_expression_sign (script_tokenizer.token ()))
        {
          break;
        }
      expression_signs.push (this->script_tokenizer.token ());
      if (this->script_tokenizer.next () == false)
        {
          return NULL;
        }
    }

  if (!this->script_tokenizer.end () &&
      !this->is_expression_end (this->script_tokenizer.token ()))
    {
      return NULL;
    }
  // we should have n signs and n+1 variables
  if (expression_variables.size () != expression_signs.size () + 1)
    {
      return NULL;
    }

  this->execute_operators (temp_vars, expression_variables,
                           expression_signs, false);
  return variable::create (*expression_variables.top ());
}


variable*
RADOScript_interpretator::parse_operand (temp_vars_t &temp_vars)
{
  variable *return_var = NULL;
  if (this->script_tokenizer.token ().compare ("\"") == 0)
    { // we have string
      return_var = this->parse_string ();
    }
  else if (this->script_tokenizer.token ().compare ("[") == 0)
    { // we have array
      return_var = this->parse_array ();
    }
  else if (isdigit (this->script_tokenizer.token ()[0]) ||
           this->script_tokenizer.token ().compare ("-") == 0)
    { // we have float or int
      return_var = this->parse_number ();
    }
  else
    {
      // we have existing variable
      return_var = *this->parse_variable();
      return return_var;
    }

  temp_vars.push_back (std::unique_ptr<variable>(return_var));
  return return_var;
}


void
RADOScript_interpretator::execute_operators (temp_vars_t &temp_vars,
                               std::stack <variable*> &variables,
                               std::stack <std::string> &operators,
                               bool primary)
{
  while (!operators.empty () &&
         variables.size () >= 2)
    {
      std::string sign = operators.top ();
      if (primary &&
          sign.compare ("*") != 0 &&
          sign.compare ("/") != 0)
        {
          break;
        }
      operators.pop ();
      variable *var2 = variables.top ();
      variables.pop ();
      variable *var1 = variables.top ();
      variables.pop ();

      variable *result = NULL;
      switch (sign.front ())
        {
        case '+':
          result = (*var1) + (*var2);
          break;
        case '-':
          result = (*var1) - (*var2);
          break;
        case '*':
          result = (*var1) * (*var2);
          break;
        case '/':
          result = (*var1) / (*var2);
          break;
        }
      variables.push (result);
      temp_vars.push_back (std::unique_ptr<variable>(result));
    }
}

bool
RADOScript_interpretator::is_expression_end (const std::string &end)
{
  if (end.compare ("\n") == 0 ||
      end.compare (",") == 0 ||
      end.compare ("]") == 0 ||
      end.compare ("=") == 0)
    return true;

  return false;
}

bool
RADOScript_interpretator::is_expression_sign (const std::string &sign)
{
  if (sign.compare ("+") == 0 ||
      sign.compare ("-") == 0 ||
      sign.compare ("/") == 0 ||
      sign.compare ("*") == 0)
    return true;
  return false;
}



