%{
//-- don't change *any* of these: if you do, you'll break the compiler.
#include <cdk/compiler.h>
#include "ast/all.h"
#define LINE               compiler->scanner()->lineno()
#define yylex()            compiler->scanner()->scan()
#define yyerror(s)         compiler->scanner()->error(s)
#define YYPARSE_PARAM_TYPE std::shared_ptr<cdk::compiler>
#define YYPARSE_PARAM      compiler
//-- don't change *any* of these --- END!
%}

%union {
  int                   i;  /* integer value */
  double                d;  /* double value */
  std::string          *s;  /* symbol name or string literal */
  cdk::basic_node      *node; /* node pointer */
  cdk::sequence_node   *sequence;
  cdk::expression_node *expression; /* expression nodes */
  cdk::lvalue_node     *lvalue;

  cdk::basic_type *type;       /* expression type */
  cdk::reference_type *ref_type;
  og::block_node       *block;
};

%token <i> tINTEGER tPUBLIC tPRIVATE tREQUIRE
%token <d> tREAL
%token <s> tIDENTIFIER tSTRING
%token tNULLPTR

%token tINT_TYPE tREAL_TYPE tSTRING_TYPE tPOINTER_TYPE tAUTO_TYPE tPOINTER
%token tRETURN tCONTINUE tBREAK tDO tFOR tWRITE tWRITELN
%token tSIZEOF

%token tPROCEDURE

%token tIF tTHEN tELIF tELSE

%token tFOR tWRITE tINPUT tDO

%nonassoc tIF tFOR
%nonassoc tTHEN tDO
%nonassoc tELIF tELSE

%right '='
%left tOR 
%left tAND
%nonassoc '~'
%left tEQ tNE
%left tGE tLE '>' '<'
%left '+' '-'
%left '*' '/' '%'
%nonassoc tUNARY
%nonassoc '[' ']'
$nonassoc '(' ')'

%type<sequence> file declarations
%type<node> declaration var_decl func_header func_def arg_decl bvar_decl
%type<type> data_type int real string auto
%type<ref_type> ptr ptr_reps
%type<block> block
%type<sequence> block_decls block_intrs args exprs id_list arg_decls
%type<node> inst cond_inst elif_set loop_inst
%type<s> str_list

/*%type <node> stmt inst*/
/*%type <sequence> list exprs vars*/
%type <expression> expr
%type <lvalue> lval

%{
//-- The rules below will be included in yyparse, the main parsing function.
%}
%%

file          : /* empty */  { compiler->ast($$ = new cdk::sequence_node(LINE)); }
              | declarations { compiler->ast($$ = $1); }
              ;

declarations  : declaration { $$ = new cdk::sequence_node(LINE, $1); }
              | declarations declaration { $$ = new cdk::sequence_node(LINE, $2, $1); }
              ;

declaration   : var_decl     { $$ = $1; }
              | func_header  { $$ = $1; }
              | func_def     { $$ = $1; }
              ;

var_decl      : tPUBLIC  data_type tIDENTIFIER           ';' { $$ = new og::variable_declaration_node(LINE,  tPUBLIC, std::make_shared<cdk::primitive_type>($2->size(), $2->name()), *$3, nullptr); delete $3; }
              | tREQUIRE data_type tIDENTIFIER           ';' { $$ = new og::variable_declaration_node(LINE, tREQUIRE, std::make_shared<cdk::primitive_type>($2->size(), $2->name()), *$3, nullptr); delete $3; }
              |          data_type tIDENTIFIER           ';' { $$ = new og::variable_declaration_node(LINE, tPRIVATE, std::make_shared<cdk::primitive_type>($1->size(), $1->name()), *$2, nullptr); delete $2; }
              | tPUBLIC  ptr_reps  tIDENTIFIER           ';' { $$ = new og::variable_declaration_node(LINE,  tPUBLIC, std::make_shared<cdk::reference_type>($2->size(), $2->referenced()), *$3, nullptr); delete $3; }
              | tREQUIRE ptr_reps  tIDENTIFIER           ';' { $$ = new og::variable_declaration_node(LINE, tREQUIRE, std::make_shared<cdk::reference_type>($2->size(), $2->referenced()), *$3, nullptr); delete $3; }
              |          ptr_reps  tIDENTIFIER           ';' { $$ = new og::variable_declaration_node(LINE, tPRIVATE, std::make_shared<cdk::reference_type>($1->size(), $1->referenced()), *$2, nullptr); delete $2; }
              | tPUBLIC  data_type tIDENTIFIER '=' expr  ';' { $$ = new og::variable_declaration_node(LINE,  tPUBLIC, std::make_shared<cdk::primitive_type>($2->size(), $2->name()), *$3, $5); delete $3; }
              | tREQUIRE data_type tIDENTIFIER '=' expr  ';' { $$ = new og::variable_declaration_node(LINE, tREQUIRE, std::make_shared<cdk::primitive_type>($2->size(), $2->name()), *$3, $5); delete $3; }
              |          data_type tIDENTIFIER '=' expr  ';' { $$ = new og::variable_declaration_node(LINE, tPRIVATE, std::make_shared<cdk::primitive_type>($1->size(), $1->name()), *$2, $4); delete $2; }
              | tPUBLIC  ptr_reps  tIDENTIFIER '=' expr  ';' { $$ = new og::variable_declaration_node(LINE,  tPUBLIC, std::make_shared<cdk::reference_type>($2->size(), $2->referenced()), *$3, $5); delete $3; }
              | tREQUIRE ptr_reps  tIDENTIFIER '=' expr  ';' { $$ = new og::variable_declaration_node(LINE, tREQUIRE, std::make_shared<cdk::reference_type>($2->size(), $2->referenced()), *$3, $5); delete $3; }
              |          ptr_reps  tIDENTIFIER '=' expr  ';' { $$ = new og::variable_declaration_node(LINE, tPRIVATE, std::make_shared<cdk::reference_type>($1->size(), $1->referenced()), *$2, $4); delete $2; }
              | tPUBLIC  auto      id_list     '=' exprs ';' { $$ = new og::variable_declaration_node(LINE,  tPUBLIC, std::make_shared<cdk::primitive_type>($2->size(), $2->name()), $3, new og::tuple_node(LINE, $5)); }
              | tREQUIRE auto      id_list     '=' exprs ';' { $$ = new og::variable_declaration_node(LINE, tREQUIRE, std::make_shared<cdk::primitive_type>($2->size(), $2->name()), $3, new og::tuple_node(LINE, $5)); }
              |          auto      id_list     '=' exprs ';' { $$ = new og::variable_declaration_node(LINE, tPRIVATE, std::make_shared<cdk::primitive_type>($1->size(), $1->name()), $2, new og::tuple_node(LINE, $4)); }
              ;

func_header   : tPUBLIC  data_type  tIDENTIFIER '('      ')' { $$ = new og::function_header_node(LINE,  tPUBLIC, std::make_shared<cdk::primitive_type>($2->size(), $2->name()), *$3, nullptr); delete $3; }
              | tREQUIRE data_type  tIDENTIFIER '('      ')' { $$ = new og::function_header_node(LINE, tREQUIRE, std::make_shared<cdk::primitive_type>($2->size(), $2->name()), *$3, nullptr); delete $3; }
              |          data_type  tIDENTIFIER '('      ')' { $$ = new og::function_header_node(LINE, tPRIVATE, std::make_shared<cdk::primitive_type>($1->size(), $1->name()), *$2, nullptr); delete $2; }
              | tPUBLIC  data_type  tIDENTIFIER '(' args ')' { $$ = new og::function_header_node(LINE,  tPUBLIC, std::make_shared<cdk::primitive_type>($2->size(), $2->name()), *$3,      $5); delete $3; }
              | tREQUIRE data_type  tIDENTIFIER '(' args ')' { $$ = new og::function_header_node(LINE, tREQUIRE, std::make_shared<cdk::primitive_type>($2->size(), $2->name()), *$3,      $5); delete $3; }
              |          data_type  tIDENTIFIER '(' args ')' { $$ = new og::function_header_node(LINE, tPRIVATE, std::make_shared<cdk::primitive_type>($1->size(), $1->name()), *$2,      $4); delete $2; }
              | tPUBLIC  ptr_reps   tIDENTIFIER '('      ')' { $$ = new og::function_header_node(LINE,  tPUBLIC, std::make_shared<cdk::reference_type>($2->size(), $2->referenced()), *$3, nullptr); delete $3; }
              | tREQUIRE ptr_reps   tIDENTIFIER '('      ')' { $$ = new og::function_header_node(LINE, tREQUIRE, std::make_shared<cdk::reference_type>($2->size(), $2->referenced()), *$3, nullptr); delete $3; }
              |          ptr_reps   tIDENTIFIER '('      ')' { $$ = new og::function_header_node(LINE, tPRIVATE, std::make_shared<cdk::reference_type>($1->size(), $1->referenced()), *$2, nullptr); delete $2; }
              | tPUBLIC  ptr_reps   tIDENTIFIER '(' args ')' { $$ = new og::function_header_node(LINE,  tPUBLIC, std::make_shared<cdk::reference_type>($2->size(), $2->referenced()), *$3,      $5); delete $3; }
              | tREQUIRE ptr_reps   tIDENTIFIER '(' args ')' { $$ = new og::function_header_node(LINE, tREQUIRE, std::make_shared<cdk::reference_type>($2->size(), $2->referenced()), *$3,      $5); delete $3; }
              |          ptr_reps   tIDENTIFIER '(' args ')' { $$ = new og::function_header_node(LINE, tPRIVATE, std::make_shared<cdk::reference_type>($1->size(), $1->referenced()), *$2,      $4); delete $2; }
              | tPUBLIC  auto       tIDENTIFIER '('      ')' { $$ = new og::function_header_node(LINE,  tPUBLIC, std::make_shared<cdk::primitive_type>($2->size(), $2->name()), *$3, nullptr); delete $3; }
              | tREQUIRE auto       tIDENTIFIER '('      ')' { $$ = new og::function_header_node(LINE, tREQUIRE, std::make_shared<cdk::primitive_type>($2->size(), $2->name()), *$3, nullptr); delete $3; }
              |          auto       tIDENTIFIER '('      ')' { $$ = new og::function_header_node(LINE, tPRIVATE, std::make_shared<cdk::primitive_type>($1->size(), $1->name()), *$2, nullptr); delete $2; }
              | tPUBLIC  auto       tIDENTIFIER '(' args ')' { $$ = new og::function_header_node(LINE,  tPUBLIC, std::make_shared<cdk::primitive_type>($2->size(), $2->name()), *$3,      $5); delete $3; }
              | tREQUIRE auto       tIDENTIFIER '(' args ')' { $$ = new og::function_header_node(LINE, tREQUIRE, std::make_shared<cdk::primitive_type>($2->size(), $2->name()), *$3,      $5); delete $3; }
              |          auto       tIDENTIFIER '(' args ')' { $$ = new og::function_header_node(LINE, tPRIVATE, std::make_shared<cdk::primitive_type>($1->size(), $1->name()), *$2,      $4); delete $2; }
              | tPUBLIC  tPROCEDURE tIDENTIFIER '('      ')' { $$ = new og::function_header_node(LINE,  tPUBLIC, std::make_shared<cdk::primitive_type>(0, cdk::TYPE_VOID),      *$3, nullptr); delete $3; }
              | tREQUIRE tPROCEDURE tIDENTIFIER '('      ')' { $$ = new og::function_header_node(LINE, tREQUIRE, std::make_shared<cdk::primitive_type>(0, cdk::TYPE_VOID),      *$3, nullptr); delete $3; }
              |          tPROCEDURE tIDENTIFIER '('      ')' { $$ = new og::function_header_node(LINE, tPRIVATE, std::make_shared<cdk::primitive_type>(0, cdk::TYPE_VOID),      *$2, nullptr); delete $2; }
              | tPUBLIC  tPROCEDURE tIDENTIFIER '(' args ')' { $$ = new og::function_header_node(LINE,  tPUBLIC, std::make_shared<cdk::primitive_type>(0, cdk::TYPE_VOID),      *$3,      $5); delete $3; }
              | tREQUIRE tPROCEDURE tIDENTIFIER '(' args ')' { $$ = new og::function_header_node(LINE, tREQUIRE, std::make_shared<cdk::primitive_type>(0, cdk::TYPE_VOID),      *$3,      $5); delete $3; }
              |          tPROCEDURE tIDENTIFIER '(' args ')' { $$ = new og::function_header_node(LINE, tPRIVATE, std::make_shared<cdk::primitive_type>(0, cdk::TYPE_VOID),      *$2,      $4); delete $2; }
              ;

func_def      : tPUBLIC  data_type  tIDENTIFIER '('      ')' block { $$ = new og::function_definition_node(LINE,  tPUBLIC, std::make_shared<cdk::primitive_type>($2->size(), $2->name()), *$3, nullptr, $6); delete $3; }
              | tREQUIRE data_type  tIDENTIFIER '('      ')' block { $$ = new og::function_definition_node(LINE, tREQUIRE, std::make_shared<cdk::primitive_type>($2->size(), $2->name()), *$3, nullptr, $6); delete $3; }
              |          data_type  tIDENTIFIER '('      ')' block { $$ = new og::function_definition_node(LINE, tPRIVATE, std::make_shared<cdk::primitive_type>($1->size(), $1->name()), *$2, nullptr, $5); delete $2; }
              | tPUBLIC  data_type  tIDENTIFIER '(' args ')' block { $$ = new og::function_definition_node(LINE,  tPUBLIC, std::make_shared<cdk::primitive_type>($2->size(), $2->name()), *$3,      $5, $7); delete $3; }
              | tREQUIRE data_type  tIDENTIFIER '(' args ')' block { $$ = new og::function_definition_node(LINE, tREQUIRE, std::make_shared<cdk::primitive_type>($2->size(), $2->name()), *$3,      $5, $7); delete $3; }
              |          data_type  tIDENTIFIER '(' args ')' block { $$ = new og::function_definition_node(LINE, tPRIVATE, std::make_shared<cdk::primitive_type>($1->size(), $1->name()), *$2,      $4, $6); delete $2; }
              | tPUBLIC  ptr_reps   tIDENTIFIER '('      ')' block { $$ = new og::function_definition_node(LINE,  tPUBLIC, std::make_shared<cdk::reference_type>($2->size(), $2->referenced()), *$3, nullptr, $6); delete $3; }
              | tREQUIRE ptr_reps   tIDENTIFIER '('      ')' block { $$ = new og::function_definition_node(LINE, tREQUIRE, std::make_shared<cdk::reference_type>($2->size(), $2->referenced()), *$3, nullptr, $6); delete $3; }
              |          ptr_reps   tIDENTIFIER '('      ')' block { $$ = new og::function_definition_node(LINE, tPRIVATE, std::make_shared<cdk::reference_type>($1->size(), $1->referenced()), *$2, nullptr, $5); delete $2; }
              | tPUBLIC  ptr_reps   tIDENTIFIER '(' args ')' block { $$ = new og::function_definition_node(LINE,  tPUBLIC, std::make_shared<cdk::reference_type>($2->size(), $2->referenced()), *$3,      $5, $7); delete $3; }
              | tREQUIRE ptr_reps   tIDENTIFIER '(' args ')' block { $$ = new og::function_definition_node(LINE, tREQUIRE, std::make_shared<cdk::reference_type>($2->size(), $2->referenced()), *$3,      $5, $7); delete $3; }
              |          ptr_reps   tIDENTIFIER '(' args ')' block { $$ = new og::function_definition_node(LINE, tPRIVATE, std::make_shared<cdk::reference_type>($1->size(), $1->referenced()), *$2,      $4, $6); delete $2; }
              | tPUBLIC  auto       tIDENTIFIER '('      ')' block { $$ = new og::function_definition_node(LINE,  tPUBLIC, std::make_shared<cdk::primitive_type>($2->size(), $2->name()), *$3, nullptr, $6); delete $3; }
              | tREQUIRE auto       tIDENTIFIER '('      ')' block { $$ = new og::function_definition_node(LINE, tREQUIRE, std::make_shared<cdk::primitive_type>($2->size(), $2->name()), *$3, nullptr, $6); delete $3; }
              |          auto       tIDENTIFIER '('      ')' block { $$ = new og::function_definition_node(LINE, tPRIVATE, std::make_shared<cdk::primitive_type>($1->size(), $1->name()), *$2, nullptr, $5); delete $2; }
              | tPUBLIC  auto       tIDENTIFIER '(' args ')' block { $$ = new og::function_definition_node(LINE,  tPUBLIC, std::make_shared<cdk::primitive_type>($2->size(), $2->name()), *$3,      $5, $7); delete $3; }
              | tREQUIRE auto       tIDENTIFIER '(' args ')' block { $$ = new og::function_definition_node(LINE, tREQUIRE, std::make_shared<cdk::primitive_type>($2->size(), $2->name()), *$3,      $5, $7); delete $3; }
              |          auto       tIDENTIFIER '(' args ')' block { $$ = new og::function_definition_node(LINE, tPRIVATE, std::make_shared<cdk::primitive_type>($1->size(), $1->name()), *$2,      $4, $6); delete $2; }
              | tPUBLIC  tPROCEDURE tIDENTIFIER '('      ')' block { $$ = new og::function_definition_node(LINE,  tPUBLIC, std::make_shared<cdk::primitive_type>(0, cdk::TYPE_VOID), *$3, nullptr, $6); delete $3; }
              | tREQUIRE tPROCEDURE tIDENTIFIER '('      ')' block { $$ = new og::function_definition_node(LINE, tREQUIRE, std::make_shared<cdk::primitive_type>(0, cdk::TYPE_VOID), *$3, nullptr, $6); delete $3; }
              |          tPROCEDURE tIDENTIFIER '('      ')' block { $$ = new og::function_definition_node(LINE, tPRIVATE, std::make_shared<cdk::primitive_type>(0, cdk::TYPE_VOID), *$2, nullptr, $5); delete $2; }
              | tPUBLIC  tPROCEDURE tIDENTIFIER '(' args ')' block { $$ = new og::function_definition_node(LINE,  tPUBLIC, std::make_shared<cdk::primitive_type>(0, cdk::TYPE_VOID), *$3,      $5, $7); delete $3; }
              | tREQUIRE tPROCEDURE tIDENTIFIER '(' args ')' block { $$ = new og::function_definition_node(LINE, tREQUIRE, std::make_shared<cdk::primitive_type>(0, cdk::TYPE_VOID), *$3,      $5, $7); delete $3; }
              |          tPROCEDURE tIDENTIFIER '(' args ')' block { $$ = new og::function_definition_node(LINE, tPRIVATE, std::make_shared<cdk::primitive_type>(0, cdk::TYPE_VOID), *$2,      $4, $6); delete $2; }
              ;

block         : '{'                         '}' { $$ = new og::block_node(LINE, nullptr, nullptr); }
              | '{' block_decls             '}' { $$ = new og::block_node(LINE,      $2, nullptr); }
              | '{' block_decls block_intrs '}' { $$ = new og::block_node(LINE,      $2,      $3); }
              | '{'             block_intrs '}' { $$ = new og::block_node(LINE, nullptr,      $2); }
              ;

arg_decl      : data_type tIDENTIFIER           { $$ = new og::variable_declaration_node(LINE, tPRIVATE, std::make_shared<cdk::primitive_type>($1->size(), $1->name()),       *$2, nullptr); delete $2; }
              | ptr_reps  tIDENTIFIER           { $$ = new og::variable_declaration_node(LINE, tPRIVATE, std::make_shared<cdk::reference_type>($1->size(), $1->referenced()), *$2, nullptr); delete $2; }
              | data_type tIDENTIFIER '=' expr  { $$ = new og::variable_declaration_node(LINE, tPRIVATE, std::make_shared<cdk::primitive_type>($1->size(), $1->name()),       *$2,      $4); delete $2; }
              | ptr_reps  tIDENTIFIER '=' expr  { $$ = new og::variable_declaration_node(LINE, tPRIVATE, std::make_shared<cdk::reference_type>($1->size(), $1->referenced()), *$2,      $4); delete $2; }
              ;

bvar_decl     : arg_decl               { $$ = $1; }
              | auto id_list '=' exprs { $$ = new og::variable_declaration_node(LINE, tPRIVATE, std::make_shared<cdk::primitive_type>($1->size(), $1->name()), $2, new og::tuple_node(LINE, $4)); }
              ;

block_decls   :             bvar_decl ';' { $$ = new cdk::sequence_node(LINE, $1);     }
              | block_decls bvar_decl ';' { $$ = new cdk::sequence_node(LINE, $2, $1); }
              ;

block_intrs   :             inst { $$ = new cdk::sequence_node(LINE, $1);     }
              | block_intrs inst { $$ = new cdk::sequence_node(LINE, $2, $1); }
              ;

data_type     : int         { $$ = $1; }
              | real        { $$ = $1; }
              | string      { $$ = $1; }
              ;

ptr_reps      :                   ptr          { $$ = $1; }
              | tPOINTER_TYPE '<' ptr_reps '>' { $$ = new cdk::reference_type(4, std::make_shared<cdk::reference_type>($3->size(), $3->referenced())); }
              ;

int           : tINT_TYPE                    { $$ = new cdk::primitive_type(4, cdk::TYPE_INT);     } ;
real          : tREAL_TYPE                   { $$ = new cdk::primitive_type(8, cdk::TYPE_DOUBLE);  } ;
string        : tSTRING_TYPE                 { $$ = new cdk::primitive_type(4, cdk::TYPE_STRING);  } ;
auto          : tAUTO_TYPE                   { $$ = new cdk::primitive_type(4, cdk::TYPE_UNSPEC);  } ;
ptr           : tPOINTER_TYPE '<' int    '>' { $$ = new cdk::reference_type(4, std::make_shared<cdk::primitive_type>($3->size(), $3->name())); }
              | tPOINTER_TYPE '<' real   '>' { $$ = new cdk::reference_type(4, std::make_shared<cdk::primitive_type>($3->size(), $3->name())); }
              | tPOINTER_TYPE '<' string '>' { $$ = new cdk::reference_type(4, std::make_shared<cdk::primitive_type>($3->size(), $3->name())); }
              | tPOINTER_TYPE '<' auto   '>' { $$ = new cdk::reference_type(4, std::make_shared<cdk::primitive_type>($3->size(), $3->name())); }
              ;

arg_decls : arg_decl               { $$ = new cdk::sequence_node(LINE, $1);     }
          | arg_decls ',' arg_decl { $$ = new cdk::sequence_node(LINE, $3, $1); }
          ;

args      : arg_decls              { $$ = $1; }
          | auto id_list '=' exprs { $$ = new cdk::sequence_node(LINE, new og::variable_declaration_node(LINE, tPRIVATE, std::make_shared<cdk::primitive_type>($1->size(), $1->name()), $2, new og::tuple_node(LINE, $4))); }
          ;

exprs     : expr           { $$ = new cdk::sequence_node(LINE, $1);     }
          | exprs ',' expr { $$ = new cdk::sequence_node(LINE, $3, $1); }
          ;

expr      : tINTEGER                  { $$ = new cdk::integer_node(LINE, $1); }
          | tREAL                     { $$ = new cdk::double_node(LINE, $1);  }
          | tNULLPTR                  { $$ = new og::nullptr_node(LINE);      }
          | str_list                  { $$ = new cdk::string_node(LINE, $1);  }
          | '-' expr  %prec tUNARY    { $$ = new cdk::neg_node(LINE, $2);     }
          | '+' expr  %prec tUNARY    { $$ = new og::identity_node(LINE, $2); }
          | expr '+' expr             { $$ = new cdk::add_node(LINE, $1, $3); }
          | expr '-' expr             { $$ = new cdk::sub_node(LINE, $1, $3); }
          | expr '*' expr             { $$ = new cdk::mul_node(LINE, $1, $3); }
          | expr '/' expr             { $$ = new cdk::div_node(LINE, $1, $3); }
          | expr '%' expr             { $$ = new cdk::mod_node(LINE, $1, $3); }
          | expr '<' expr             { $$ = new cdk::lt_node(LINE, $1, $3);  }
          | expr '>' expr             { $$ = new cdk::gt_node(LINE, $1, $3);  }
          | expr tGE expr             { $$ = new cdk::ge_node(LINE, $1, $3);  }
          | expr tLE expr             { $$ = new cdk::le_node(LINE, $1, $3);  }
          | expr tOR expr             { $$ = new cdk::or_node(LINE, $1, $3);  }
          | expr tAND expr            { $$ = new cdk::and_node(LINE, $1, $3); }
          | expr tNE expr             { $$ = new cdk::ne_node(LINE, $1, $3);  }
          | expr tEQ expr             { $$ = new cdk::eq_node(LINE, $1, $3);  }
          | '(' expr ')'              { $$ = $2; }
          | lval                      { $$ = new cdk::rvalue_node(LINE, $1); }
          | lval '?' %prec tUNARY     { $$ = new og::address_node(LINE, $1); }
          | lval '=' expr             { $$ = new cdk::assignment_node(LINE, $1, $3); }
          | tIDENTIFIER '('       ')' { $$ = new og::function_call_node(LINE, *$1);     delete $1; }
          | tIDENTIFIER '(' exprs ')' { $$ = new og::function_call_node(LINE, *$1, $3); delete $1; }
          | tSIZEOF     '(' exprs ')' { $$ = new og::sizeof_node(LINE, $3); }               
          | '[' expr ']'              { $$ = new og::stack_alloc_node(LINE, $2); }
          ;

str_list  :          tSTRING { $$ = $1; }
          | str_list tSTRING { $$ = new std::string(*$1 + *$2); delete $1; delete $2; }
          ;

id_list   :             tIDENTIFIER { $$ = new cdk::sequence_node(LINE, new cdk::variable_node(LINE, *$1));     delete $1; }
          | id_list ',' tIDENTIFIER { $$ = new cdk::sequence_node(LINE, new cdk::variable_node(LINE, *$3), $1); delete $3; }
          ;

lval      : tIDENTIFIER               { $$ = new cdk::variable_node(LINE, $1); delete $1; }
          | expr '[' expr ']'         { $$ = new og::left_index_node(LINE, $1, $3);  }
          | tIDENTIFIER '@' tINTEGER  { $$ = new og::tuple_index_node(LINE, *$1, $3); delete $1; }
          ;

inst      : expr              ';'     { $$ = new og::evaluation_node(LINE, $1); }
          | tWRITE exprs      ';'     { $$ = new og::write_node(LINE, $2, false); }
          | tWRITELN  exprs   ';'     { $$ = new og::write_node(LINE, $2, true); }
          | tBREAK                    { $$ = new og::break_node(LINE); }
          | tCONTINUE                 { $$ = new og::continue_node(LINE); }
          | tRETURN           ';'     { $$ = new og::return_node(LINE, nullptr); }
          | tRETURN exprs     ';'     { $$ = new og::return_node(LINE, $2); }
          | cond_inst                 { $$ = $1; }
          | loop_inst                 { $$ = $1; }
          | block                     { $$ = $1; }
          ;

cond_inst : tIF expr tTHEN inst                      { $$ = new og::if_node(LINE, $2, $4); }
          | tIF expr tTHEN inst tELSE inst           { $$ = new og::if_else_node(LINE, $2, $4, $6); }
          | tIF expr tTHEN inst elif_set             { $$ = new og::if_else_node(LINE, $2, $4, $5); }
          ;

elif_set  :          tELIF expr tTHEN inst            { $$ = new og::if_node(LINE, $2, $4); }
          |          tELIF expr tTHEN inst tELSE inst { $$ = new og::if_else_node(LINE, $2, $4, $6); }
          | elif_set tELIF expr tTHEN inst            { $$ = new og::if_node(LINE, $3, $5); }
          | elif_set tELIF expr tTHEN inst tELSE inst { $$ = new og::if_else_node(LINE, $3, $5, $7); }
          ;

loop_inst : tFOR       ';' exprs ';' exprs tDO inst { $$ = new og::for_node(LINE, nullptr,      $3,      $5, $7); }
          | tFOR       ';'       ';' exprs tDO inst { $$ = new og::for_node(LINE, nullptr, nullptr,      $4, $6); }
          | tFOR       ';' exprs ';'       tDO inst { $$ = new og::for_node(LINE, nullptr,      $3, nullptr, $6); }
          | tFOR       ';'       ';'       tDO inst { $$ = new og::for_node(LINE, nullptr, nullptr, nullptr, $5); }
          | tFOR args  ';' exprs ';' exprs tDO inst { $$ = new og::for_node(LINE,      $2,      $4,      $6, $8); }
          | tFOR args  ';'       ';' exprs tDO inst { $$ = new og::for_node(LINE,      $2, nullptr,      $5, $7); }
          | tFOR args  ';' exprs ';'       tDO inst { $$ = new og::for_node(LINE,      $2,      $4, nullptr, $7); }
          | tFOR args  ';'       ';'       tDO inst { $$ = new og::for_node(LINE,      $2, nullptr, nullptr, $6); }
          | tFOR exprs ';' exprs ';' exprs tDO inst { $$ = new og::for_node(LINE,      $2,      $4,      $6, $8); }
          | tFOR exprs ';'       ';' exprs tDO inst { $$ = new og::for_node(LINE,      $2, nullptr,      $5, $7); }
          | tFOR exprs ';' exprs ';'       tDO inst { $$ = new og::for_node(LINE,      $2,      $4, nullptr, $7); }
          | tFOR exprs ';'       ';'       tDO inst { $$ = new og::for_node(LINE,      $2, nullptr, nullptr, $6); }
          ;

%%
