#ifndef CONVERT_SCRIPT_TO_NUMBER_H
#define CONVERT_SCRIPT_TO_NUMBER_H


#include "exprtk.hpp"



double Script_to_Number(double x, std::string param_expression){


    typedef exprtk::symbol_table<double> symbol_table_t;
    typedef exprtk::expression<double>     expression_t;
    typedef exprtk::parser<double>             parser_t;

   symbol_table_t symbol_table;
   symbol_table.add_variable("x",x);
   
   expression_t expression;
   expression.register_symbol_table(symbol_table);

   parser_t parser;
   parser.compile(param_expression.c_str(),expression);
   
   
   return expression.value();
};

double Script_to_Number(double x ,double y , std::string param_expression){


    typedef exprtk::symbol_table<double> symbol_table_t;
    typedef exprtk::expression<double>     expression_t;
    typedef exprtk::parser<double>             parser_t;

   symbol_table_t symbol_table;
   symbol_table.add_variable("x",x);
   symbol_table.add_variable("y",y);
   
   expression_t expression;
   expression.register_symbol_table(symbol_table);

   parser_t parser;
   parser.compile(param_expression.c_str(),expression);
   
   
   return expression.value();
};




double Script_to_Number(double x ,double y , double z , std::string param_expression){


    typedef exprtk::symbol_table<double> symbol_table_t;
    typedef exprtk::expression<double>     expression_t;
    typedef exprtk::parser<double>             parser_t;

   symbol_table_t symbol_table;
   symbol_table.add_variable("x",x);
   symbol_table.add_variable("y",y);
   symbol_table.add_variable("z",z);
   
   
   expression_t expression;
   expression.register_symbol_table(symbol_table);

   parser_t parser;
   parser.compile(param_expression.c_str(),expression);
   
   
   return expression.value();
};


/////////////  To_String Function  //////////////////////////
template<typename T>
std::string Number_to_string(T i)
{
    std::stringstream ss;
    ss << i;
    return ss.str();
}
////////////////////////////////////////////////////////////

#endif // CONVERT_SCRIPT_TO_NUMBER_H
