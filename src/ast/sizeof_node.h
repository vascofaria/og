#ifndef __OG_AST_SIZEOF_NODE_H__
#define __OG_AST_SIZEOF_NODE_H__

#include "targets/basic_ast_visitor.h"
#include <cdk/ast/expression_node.h>
#include <cdk/ast/sequence_node.h>

namespace og {

  /**
   * Class for describing sizeof nodes.
   */
  class sizeof_node: public cdk::expression_node {
    cdk::sequence_node *_expressions;

  public:
    inline sizeof_node(int lineno, cdk::sequence_node *expressions) :
        cdk::expression_node(lineno), _expressions(expressions) {
    }

  public:
    inline cdk::sequence_node *expressions() {
      return _expressions;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_sizeof_node(this, level);
    }

  };

} // og

#endif
