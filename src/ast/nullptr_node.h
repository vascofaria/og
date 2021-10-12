#ifndef __OG_AST_NULLPTR_NODE_H__
#define __OG_AST_NULLPTR_NODE_H__

#include "targets/basic_ast_visitor.h"
#include <cdk/ast/expression_node.h>
#include <cdk/ast/lvalue_node.h>

namespace og {

  /**
   * Class for describing nullptr nodes.
   */
  class nullptr_node: public cdk::expression_node {

  public:
    inline nullptr_node(int lineno) :
        cdk::expression_node(lineno) {
    }

  public:
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_nullptr_node(this, level);
    }

  };

} // og

#endif
