#include <string>
#include <sstream>
#include "targets/type_checker.h"
#include "targets/postfix_writer.h"
#include "targets/frame_size_calculator.h"
#include "targets/sizeof_calculator.h"
#include "targets/symbol.h"
#include "ast/all.h"  // all.h is automatically generated
#include "og_parser.tab.h"

//---------------------------------------------------------------------------

void og::postfix_writer::do_nil_node(cdk::nil_node * const node, int lvl) {
  // EMPTY
}
void og::postfix_writer::do_data_node(cdk::data_node * const node, int lvl) {
  // EMPTY
}

void og::postfix_writer::do_not_node(cdk::not_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl + 2);
  _pf.INT(0);
  _pf.EQ();
  _pf.NOT();
}

void og::postfix_writer::do_and_node(cdk::and_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl = ++_lbl;
  node->left()->accept(this, lvl + 2);
  _pf.DUP32();
  _pf.JZ(mklbl(lbl));
  node->right()->accept(this, lvl + 2);
  _pf.AND();
  _pf.ALIGN();
  _pf.LABEL(mklbl(lbl));
}

void og::postfix_writer::do_or_node(cdk::or_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl = ++_lbl;
  node->left()->accept(this, lvl + 2);
  _pf.DUP32();
  _pf.JNZ(mklbl(lbl));
  node->right()->accept(this, lvl + 2);
  _pf.OR();
  _pf.ALIGN();
  _pf.LABEL(mklbl(lbl));
}

//---------------------------------------------------------------------------

void og::postfix_writer::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  for (size_t i = 0; i < node->size(); i++) {
    node->node(i)->accept(this, lvl);
  }
}

//---------------------------------------------------------------------------

void og::postfix_writer::do_integer_node(cdk::integer_node * const node, int lvl) {
  if (_inFunctionBody) {
    _pf.INT(node->value()); // integer literal is on the stack: push an integer
  } else {
    _pf.SINT(node->value()); // integer literal is on the DATA segment
  }
}

void og::postfix_writer::do_double_node(cdk::double_node * const node, int lvl) {
  if (_inFunctionBody) {
    _pf.DOUBLE(node->value());
  } else {
    _pf.SDOUBLE(node->value());
  }
}

void og::postfix_writer::do_string_node(cdk::string_node * const node, int lvl) {
  int lbl1;

  /* generate the string */
  _pf.RODATA(); // strings are DATA readonly
  _pf.ALIGN(); // make sure we are aligned
  _pf.LABEL(mklbl(lbl1 = ++_lbl)); // give the string a name
  _pf.SSTRING(node->value()); // output string characters

  if (_function) {
    // local variable initializer
    _pf.TEXT();
    _pf.ADDR(mklbl(lbl1));
  } else {
    // global variable initializer
    _pf.DATA();
    _pf.SADDR(mklbl(lbl1));
  }
}

//---------------------------------------------------------------------------

void og::postfix_writer::do_neg_node(cdk::neg_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl); // determine the value
  _pf.NEG(); // 2-complement
}

//---------------------------------------------------------------------------

void og::postfix_writer::do_add_node(cdk::add_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->left()->accept(this, lvl + 2);
  if (node->is_typed(cdk::TYPE_DOUBLE) && node->left()->is_typed(cdk::TYPE_INT)) {
    _pf.I2D();
  } else if (node->is_typed(cdk::TYPE_POINTER) && node->left()->is_typed(cdk::TYPE_INT)) {
    _pf.INT(3);
    _pf.SHTL();
  }

  node->right()->accept(this, lvl + 2);
  if (node->type()->name() == cdk::TYPE_DOUBLE && node->right()->is_typed(cdk::TYPE_INT)) {
    _pf.I2D();
  } else if (node->is_typed(cdk::TYPE_POINTER) && node->right()->is_typed(cdk::TYPE_INT)) {
    _pf.INT(3);
    _pf.SHTL();
  }

  if (node->is_typed(cdk::TYPE_DOUBLE))
  _pf.DADD();
  else
  _pf.ADD();
}

void og::postfix_writer::do_sub_node(cdk::sub_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->left()->accept(this, lvl + 2);
  if (node->is_typed(cdk::TYPE_DOUBLE) && node->left()->is_typed(cdk::TYPE_INT))
  _pf.I2D();

  node->right()->accept(this, lvl + 2);
  if (node->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_INT)) {
    _pf.I2D();
  } else if (node->is_typed(cdk::TYPE_POINTER) && node->right()->is_typed(cdk::TYPE_INT)) {
    _pf.INT(3);
    _pf.SHTL();
  }

  if (node->is_typed(cdk::TYPE_DOUBLE))
  _pf.DSUB();
  else
  _pf.SUB();
}

void og::postfix_writer::do_mul_node(cdk::mul_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  
  node->left()->accept(this, lvl + 2);
  if (node->is_typed(cdk::TYPE_DOUBLE) && node->left()->is_typed(cdk::TYPE_INT))
  _pf.I2D();

  node->right()->accept(this, lvl + 2);
  if (node->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_INT))
  _pf.I2D();

  if (node->is_typed(cdk::TYPE_DOUBLE))
  _pf.DMUL();
  else
  _pf.MUL();
}

void og::postfix_writer::do_div_node(cdk::div_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  
  node->left()->accept(this, lvl + 2);
  if (node->is_typed(cdk::TYPE_DOUBLE) && node->left()->is_typed(cdk::TYPE_INT))
  _pf.I2D();

  node->right()->accept(this, lvl + 2);
  if (node->is_typed(cdk::TYPE_DOUBLE) && node->right()->is_typed(cdk::TYPE_INT))
  _pf.I2D();

  if (node->is_typed(cdk::TYPE_DOUBLE))
  _pf.DDIV();
  else
  _pf.DIV();
}

void og::postfix_writer::do_mod_node(cdk::mod_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  _pf.MOD();
}

void og::postfix_writer::do_lt_node(cdk::lt_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  
  node->left()->accept(this, lvl + 2);
  if (node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_DOUBLE))
  _pf.I2D();

  node->right()->accept(this, lvl + 2);
  if (node->right()->is_typed(cdk::TYPE_INT) && node->left()->is_typed(cdk::TYPE_DOUBLE))
  _pf.I2D();

  _pf.LT();
}

void og::postfix_writer::do_le_node(cdk::le_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  
  node->left()->accept(this, lvl + 2);
  if (node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_DOUBLE))
  _pf.I2D();

  node->right()->accept(this, lvl + 2);
  if (node->right()->is_typed(cdk::TYPE_INT) && node->left()->is_typed(cdk::TYPE_DOUBLE))
  _pf.I2D();

  _pf.LE();
}
void og::postfix_writer::do_ge_node(cdk::ge_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->left()->accept(this, lvl + 2);
  if (node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_DOUBLE))
  _pf.I2D();

  node->right()->accept(this, lvl + 2);
  if (node->right()->is_typed(cdk::TYPE_INT) && node->left()->is_typed(cdk::TYPE_DOUBLE))
  _pf.I2D();

  _pf.GE();
}
void og::postfix_writer::do_gt_node(cdk::gt_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->left()->accept(this, lvl + 2);
  if (node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_DOUBLE))
  _pf.I2D();

  node->right()->accept(this, lvl + 2);
  if (node->right()->is_typed(cdk::TYPE_INT) && node->left()->is_typed(cdk::TYPE_DOUBLE))
  _pf.I2D();

  _pf.GT();
}
void og::postfix_writer::do_ne_node(cdk::ne_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->left()->accept(this, lvl + 2);
  if (node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_DOUBLE))
  _pf.I2D();

  node->right()->accept(this, lvl + 2);
  if (node->right()->is_typed(cdk::TYPE_INT) && node->left()->is_typed(cdk::TYPE_DOUBLE))
  _pf.I2D();

  _pf.NE();
}
void og::postfix_writer::do_eq_node(cdk::eq_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->left()->accept(this, lvl + 2);
  if (node->left()->is_typed(cdk::TYPE_INT) && node->right()->is_typed(cdk::TYPE_DOUBLE))
  _pf.I2D();

  node->right()->accept(this, lvl + 2);
  if (node->right()->is_typed(cdk::TYPE_INT) && node->left()->is_typed(cdk::TYPE_DOUBLE))
  _pf.I2D();

  _pf.EQ();
}

//---------------------------------------------------------------------------

void og::postfix_writer::do_variable_node(cdk::variable_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  const std::string &id = node->name();
  std::shared_ptr<og::symbol> symbol = _symtab.find(id);
  if (symbol->global()) {
    _pf.ADDR(symbol->name());
  }
  else {
    _pf.LOCAL(symbol->offset());
  }
}

void og::postfix_writer::do_rvalue_node(cdk::rvalue_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->lvalue()->accept(this, lvl);
  if (node->type()->name() == cdk::TYPE_DOUBLE) {
    _pf.LDDOUBLE();
  }
  else {
    _pf.LDINT();
  }
}

void og::postfix_writer::do_assignment_node(cdk::assignment_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  node->rvalue()->accept(this, lvl + 2);
  if (node->is_typed(cdk::TYPE_DOUBLE)) {
    if (node->rvalue()->is_typed(cdk::TYPE_INT))
    _pf.I2D();
    _pf.DUP64();
  } else {
    _pf.DUP32();
  }

  node->lvalue()->accept(this, lvl);
  if (node->is_typed(cdk::TYPE_DOUBLE)) {
    _pf.STDOUBLE();
  } else {
    _pf.STINT();
  }

}

//---------------------------------------------------------------------------

void og::postfix_writer::do_evaluation_node(og::evaluation_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl); // determine the value
  if (node->argument()->is_typed(cdk::TYPE_INT)) {
    _pf.TRASH(4); // delete the evaluated value
  } else if (node->argument()->is_typed(cdk::TYPE_DOUBLE)) {
    _pf.TRASH(8); // delete the evaluated value's address
  } else if (node->argument()->is_typed(cdk::TYPE_STRING)) {
    _pf.TRASH(4); // delete the evaluated value's address
  } else if (node->argument()->is_typed(cdk::TYPE_POINTER)) {
    _pf.TRASH(4); // delete the evaluated value's address
  } else if (node->argument()->is_typed(cdk::TYPE_STRUCT)) {
    _pf.TRASH(((og::tuple_node*) node->argument())->type()->size()); // delete the evaluated value's address
  } else if (node->argument()->is_typed(cdk::TYPE_VOID)) {
    // nothing to evaluate, procedure
  } else {
    std::cerr << "ERROR: CANNOT HAPPEN!" << std::endl;
    exit(1);
  }
}

//---------------------------------------------------------------------------

void og::postfix_writer::do_for_node(og::for_node * const node, int lvl) {
  // ASSERT_SAFE_EXPRESSIONS;

  _forIni.push(++_lbl);
  _forStep.push(++_lbl);
  _forEnd.push(++_lbl);

  _inForInit = true;

  if (node->inits())
    node->inits()->accept(this, lvl + 2);

  _pf.ALIGN();
  _pf.LABEL(mklbl(_forIni.top()));

  node->condition()->accept(this, lvl);

  _pf.JZ(mklbl(_forEnd.top()));

  node->block()->accept(this, lvl + 2);

  _pf.ALIGN();
  _pf.LABEL(mklbl(_forStep.top()));
  node->incrs()->accept(this, lvl + 2);

  _pf.JMP(mklbl(_forIni.top()));

  _pf.ALIGN();
  _pf.LABEL(mklbl(_forEnd.top()));

  _inForInit = false;

  _forIni.pop();
  _forStep.pop();
  _forEnd.pop();
}

//---------------------------------------------------------------------------

void og::postfix_writer::do_if_node(og::if_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl1;
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl1 = ++_lbl));
  node->block()->accept(this, lvl + 2);
  _pf.LABEL(mklbl(lbl1));
}

//---------------------------------------------------------------------------

void og::postfix_writer::do_if_else_node(og::if_else_node * const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  int lbl1, lbl2;
  node->condition()->accept(this, lvl);
  _pf.JZ(mklbl(lbl1 = ++_lbl));
  node->thenblock()->accept(this, lvl + 2);
  _pf.JMP(mklbl(lbl2 = ++_lbl));
  _pf.LABEL(mklbl(lbl1));
  node->elseblock()->accept(this, lvl + 2);
  _pf.LABEL(mklbl(lbl1 = lbl2));
}

//---------------------------------------------------------------------------

void og::postfix_writer::do_input_node(og::input_node *const node, int lvl) {
  if (_lvalueType->name() == cdk::TYPE_DOUBLE) {
    _functions_to_declare.insert("readd");
    _pf.CALL("readd");
    _pf.LDFVAL64();
  } else if (_lvalueType->name() == cdk::TYPE_INT) {
    _functions_to_declare.insert("readi");
    _pf.CALL("readi");
    _pf.LDFVAL32();
  } else {
    std::cerr << "FATAL: " << node->lineno() << ": cannot read type" << std::endl;
    return;
  }
}

//---------------------------------------------------------------------------

void og::postfix_writer::do_break_node(og::break_node *const node, int lvl) {
  if (_forIni.size() != 0) {
    _pf.JMP(mklbl(_forEnd.top())); // jump to for end
  } else
    error(node->lineno(), "'break' outside 'for'");
}

//---------------------------------------------------------------------------

void og::postfix_writer::do_return_node(og::return_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  if (_function->type()->name() != cdk::TYPE_VOID) {
    node->retvals()->accept(this, lvl + 2);

    if (_function->type()->name() == cdk::TYPE_INT || _function->type()->name() == cdk::TYPE_STRING
        || _function->type()->name() == cdk::TYPE_POINTER) {
      _pf.STFVAL32();
    } else if (_function->type()->name() == cdk::TYPE_DOUBLE) {
      
      // FIX THIS for multiple retvals

      if (((cdk::expression_node*) node->retvals()->node(0))->type()->name() == cdk::TYPE_INT)
        _pf.I2D();
      _pf.STFVAL64();
    } else {
      std::cerr << node->lineno() << ": should not happen: unknown return type" << std::endl;
    }
  }

  _returnSeen = true;

  _pf.LEAVE();
  _pf.RET();

}

//---------------------------------------------------------------------------

void og::postfix_writer::do_left_index_node(og::left_index_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  if (node->base()) {
    node->base()->accept(this, lvl);
  } else {
    if (_function) {
      _pf.LOCV(-_function->type()->size());
    } else {
      std::cerr << "FATAL: " << node->lineno() << ": trying to use return value outside function" << std::endl;
    }
  }
  node->index()->accept(this, lvl);
  _pf.INT(3);
  _pf.SHTL();
  _pf.ADD(); // add pointer and index
}

void og::postfix_writer::do_tuple_index_node(og::tuple_index_node *const node, int lvl) {
  // EMPTY
}


//---------------------------------------------------------------------------

void og::postfix_writer::do_stack_alloc_node(og::stack_alloc_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->argument()->accept(this, lvl);
  _pf.INT(3);
  _pf.SHTL();
  _pf.ALLOC(); // allocate
  _pf.SP();// put base pointer in stack
}

//---------------------------------------------------------------------------

void og::postfix_writer::do_variable_declaration_node(og::variable_declaration_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  int offset = 0, typesize = node->type()->size();
  if (_inFunctionBody) {
    _offset -= typesize;
    offset = _offset;
  } 
  else if (_inFunctionArgs) {
    offset = _offset;
    _offset += typesize;
  } 
  else {
    offset = 0; // global variable
  }

  std::shared_ptr<og::symbol> symbol = new_symbol();
  if (symbol) {
    symbol->set_offset(offset);
    reset_new_symbol();
  }

  if (node->identifiers()->size() == 1) {

    auto id = ((cdk::variable_node *) node->identifiers()->node(0))->name();

    if (_inFunctionBody) {
      if (node->initializer()) {
        node->initializer()->accept(this, lvl);

        // if right exprs size is one
        if (node->initializer()->is_typed(cdk::TYPE_STRUCT) && ((og::tuple_node*) node->initializer())->sequence()->size() == 1) {
          cdk::expression_node* expr = dynamic_cast<cdk::expression_node*>(((og::tuple_node*) node->initializer())->sequence()->node(0));
          if (expr->is_typed(cdk::TYPE_DOUBLE)) {
            _pf.LOCAL(symbol->offset());
            _pf.STDOUBLE();
          } else {
            _pf.LOCAL(symbol->offset());
            _pf.STINT();
          }
        }
        else if (node->type()->name() == cdk::TYPE_INT || node->type()->name() == cdk::TYPE_STRING || node->type()->name() == cdk::TYPE_POINTER) {
          _pf.LOCAL(symbol->offset());
          _pf.STINT();
        } else if (node->type()->name() == cdk::TYPE_DOUBLE) {
          _pf.LOCAL(symbol->offset());
          _pf.STDOUBLE();
        } else if (node->type()->name() == cdk::TYPE_STRUCT) {
          int tuple_offset = 0;
          for (size_t i = ((og::tuple_node*) node)->sequence()->size() - 1; i > 0; i--) {
            cdk::expression_node* expr = dynamic_cast<cdk::expression_node*>(((og::tuple_node*) node)->sequence()->node(i));
            if (expr->is_typed(cdk::TYPE_INT) || expr->is_typed(cdk::TYPE_STRING) || expr->is_typed(cdk::TYPE_POINTER)) {
              _pf.LOCAL(symbol->offset() - tuple_offset);
              _pf.STINT();
              tuple_offset += 4;
            } else if (expr->is_typed(cdk::TYPE_DOUBLE)) {
              _pf.LOCAL(symbol->offset() - tuple_offset);
              _pf.STDOUBLE();
              tuple_offset += 8;
            } else {
              std::cerr << "cannot initialize tuple" << std::endl;
            }
          }
        } else {
          std::cerr << "cannot initialize" << std::endl;
        }
      }
    } 
    else {
      if (!_function) {

        if (node->initializer() == nullptr) {
          _pf.BSS();
          _pf.ALIGN();
          _pf.LABEL(id);
          _pf.SALLOC(typesize);
        }
        else {
          if (node->type()->name() == cdk::TYPE_INT || node->type()->name() == cdk::TYPE_DOUBLE || node->type()->name() == cdk::TYPE_POINTER) {
            _pf.DATA();
            _pf.ALIGN();
            if (node->qualifier() == tPUBLIC) _pf.GLOBAL(id, _pf.OBJ());
            _pf.LABEL(id);

            // if right exprs size is one
            if (node->initializer()->is_typed(cdk::TYPE_STRUCT) && ((og::tuple_node*) node->initializer())->sequence()->size() == 1) {
              ((og::tuple_node*) node->initializer())->sequence()->node(0)->accept(this, lvl);
            }
            else if (node->type()->name() == cdk::TYPE_INT) {
              node->initializer()->accept(this, lvl);
            } 
            else if (node->type()->name() == cdk::TYPE_POINTER) {
              node->initializer()->accept(this, lvl);
            } 
            else if (node->type()->name() == cdk::TYPE_DOUBLE) {
              if (node->initializer()->type()->name() == cdk::TYPE_DOUBLE) {
                node->initializer()->accept(this, lvl);
              } 
              else if (node->initializer()->is_typed(cdk::TYPE_INT)) {
                cdk::integer_node *dclini = dynamic_cast<cdk::integer_node *>(node->initializer());
                cdk::double_node ddi(dclini->lineno(), dclini->value());
                ddi.accept(this, lvl);
              } 
              else {
                std::cerr << node->lineno() << ": '" << id << "' has bad initializer for real value\n";
                _errors = true;
              }
            }
          } 
          else if (node->type()->name() == cdk::TYPE_STRING) {
              _pf.DATA();
              _pf.ALIGN();
              _pf.LABEL(id);
              if (node->qualifier() == tPUBLIC) _pf.GLOBAL(id, _pf.OBJ());
              node->initializer()->accept(this, lvl);
          } 
          else {
            std::cerr << node->lineno() << ": '" << id << "' has unexpected initializer\n";
            _errors = true;
          }
        }
      }
    }

  } else if (node->identifiers()->size() > 1 && node->initializer()) { // multiple identifiers

    if (node->initializer() && node->initializer()->is_typed(cdk::TYPE_STRUCT)
        && ((og::tuple_node*) node->initializer())->sequence()->size() == node->identifiers()->size()) {

      for (size_t i = 0; i < node->identifiers()->size(); i++) {
        // cdk::expression_node* init = dynamic_cast<cdk::expression_node*>(((og::tuple_node*) node->initializer())->sequence()->node(i));
        
        // const std::string &id = ((cdk::variable_node*) node->identifiers()->node(i))->name();

        if (_inFunctionBody) {

        }
        else {

        }

      }
    }

  } else {
    std::cerr << node->lineno() << ": '" << "Bad identifiers\n";
  }
}

//---------------------------------------------------------------------------

void og::postfix_writer::do_continue_node(og::continue_node *const node, int lvl) {
  if (_forIni.size() != 0) {
    _pf.JMP(mklbl(_forStep.top()));
  } else
    error(node->lineno(), "'restart' outside 'for'");
}

//---------------------------------------------------------------------------

void og::postfix_writer::do_write_node(og::write_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  for (size_t i = 0; i < node->expressions()->size(); i++) {
    cdk::expression_node* expr_node = (cdk::expression_node*) node->expressions()->node(i);

    expr_node->accept(this, lvl + 2);

    if (expr_node->type()->name() == cdk::TYPE_INT || expr_node->type()->name() == cdk::TYPE_POINTER) {
      _functions_to_declare.insert("printi");
      _pf.CALL("printi");
      _pf.TRASH(4);
    } else if (expr_node->type()->name() == cdk::TYPE_DOUBLE) {
      _functions_to_declare.insert("printd");
      _pf.CALL("printd");
      _pf.TRASH(8);
    } else if (expr_node->type()->name() == cdk::TYPE_STRING) {
      _functions_to_declare.insert("prints");
      _pf.CALL("prints");
      _pf.TRASH(4);
    } else {
      std::cerr << "cannot print expression of unknown type" << std::endl;
      return;
    }
  }

  if (node->newline()) {
    _functions_to_declare.insert("println");
    _pf.CALL("println");
  }
}

//---------------------------------------------------------------------------

void og::postfix_writer::do_sizeof_node(og::sizeof_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  sizeof_calculator soc(_compiler, _symtab);
  node->accept(&soc, lvl);

  if (_inFunctionBody) {
    _pf.INT(soc.localsize()); // integer literal is on the stack: push an integer
  } else {
    _pf.SINT(soc.localsize()); // integer literal is on the DATA segment
  }
}

//---------------------------------------------------------------------------

void og::postfix_writer::do_address_node(og::address_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;
  node->lvalue()->accept(this, lvl + 2);
}

//---------------------------------------------------------------------------

void og::postfix_writer::do_block_node(og::block_node *const node, int lvl) {
  _symtab.push();
  if (node->declarations()) node->declarations()->accept(this, lvl + 2);
  if (node->instructions()) node->instructions()->accept(this, lvl + 2);
  _symtab.pop();
}

//---------------------------------------------------------------------------

void og::postfix_writer::do_function_call_node(og::function_call_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  std::shared_ptr<og::symbol> symbol = _symtab.find(node->identifier());

  size_t argsSize = 0;
  if (node->arguments()) {
    for (int ax = node->arguments()->size(); ax > 0; ax--) {
      cdk::expression_node *arg = dynamic_cast<cdk::expression_node*>(node->arguments()->node(ax - 1));
      arg->accept(this, lvl + 2);
      if (arg->type() && arg->type()->name() == cdk::TYPE_INT && symbol->arg_type(ax - 1)->name() == cdk::TYPE_DOUBLE)
        _pf.I2D();
      argsSize += arg->type()->size();
    }
  }
  _pf.CALL(node->identifier());
  if (argsSize != 0) {
    _pf.TRASH(argsSize);
  }

  if (symbol->type()) { // if not a procedure
    if (symbol->is_typed(cdk::TYPE_INT) || symbol->is_typed(cdk::TYPE_POINTER) || symbol->is_typed(cdk::TYPE_STRING)) {
      _pf.LDFVAL32();
    }
    else if (symbol->is_typed(cdk::TYPE_DOUBLE)) {
      _pf.LDFVAL64();
    }
  }
}

void og::postfix_writer::do_function_definition_node(og::function_definition_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  _function = new_symbol();
  _functions_to_declare.erase(_function->name());
  reset_new_symbol();

  _offset = 8;
  _symtab.push();
  if (node->arguments()) {
    _inFunctionArgs = true;
    for (size_t i = 0; i < node->arguments()->size(); i++) {
      if (node->arguments()->node(i) == nullptr) break;
      node->arguments()->node(i)->accept(this, 0);
    }
    _inFunctionArgs = false;
  }

  _pf.TEXT();
  _pf.ALIGN();
  if (node->qualifier() == tPUBLIC) _pf.GLOBAL(_function->name(), _pf.FUNC());
  _pf.LABEL(_function->name());

  frame_size_calculator lsc(_compiler, _symtab);
  node->accept(&lsc, lvl);
  _pf.ENTER(lsc.localsize());

  _inFunctionBody = true;
  _offset = -_function->type()->size();

  node->block()->accept(this, lvl + 2);

  _inFunctionBody = false;
  _symtab.pop();

  if (!_returnSeen) {
    _pf.LEAVE();
    _pf.RET();
  }

  if (node->identifier() == "og") {
    for (std::string s : _functions_to_declare)
      _pf.EXTERN(s);
  }

}

void og::postfix_writer::do_function_header_node(og::function_header_node *const node, int lvl) {
  ASSERT_SAFE_EXPRESSIONS;

  if (!new_symbol()) return;

  std::shared_ptr<og::symbol> function = new_symbol();
  _functions_to_declare.insert(function->name());
  reset_new_symbol();
}

//---------------------------------------------------------------------------

void og::postfix_writer::do_nullptr_node(og::nullptr_node *const node, int lvl) {
  // EMPTY
}

//---------------------------------------------------------------------------

void og::postfix_writer::do_identity_node(og::identity_node *const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
}

//---------------------------------------------------------------------------

void og::postfix_writer::do_tuple_node(og::tuple_node *const node, int lvl) {
  node->sequence()->accept(this, lvl + 2);
}
