#include <string>
#include "targets/type_checker.h"
#include "ast/all.h"  // automatically generated
#include <cdk/types/primitive_type.h>
#include "targets/sizeof_calculator.h"
#include "og_parser.tab.h"

#define ASSERT_UNSPEC { if (node->type() != nullptr && !node->is_typed(cdk::TYPE_UNSPEC)) return; }

//---------------------------------------------------------------------------

void og::type_checker::do_sequence_node(cdk::sequence_node *const node, int lvl) {
  for (size_t i = 0; i < node->size(); i++)
    node->node(i)->accept(this, lvl + 2);
}

//---------------------------------------------------------------------------

void og::type_checker::do_nil_node(cdk::nil_node *const node, int lvl) {
  // EMPTY
}
void og::type_checker::do_data_node(cdk::data_node *const node, int lvl) {
  // EMPTY
}

void og::type_checker::do_not_node(cdk::not_node *const node, int lvl) {
  processUnaryExpression(node, lvl);
}
void og::type_checker::do_and_node(cdk::and_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void og::type_checker::do_or_node(cdk::or_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}

//---------------------------------------------------------------------------

void og::type_checker::do_integer_node(cdk::integer_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(cdk::make_primitive_type(4, cdk::TYPE_INT));
}

void og::type_checker::do_double_node(cdk::double_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(cdk::make_primitive_type(8, cdk::TYPE_DOUBLE));
}

void og::type_checker::do_string_node(cdk::string_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(cdk::make_primitive_type(4, cdk::TYPE_STRING));
}

//---------------------------------------------------------------------------

void og::type_checker::processUnaryExpression(cdk::unary_operation_node *const node, int lvl) {
  node->argument()->accept(this, lvl + 2);

  if (node->argument()->is_typed(cdk::TYPE_INT))
    node->type(cdk::make_primitive_type(4, cdk::TYPE_INT));
  else if (node->argument()->is_typed(cdk::TYPE_DOUBLE))
    node->type(cdk::make_primitive_type(8, cdk::TYPE_DOUBLE));
  else throw std::string("wrong type in argument of unary expression");

}

void og::type_checker::do_neg_node(cdk::neg_node *const node, int lvl) {
  processUnaryExpression(node, lvl);
}

//---------------------------------------------------------------------------

void og::type_checker::processBinaryExpression(cdk::binary_operation_node *const node, int lvl) {
  ASSERT_UNSPEC;

  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);

  if (node->left()->type()->name() == cdk::TYPE_DOUBLE && node->right()->type()->name() == cdk::TYPE_DOUBLE)
    node->type(cdk::make_primitive_type(8, cdk::TYPE_DOUBLE));
  else if (node->left()->type()->name() == cdk::TYPE_DOUBLE && node->right()->type()->name() == cdk::TYPE_INT)
    node->type(cdk::make_primitive_type(8, cdk::TYPE_DOUBLE));
  else if (node->left()->type()->name() == cdk::TYPE_INT && node->right()->type()->name() == cdk::TYPE_DOUBLE)
    node->type(cdk::make_primitive_type(8, cdk::TYPE_DOUBLE));
  else if (node->left()->type()->name() == cdk::TYPE_POINTER && node->right()->type()->name() == cdk::TYPE_INT)
    node->type(node->left()->type());
  else if (node->left()->type()->name() == cdk::TYPE_INT && node->right()->type()->name() == cdk::TYPE_POINTER)
    node->type(node->right()->type());
  else if (node->left()->type()->name() == cdk::TYPE_INT && node->right()->type()->name() == cdk::TYPE_INT)
    node->type(cdk::make_primitive_type(4, cdk::TYPE_INT));
  else if (node->left()->type()->name() == cdk::TYPE_UNSPEC && node->right()->type()->name() == cdk::TYPE_UNSPEC) {
  // Auto types, currently just casting to int
    node->type(cdk::make_primitive_type(4, cdk::TYPE_INT));
    node->left()->type(cdk::make_primitive_type(4, cdk::TYPE_INT));
    node->right()->type(cdk::make_primitive_type(4, cdk::TYPE_INT));
  }
  else throw std::string("wrong types in binary expression");

}

void og::type_checker::do_add_node(cdk::add_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void og::type_checker::do_sub_node(cdk::sub_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void og::type_checker::do_mul_node(cdk::mul_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void og::type_checker::do_div_node(cdk::div_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void og::type_checker::do_mod_node(cdk::mod_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void og::type_checker::do_lt_node(cdk::lt_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void og::type_checker::do_le_node(cdk::le_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void og::type_checker::do_ge_node(cdk::ge_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void og::type_checker::do_gt_node(cdk::gt_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void og::type_checker::do_ne_node(cdk::ne_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void og::type_checker::do_eq_node(cdk::eq_node *const node, int lvl) {
  processBinaryExpression(node, lvl);
}

//---------------------------------------------------------------------------

void og::type_checker::do_variable_node(cdk::variable_node *const node, int lvl) {
  ASSERT_UNSPEC;
  const std::string &id = node->name();
  std::shared_ptr<og::symbol> symbol = _symtab.find(id);

  if (symbol != nullptr) {
    node->type(symbol->type());
  } else {
    throw id;
  }
}

void og::type_checker::do_rvalue_node(cdk::rvalue_node *const node, int lvl) {
  ASSERT_UNSPEC;

  try {
    node->lvalue()->accept(this, lvl);
    node->type(node->lvalue()->type());
  } catch (const std::string &id) {
    throw "undeclared variable '" + id + "'";
  }
}

void og::type_checker::do_assignment_node(cdk::assignment_node *const node, int lvl) {
  ASSERT_UNSPEC;

  node->lvalue()->accept(this, lvl + 2);
  node->rvalue()->accept(this, lvl + 2);

  if (node->lvalue()->is_typed(cdk::TYPE_INT)) {
    if (node->rvalue()->is_typed(cdk::TYPE_INT)) {
      node->type(cdk::make_primitive_type(4, cdk::TYPE_INT));
    } else if (node->rvalue()->is_typed(cdk::TYPE_UNSPEC)) {
      node->type(cdk::make_primitive_type(4, cdk::TYPE_INT));
      node->rvalue()->type(cdk::make_primitive_type(4, cdk::TYPE_INT));
    } else
      throw std::string("wrong assignment to integer");
  }

  else if (node->lvalue()->is_typed(cdk::TYPE_POINTER)) {

    if (node->rvalue()->is_typed(cdk::TYPE_POINTER)) {
      node->type(node->lvalue()->type());
    } else if (node->rvalue()->is_typed(cdk::TYPE_INT)) {
      node->type(cdk::make_primitive_type(4, cdk::TYPE_POINTER));
    } else if (node->rvalue()->is_typed(cdk::TYPE_UNSPEC)) {
      node->type(cdk::make_primitive_type(4, cdk::TYPE_ERROR));
      node->rvalue()->type(cdk::make_primitive_type(4, cdk::TYPE_ERROR));
    } else
      throw std::string("wrong assignment to pointer");
  }

  else if (node->lvalue()->is_typed(cdk::TYPE_DOUBLE)) {
    if (node->rvalue()->is_typed(cdk::TYPE_DOUBLE)|| node->rvalue()->is_typed(cdk::TYPE_INT)) {
      node->type(cdk::make_primitive_type(8, cdk::TYPE_DOUBLE));
    } else if (node->rvalue()->is_typed(cdk::TYPE_UNSPEC)) {
      node->type(cdk::make_primitive_type(8, cdk::TYPE_DOUBLE));
      node->rvalue()->type(cdk::make_primitive_type(8, cdk::TYPE_DOUBLE));
    } else
      throw std::string("wrong assignment to real");
  }

  else if (node->lvalue()->is_typed(cdk::TYPE_STRING)) {
    if (node->rvalue()->is_typed(cdk::TYPE_STRING)) {
      node->type(cdk::make_primitive_type(4, cdk::TYPE_STRING));
    } else if (node->rvalue()->is_typed(cdk::TYPE_UNSPEC)) {
      node->type(cdk::make_primitive_type(4, cdk::TYPE_STRING));
      node->rvalue()->type(cdk::make_primitive_type(4, cdk::TYPE_STRING));
    } else
      throw std::string("wrong assignment to string");
  }

  else {
    throw std::string("wrong types in assignment");
  }
}

//---------------------------------------------------------------------------

void og::type_checker::do_evaluation_node(og::evaluation_node *const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
}

//---------------------------------------------------------------------------

void og::type_checker::do_for_node(og::for_node *const node, int lvl) {

  if (node->inits())
    node->inits()->accept(this, lvl + 2);

  node->condition()->accept(this, lvl + 2);

  node->incrs()->accept(this, lvl + 2);
}

//---------------------------------------------------------------------------

void og::type_checker::do_if_node(og::if_node *const node, int lvl) {
  node->condition()->accept(this, lvl + 2);
}

void og::type_checker::do_if_else_node(og::if_else_node *const node, int lvl) {
  node->condition()->accept(this, lvl + 2);
}

//---------------------------------------------------------------------------

void og::type_checker::do_input_node(og::input_node *const node, int lvl) {
  node->type(cdk::make_primitive_type(0, cdk::TYPE_UNSPEC));
}

//---------------------------------------------------------------------------

void og::type_checker::do_break_node(og::break_node *const node, int lvl) {
  // EMPTY: NOTHING TODO
}

//---------------------------------------------------------------------------

void og::type_checker::do_return_node(og::return_node *const node, int lvl) {
  node->retvals()->accept(this, lvl + 2);
} 

//---------------------------------------------------------------------------

void og::type_checker::do_left_index_node(og::left_index_node *const node, int lvl) {
  ASSERT_UNSPEC;

  if (node->base()) {
    node->base()->accept(this, lvl + 2);
    if (!node->base()->is_typed(cdk::TYPE_POINTER)) {
      throw std::string("pointer expression expected in index left-value");
    }
  }

  node->index()->accept(this, lvl + 2);
  if (!node->index()->is_typed(cdk::TYPE_INT)) {
    throw std::string("integer expression expected in left-value index");
  }

  std::shared_ptr<cdk::reference_type> ptr = reference_type_cast(node->base()->type());

  if (ptr->referenced()->name() == cdk::TYPE_INT) {
    node->type(cdk::make_primitive_type(4, cdk::TYPE_INT));
  }
  else if (ptr->referenced()->name() == cdk::TYPE_DOUBLE) {
    node->type(cdk::make_primitive_type(8, cdk::TYPE_DOUBLE));
  }
  else if (ptr->referenced()->name() == cdk::TYPE_STRING) {
    node->type(cdk::make_primitive_type(4, cdk::TYPE_STRING));
  }
  else if (ptr->referenced()->name() == cdk::TYPE_POINTER) {
    node->type(ptr->referenced());
  }
  else if (ptr->referenced()->name() == cdk::TYPE_STRUCT) {
    node->type(cdk::make_primitive_type(ptr->referenced()->size(), cdk::TYPE_STRUCT));
  }
  else if (ptr->referenced()->name() == cdk::TYPE_VOID) {
    node->type(cdk::make_primitive_type(4, cdk::TYPE_VOID));
  }

}

void og::type_checker::do_tuple_index_node(og::tuple_index_node *const node, int lvl) {
  // EMPTY
}


//---------------------------------------------------------------------------

void og::type_checker::do_stack_alloc_node(og::stack_alloc_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->argument()->accept(this, lvl + 2);
  if (node->argument()->type()->name() != cdk::TYPE_INT) {
    throw std::string(
      "integer expression expected in allocation expression");
  }
  node->type(make_reference_type(4, make_primitive_type(8, cdk::TYPE_UNSPEC)));
}

//---------------------------------------------------------------------------

void og::type_checker::do_variable_declaration_node(og::variable_declaration_node *const node, int lvl) {

  if (node->identifiers()->size() == 1) {
    if (node->initializer()) {
      node->initializer()->accept(this, lvl + 2);
      if (node->type()->name() == cdk::TYPE_INT) {
        if (!node->initializer()->is_typed(cdk::TYPE_INT)) throw std::string(
            "wrong type for initializer (integer expected).");
      } else if (node->type()->name() == cdk::TYPE_DOUBLE) {
        if (!node->initializer()->is_typed(cdk::TYPE_INT)
            && !node->initializer()->is_typed(cdk::TYPE_DOUBLE)) throw std::string(
            "wrong type for initializer (integer or double expected).");
      } else if (node->type()->name() == cdk::TYPE_STRING) {
        if (!node->initializer()->is_typed(cdk::TYPE_STRING)) throw std::string(
            "wrong type for initializer (string expected).");
      } else if (node->type()->name() == cdk::TYPE_POINTER) {
        if (!node->initializer()->is_typed(cdk::TYPE_POINTER)
            && !node->initializer()->is_typed(cdk::TYPE_INT)) throw std::string(
            "wrong type for initializer (pointer or integer expected).");
      } else if (node->type()->name() == cdk::TYPE_STRUCT) {
        if (!node->initializer()->is_typed(cdk::TYPE_STRUCT)) throw std::string(
            "wrong type for initializer (tuple expected).");
      } else if (node->type()->name() == cdk::TYPE_UNSPEC) {
        if (node->initializer()) {
          cdk::sequence_node* seq = dynamic_cast<cdk::sequence_node*>(((og::tuple_node*) node->initializer())->sequence());
          if (seq->size() == 1) {
            node->type(((cdk::expression_node*)seq->node(0))->type());
          } else {
            node->type(node->initializer()->type());
          }
        }
      } else {
        throw std::string("unknown type for initializer.");
      }
    }

    const std::string &id = ((cdk::variable_node*) node->identifiers()->node(0))->name();

    std::shared_ptr<og::symbol> symbol = std::make_shared<og::symbol>(
      node->type(), id, 0, node->qualifier(), (bool)node->initializer(), false);

    if (_symtab.insert(id, symbol)) {
      _parent->set_new_symbol(symbol);
    } else {
      throw std::string("variable '" + id + "' redeclared");
    }

  } else if (node->identifiers()->size() > 1) {

    if (node->initializer() && node->initializer()->is_typed(cdk::TYPE_STRUCT)
        && ((og::tuple_node*) node->initializer())->sequence()->size() == node->identifiers()->size()) {

      node->type(cdk::make_primitive_type(4, cdk::TYPE_UNSPEC));
      for (size_t i = 0; i < node->identifiers()->size(); i++) {
        cdk::expression_node* init = dynamic_cast<cdk::expression_node*>(((og::tuple_node*) node->initializer())->sequence()->node(i));
        const std::string &id = ((cdk::variable_node*) node->identifiers()->node(i))->name();

        std::shared_ptr<og::variable_declaration_node> var_decl_node = std::make_shared<og::variable_declaration_node>(
          node->lineno(), node->qualifier(), cdk::make_primitive_type(4, cdk::TYPE_UNSPEC), id, init);

        var_decl_node->accept(this, lvl);

      }

    } else {
      throw std::string("Invalid Initializer.");
    }

  } else {
    throw std::string("Invalid Identifiers.");
  }

}

//---------------------------------------------------------------------------

void og::type_checker::do_continue_node(og::continue_node *const node, int lvl) {
  // EMPTY: NOTHING TODO
}

//---------------------------------------------------------------------------

void og::type_checker::do_write_node(og::write_node *const node, int lvl) {
  node->expressions()->accept(this, lvl + 2);
  for (size_t i = 0; i < node->expressions()->size(); i++)
    if (((cdk::expression_node*) node->expressions()->node(i))->type()->name() == cdk::TYPE_VOID)
      throw std::string("wrong type in print argument");
}

//---------------------------------------------------------------------------

void og::type_checker::do_sizeof_node(og::sizeof_node *const node, int lvl) {
  node->type(cdk::make_primitive_type(4, cdk::TYPE_INT));
  node->expressions()->accept(this, lvl + 2);
}

//---------------------------------------------------------------------------

void og::type_checker::do_address_node(og::address_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->lvalue()->accept(this, lvl + 2);
  if (node->lvalue()->type()->name() == cdk::TYPE_INT)
    node->type(cdk::make_reference_type(4, std::make_shared<cdk::primitive_type>(4, cdk::TYPE_INT)));
  else if (node->lvalue()->type()->name() == cdk::TYPE_DOUBLE)
    node->type(cdk::make_reference_type(4, std::make_shared<cdk::primitive_type>(8, cdk::TYPE_DOUBLE)));
  else if (node->lvalue()->type()->name() == cdk::TYPE_STRING)
    node->type(cdk::make_reference_type(4, std::make_shared<cdk::primitive_type>(4, cdk::TYPE_STRING)));
  else if (node->lvalue()->type()->name() == cdk::TYPE_POINTER)
    node->type(cdk::make_reference_type(4, node->lvalue()->type()));
  else if (node->lvalue()->type()->name() == cdk::TYPE_STRUCT)
    node->type(cdk::make_reference_type(4, node->lvalue()->type()));
  else throw std::string("wrong type in unary logical expression of addressing");
}

//---------------------------------------------------------------------------

void og::type_checker::do_block_node(og::block_node *const node, int lvl) {
  // EMPTY
}

//---------------------------------------------------------------------------

void og::type_checker::do_function_call_node(og::function_call_node *const node, int lvl) {
  ASSERT_UNSPEC;
  const std::string &id = node->identifier();
  std::shared_ptr<og::symbol> symbol = _symtab.find(id);

  if (symbol == nullptr) throw std::string("symbol '" + id + "' is undeclared.");

  if (!symbol->isFunction()) throw std::string("symbol '" + id + "' is not a function.");

  node->type(symbol->type());

  if (node->arguments()) {
    node->arguments()->accept(this, lvl + 2);
  }

}

void og::type_checker::do_function_definition_node(og::function_definition_node *const node, int lvl) {
  std::string id;

  if (node->identifier() == "og")
    id = "_main";
  else if (node->identifier() == "_main")
    id = "._main";
  else
    id = node->identifier();

  std::shared_ptr<og::symbol> function = std::make_shared<og::symbol>(
    node->type(), id, 0, node->qualifier(), false, true);

  if (node->arguments()) {
    for (size_t i = 0; i < node->arguments()->size(); i++)
      function->add_arg_type(((og::variable_declaration_node*) node->arguments()->node(i))->type());
  }

  std::shared_ptr<og::symbol> previous = _symtab.find(function->name());
  
  if (previous) {
    if (previous->forward()
        && ((previous->qualifier() == tPUBLIC && node->qualifier() == tPUBLIC)
            || (previous->qualifier() == tPRIVATE && node->qualifier() == tPRIVATE))) {
      _symtab.replace(function->name(), function);
      _parent->set_new_symbol(function);
    } else {
      throw std::string("conflicting definition for '" + function->name() + "'");
    }
  } else {
    _symtab.insert(function->name(), function);
    _parent->set_new_symbol(function);
  }
  
}

void og::type_checker::do_function_header_node(og::function_header_node *const node, int lvl) {
  std::string id;

  if (node->identifier() == "og")
    id = "_main";
  else if (node->identifier() == "_main")
    id = "._main";
  else
    id = node->identifier();

  std::shared_ptr<og::symbol> function = std::make_shared<og::symbol>(
    node->type(), id, 0, node->qualifier(), false, true, true);

  if (node->arguments()) {
    for (size_t i = 0; i < node->arguments()->size(); i++)
      function->add_arg_type(((og::variable_declaration_node*) node->arguments()->node(i))->type());
  }

  std::shared_ptr<og::symbol> previous = _symtab.find(function->name());
  if (previous) {
    throw std::string("conflicting declaration for '" + function->name() + "'");
  } else {
    _symtab.insert(function->name(), function);
    _parent->set_new_symbol(function);
  }
}

//---------------------------------------------------------------------------

void og::type_checker::do_nullptr_node(og::nullptr_node *const node, int lvl) {
  ASSERT_UNSPEC;
  node->type(cdk::make_primitive_type(4, cdk::TYPE_POINTER));
}

//---------------------------------------------------------------------------

void og::type_checker::do_identity_node(og::identity_node *const node, int lvl) {
  processUnaryExpression(node, lvl);
}

//---------------------------------------------------------------------------

void og::type_checker::do_tuple_node(og::tuple_node *const node, int lvl) {
  ASSERT_UNSPEC;

  node->sequence()->accept(this, lvl + 2);

  sizeof_calculator soc(_compiler, _symtab);
  node->accept(&soc, lvl);

  node->type(cdk::make_primitive_type(soc.localsize(), cdk::TYPE_STRUCT));
}
