%option c++ prefix="og_scanner_" outfile="og_scanner.cpp"
%option stack noyywrap yylineno 8bit
%{ 
// make relevant includes before including the parser's tab file
#include <string>
#include <cdk/ast/sequence_node.h>
#include <cdk/ast/expression_node.h>
#include <cdk/ast/lvalue_node.h>
#include "og_parser.tab.h"

// don't change this
#define yyerror LexerError
%}
%x X_STRING X_COMMENT X_ASCII IGN_STR X_HEXA
%%

"int"                  return tINT_TYPE;
"real"                 return tREAL_TYPE;
"string"               return tSTRING_TYPE;
"ptr"                  return tPOINTER_TYPE;
"auto"                 return tAUTO_TYPE;

"nullptr"              return tNULLPTR;

"sizeof"               return tSIZEOF;

"public"               return tPUBLIC;
"procedure"            return tPROCEDURE;
"require"              return tREQUIRE;

"if"                   return tIF;
"then"                 return tTHEN;
"elif"                 return tELIF;
"else"                 return tELSE;

">="                   return tGE;
"<="                   return tLE;
"=="                   return tEQ;
"!="                   return tNE;

"&&"                   return tAND;
"||"                   return tOR;

"break"                return tBREAK;
"continue"             return tCONTINUE;
"return"               return tRETURN;

"do"                   return tDO;
"for"                  return tFOR; 
"write"                return tWRITE;
"writeln"              return tWRITELN;
"input"                return tINPUT;

[A-Za-z][A-Za-z0-9_]*  yylval.s = new std::string(yytext); return tIDENTIFIER;

\"                     yy_push_state(X_STRING); yylval.s = new std::string("");
<X_STRING>\"           yy_pop_state(); return tSTRING;


<X_STRING>\\            yy_push_state(X_ASCII);

<X_ASCII>0             *yylval.s += '\0'; yy_pop_state(); yy_push_state(IGN_STR);
<X_ASCII>[A-Fa-f0-9]{1,2} { *yylval.s += (char)strtol(yytext, nullptr, 16); yy_pop_state(); }

<X_ASCII>n             *yylval.s += '\n'; yy_pop_state();
<X_ASCII>r             *yylval.s += '\r'; yy_pop_state();
<X_ASCII>t             *yylval.s += '\t'; yy_pop_state();
<X_ASCII>"\""          *yylval.s += '"';  yy_pop_state();
<X_ASCII>.             *yylval.s += yytext; yy_pop_state();

<X_STRING>.            *yylval.s += yytext;
<X_STRING>\n           yyerror("newline in string");

<IGN_STR>\"            yy_pop_state(); yy_pop_state(); return tSTRING;
<IGN_STR>.             ;

"/*"                   yy_push_state(X_COMMENT);
<X_COMMENT>"*/"        yy_pop_state();
<X_COMMENT>.|\n        ;

"//".*$                ;

[0-9]+                 yylval.i = strtol(yytext, nullptr, 10); return tINTEGER;

"0x"                   yy_push_state(X_HEXA);
<X_HEXA>[A-Fa-f0-9]+   yylval.i = strtol(yytext, nullptr, 16); yy_pop_state(); return tINTEGER;

([0-9]*\.[0-9]+|[0-9]+\.[0-9]*)([Ee]([-+])?[0-9]+)? { yylval.d = strtod(yytext, NULL); return tREAL; }

[-?@~()\[\]<>=+*/%,;{}.]  return *yytext;

[ \t\n]+               ; /* ignore whitespace */

.                      yyerror("Unknown character");

%%
