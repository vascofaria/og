#include <string>
#include "targets/sizeof_calculator.h"
#include "targets/symbol.h"
#include "ast/all.h"

og::sizeof_calculator::~sizeof_calculator() {
  os().flush();
}

//---------------------------------------------------------------------------

void og::sizeof_calculator::do_nil_node(cdk::nil_node * const node, int lvl) {
  // EMPTY
}
void og::sizeof_calculator::do_data_node(cdk::data_node * const node, int lvl) {
  // EMPTY
}

//---------------------------------------------------------------------------

void og::sizeof_calculator::do_evaluation_node(og::evaluation_node * const node, int lvl) {
  // EMPTY
}

//---------------------------------------------------------------------------

void og::sizeof_calculator::do_input_node(og::input_node *const node, int lvl) {
  // EMPTY
}

//---------------------------------------------------------------------------

void og::sizeof_calculator::do_break_node(og::break_node *const node, int lvl) {
  // EMPTY
}

//---------------------------------------------------------------------------

void og::sizeof_calculator::do_return_node(og::return_node *const node, int lvl) {
  // EMPTY
}

//---------------------------------------------------------------------------

void og::sizeof_calculator::do_left_index_node(og::left_index_node *const node, int lvl) {
  // EMPTY
}

void og::sizeof_calculator::do_tuple_index_node(og::tuple_index_node *const node, int lvl) {
  // EMPTY
}

//---------------------------------------------------------------------------

void og::sizeof_calculator::do_stack_alloc_node(og::stack_alloc_node *const node, int lvl) {
  // EMPTY
}

//---------------------------------------------------------------------------

void og::sizeof_calculator::do_continue_node(og::continue_node *const node, int lvl) {
  // EMPTY
}

//---------------------------------------------------------------------------

void og::sizeof_calculator::do_write_node(og::write_node *const node, int lvl) {
  // EMPTY
}

//---------------------------------------------------------------------------

void og::sizeof_calculator::do_address_node(og::address_node *const node, int lvl) {
  // EMPTY
}

//---------------------------------------------------------------------------

void og::sizeof_calculator::do_function_call_node(og::function_call_node *const node, int lvl) {
  // EMPTY
}

void og::sizeof_calculator::do_function_header_node(og::function_header_node *const node, int lvl) {
  // EMPTY
}

void og::sizeof_calculator::do_function_definition_node(og::function_definition_node * const node, int lvl) {
  // EMPTY
}

//---------------------------------------------------------------------------

void og::sizeof_calculator::do_nullptr_node(og::nullptr_node *const node, int lvl) {
  // EMPTY
}

//---------------------------------------------------------------------------

void og::sizeof_calculator::do_block_node(og::block_node * const node, int lvl) {
  // EMPTY
}

void og::sizeof_calculator::do_for_node(og::for_node * const node, int lvl) {
  // EMPTY
}

void og::sizeof_calculator::do_if_node(og::if_node * const node, int lvl) {
  // EMPTY
}

void og::sizeof_calculator::do_if_else_node(og::if_else_node * const node, int lvl) {
  // EMPTY
}

//---------------------------------------------------------------------------

void og::sizeof_calculator::do_sizeof_node(og::sizeof_node *const node, int lvl) {
  node->expressions()->accept(this, lvl);
}

void og::sizeof_calculator::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  for (size_t i = 0; i < node->size(); i++) {
    cdk::basic_node *n = node->node(i);
    if (n == nullptr) break;
    n->accept(this, lvl + 2);
  }
}

void og::sizeof_calculator::do_integer_node(cdk::integer_node * const node, int lvl) {
  _localsize += node->type()->size();
}

void og::sizeof_calculator::do_double_node(cdk::double_node * const node, int lvl) {
  _localsize += node->type()->size();
}

void og::sizeof_calculator::do_string_node(cdk::string_node * const node, int lvl) {
  _localsize += ((std::string) node->value()).length();
}

void og::sizeof_calculator::do_tuple_node(og::tuple_node *const node, int lvl) {
  node->sequence()->accept(this, lvl + 2);
}

void og::sizeof_calculator::do_not_node(cdk::not_node * const node, int lvl) {
  // EMPTY
}

void og::sizeof_calculator::do_and_node(cdk::and_node * const node, int lvl) {
  // EMPTY
}

void og::sizeof_calculator::do_or_node(cdk::or_node * const node, int lvl) {
  // EMPTY
}

void og::sizeof_calculator::do_neg_node(cdk::neg_node * const node, int lvl) {
  // EMPTY
}

void og::sizeof_calculator::do_identity_node(og::identity_node *const node, int lvl) {
  // EMPTY
}

void og::sizeof_calculator::do_add_node(cdk::add_node * const node, int lvl) {
  _localsize += node->type()->size();
}

void og::sizeof_calculator::do_sub_node(cdk::sub_node * const node, int lvl) {
  _localsize += node->type()->size();
}

void og::sizeof_calculator::do_mul_node(cdk::mul_node * const node, int lvl) {
  _localsize += node->type()->size();
}

void og::sizeof_calculator::do_div_node(cdk::div_node * const node, int lvl) {
  _localsize += node->type()->size();
}

void og::sizeof_calculator::do_mod_node(cdk::mod_node * const node, int lvl) {
  _localsize += node->type()->size();
}

void og::sizeof_calculator::do_lt_node(cdk::lt_node * const node, int lvl) {
  _localsize += node->type()->size();
}

void og::sizeof_calculator::do_le_node(cdk::le_node * const node, int lvl) {
  _localsize += node->type()->size();
}

void og::sizeof_calculator::do_ge_node(cdk::ge_node * const node, int lvl) {
  _localsize += node->type()->size();
}

void og::sizeof_calculator::do_gt_node(cdk::gt_node * const node, int lvl) {
  _localsize += node->type()->size();
}

void og::sizeof_calculator::do_ne_node(cdk::ne_node * const node, int lvl) {
  _localsize += node->type()->size();
}

void og::sizeof_calculator::do_eq_node(cdk::eq_node * const node, int lvl) {
  _localsize += node->type()->size();
}

void og::sizeof_calculator::do_variable_node(cdk::variable_node * const node, int lvl) {
  if (node->is_typed(cdk::TYPE_INT) || node->is_typed(cdk::TYPE_POINTER)
      || node->is_typed(cdk::TYPE_DOUBLE) || node->is_typed(cdk::TYPE_STRUCT)) {
    _localsize += node->type()->size();
  } // MISSING THE OTHERS TYPES
}

void og::sizeof_calculator::do_rvalue_node(cdk::rvalue_node * const node, int lvl) {
  node->lvalue()->accept(this, lvl);
}

void og::sizeof_calculator::do_assignment_node(cdk::assignment_node * const node, int lvl) {
  // EMPTY
}

void og::sizeof_calculator::do_variable_declaration_node(og::variable_declaration_node * const node, int lvl) {
  // EMPTY
}
