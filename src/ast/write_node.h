#ifndef __OG_AST_WRITE_NODE_H__
#define __OG_AST_WRITE_NODE_H__

#include "targets/basic_ast_visitor.h"
#include <cdk/ast/basic_node.h>
#include <cdk/ast/sequence_node.h>

namespace og {

  /**
   * Class for describing write nodes.
   */
  class write_node: public cdk::basic_node {
    cdk::sequence_node *_expressions;
    bool _newline = false;

  public:
    inline write_node(int lineno, cdk::sequence_node *expressions, bool newline = false) :
        cdk::basic_node(lineno), _expressions(expressions), _newline(newline) {
    }

  public:
    inline cdk::sequence_node *expressions() {
      return _expressions;
    }
    inline bool newline() {
      return _newline;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_write_node(this, level);
    }

  };

} // og

#endif
