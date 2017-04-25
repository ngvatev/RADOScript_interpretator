#ifndef RADO_SCRIPT_H_
#define RADO_SCRIPT_H_

#include <string>
#include <map>
#include <vector>
#include <stack>
#include <memory>


#include "variable.h"
#include "tokenizer.h"


class RADOScript_interpretator
{

private: //data
  typedef std::vector<std::unique_ptr<variable>> temp_vars_t;
  // =#\n+-*/*[],."
  static const std::string script_delimiters;
  tokenizer script_tokenizer;

  // stores all the variables declared in the script here
  std::map<std::string, variable*> variables;
private: // methods

  // parses RADOScript script
  bool parse_text (const std::string &text);

  // print the next expression in the script
  bool print_expression ();

  // create a variable and assign value to it
  bool variable_assignment ();

  // returns variable** where variable* is null if the variable
  // doesnt exist and a new variable with that name is created
  variable** parse_variable ();

  // returns new variable index that needs to be deleted
  variable* parse_index ();

  // creates new string_variable needs to be deleted
  variable* parse_string ();

  // creates new int/float_variable needs to be deleted
  variable* parse_number ();

  // creates new array_varaible needs to be deleted
  variable* parse_array ();

  // evaluates expression and returns variable
  // that needs to be deleted
  variable* evaluate_expression ();

  // parses an operand (string, array, number ... )
  variable* parse_operand (temp_vars_t &temp_vars);

  // gets the top 2 variables and executes the top operator and palces
  // the result back in the stack
  // it is repeated until operators is empty or we have less than 2 variables
  // if primary is true executes only the top primary operators
  void execute_operators (temp_vars_t &temp_vars,
                          std::stack <variable*> &variables,
                          std::stack <std::string> &operators,
                          bool primary);

  bool is_expression_end (const std::string &end);
  bool is_expression_sign (const std::string &sign);





  /*+++++++++++++++++++++++++++++++++++++++++++++++++++*/
  bool execute_operators (std::vector<variable> &vals,
                          std::vector<char> &operators,
                          variable &rv);

  bool execute_numbers (std::vector<variable> &vals,
                        std::vector<char> operators,
                        variable &rv);



public:
  RADOScript_interpretator ();
  bool execute (const std::string &file_name);
  void clear ();
  ~RADOScript_interpretator ();
};



#endif /* RADO_SCRIPT_H_ */
