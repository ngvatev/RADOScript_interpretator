#include <iostream>
#include <fstream>
#include "RADOScriptinterpretator.h"

static const char* test_file_name = "RADOScript_testing_script";

static struct
{
  std::string script_name;
  std::string script;
} test_scripts[] =
    { // scripts that are expected to work
     // assignments
     {
      "string assignment",
      "string_variable=\"some_string\"\n"
      "#string_variable"
     },
     {
      "float assignment",
      "float_variable=3.5\n"
      "#float_variable"
     },
     {
      "int assignment",
      "int_variable = 5\n"
      "#int_variable"
     },
     {
      "array assignment",
      "array_variable=[1,2.3,4,\"pet\",[6,7.2,\"osem\"]]\n"
      "#array_variable"
     },

     // expression evaluation
     {
       "number operators",
       "#5*2.2+1+6.2/2"
     },
     {
      "string operator +",
      "first_part=\"asd\"+\"fgh\"\n"
      "second_part=\"jkl\"\n"
      "#first_part+second_part"
     },
     {
      "array operator +",
      "array1=[1,2,3]\n"
      "array2=array1+[4,5,6]\n"
      "#array2\n"
      "#array2-1\n"
     },
     {
      "array operator -",
      "array=[1,2,3]\n"
      "array3=array-1\n"
      "#array3"
     },
     {
      "array operator []",
      "array4=[1,2,3]\n"
      "#array4[1]\n"
      },
      {
       "array operator [] assignment",
       "array5=[1,2,3]\n"
       "array5[1]=21\n"
       "#array5[1]\n"
      },
      // crash tests
      {
       "division by 0",
       "#6/0"
      },
      {
       "illegal operator test",
       "#\"asd\"+3"
      },
   };


bool write_script (const std::string &script,
                   const std::string &file_name)
{
  std::ofstream test_file (file_name,
                           std::ofstream::out | std::ofstream::trunc);

  if (!test_file.is_open ())
    {
      std::cout<<"problem: can not open file\n";
      return false;
    }

  test_file.write (script.c_str (), script.size ());
  test_file.close ();
  return true;
}

void test ()
{
  RADOScript_interpretator interpretator;
  size_t tests_size = sizeof (test_scripts)/sizeof (test_scripts[0]);

  for (size_t i = 0; i < tests_size; ++i)
    {
      std::cout<<"========test "<<i<<" :"
          <<test_scripts[i].script_name<<"========\n";


      if (write_script (test_scripts[i].script, test_file_name) == false)
        continue;

      bool success = false;
      try
        {
          // TODO redirect stdout and check if the results is correct
          success = interpretator.execute (test_file_name);
        }
      catch (std::runtime_error &e)
        {
          std::cout<<e.what ()<<std::endl;
        }
      if (success)
        std::cout<<"ok\n";
      else
        std::cout<<"failed\n";
      interpretator.clear ();
    }
}


int main ()
{
  test ();
  return 0;
}
