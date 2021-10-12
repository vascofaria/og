#ifndef __OG_TARGETS_XML_WRITER_H__
#define __OG_TARGETS_XML_WRITER_H__

#include <string>
#include <sstream>
#include <stack>
#include <iostream>
#include <cdk/ast/basic_node.h>
#include <cdk/symbol_table.h>
#include "targets/basic_ast_visitor.h"
#include "targets/symbol.h"

namespace og {

  /**
   * Print nodes as XML elements to the output stream.
   */
  class xml_writer: public basic_ast_visitor {

    cdk::symbol_table<og::symbol> &_symtab;

    std::ostringstream _namestream;

    bool _errors, _inFunctionArgs, _inFunctionBody;

    bool _inForInit;

    bool _returnSeen;

    std::stack<int> _forIni, _forStep, _forEnd;

    std::stack<bool> _globals;

    std::shared_ptr<og::symbol> _function;

    int _offset;

    std::shared_ptr<cdk::basic_type> _lvalueType;

  public:
    xml_writer(std::shared_ptr<cdk::compiler> compiler, cdk::symbol_table<og::symbol> &symtab) :
      basic_ast_visitor(compiler), _symtab(symtab), _namestream(""), _errors(false), _inFunctionArgs(false), _inFunctionBody(
        false), _inForInit(false), _returnSeen(false), _function(nullptr), _offset(0), _lvalueType(nullptr) {
    }

  public:
    ~xml_writer() {
      os().flush();
    }

  private:
    void error(int lineno, std::string s) {
      std::cerr << "error: " << lineno << ": " << s << std::endl;
    }

    void openTag(const std::string &tag, int lvl) {
      os() << std::string(lvl, ' ') + "<" + tag + ">" << std::endl;
    }

    void openTag(const cdk::basic_node *node, int lvl) {
      openTag(node->label(), lvl);
    }

    void closeTag(const std::string &tag, int lvl) {
      os() << std::string(lvl, ' ') + "</" + tag + ">" << std::endl;
    }

    void closeTag(const cdk::basic_node *node, int lvl) {
      closeTag(node->label(), lvl);
    }

  protected:
    void do_binary_operation(cdk::binary_operation_node *const node, int lvl);
    
    void do_unary_operation(cdk::unary_operation_node *const node, int lvl);
    
    template<typename T>
    void process_literal(cdk::literal_node<T> *const node, int lvl) {
      os() << std::string(lvl, ' ') << "<" << node->label() << ">" << node->value() << "</" << node->label() << ">" << std::endl;
    }

  public:
    // do not edit these lines
#define __IN_VISITOR_HEADER__
#include "ast/visitor_decls.h"       // automatically generated
#undef __IN_VISITOR_HEADER__
    // do not edit these lines: end

  };

} // og

#endif
