#ifndef __OG_AST_FUNCTON_HEADER_NODE_H__
#define __OG_AST_FUNCTON_HEADER_NODE_H__

#include <string>
#include "targets/basic_ast_visitor.h"
#include <cdk/ast/basic_node.h>
#include <cdk/ast/sequence_node.h>
#include <cdk/types/basic_type.h>

namespace og {

  /**
   * Class for describing function header nodes.
   */
  class function_header_node: public cdk::basic_node {
    int _qualifier;
    std::shared_ptr<cdk::basic_type> _type;
    std::string _identifier;
    cdk::sequence_node *_arguments;

  public:
    inline function_header_node(int lineno, int qualifier, const std::string &identifier, cdk::sequence_node *arguments) :
        cdk::basic_node(lineno), _qualifier(qualifier), _identifier(identifier), _arguments(arguments) {
    }
    inline function_header_node(int lineno, int qualifier, std::shared_ptr<cdk::basic_type> type, const std::string &identifier, cdk::sequence_node *arguments) :
        cdk::basic_node(lineno), _qualifier(qualifier), _type(type), _identifier(identifier), _arguments(arguments) {
    }

  public:
    inline int qualifier() {
      return _qualifier;
    }
    inline std::shared_ptr<cdk::basic_type> type() {
      return _type;
    }
    inline const std::string &identifier() {
      return _identifier;
    }
    inline cdk::sequence_node *arguments() {
      return _arguments;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_function_header_node(this, level);
    }

  };

} // og

#endif
