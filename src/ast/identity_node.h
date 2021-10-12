#ifndef __OG_AST_IDENTITY_NODE_H__
#define __OG_AST_IDENTITY_NODE_H__

#include <cdk/ast/expression_node.h>
#include <cdk/ast/unary_operation_node.h>
#include "targets/basic_ast_visitor.h"

namespace og {

	/**
	 * Class for describing identity nodes.
	 */
	class identity_node: public cdk::unary_operation_node {
		
		public:

			inline identity_node(int lineno, cdk::expression_node *expr) :
				cdk::unary_operation_node(lineno, expr) {}

		public:
			
			void accept(basic_ast_visitor *sp, int level) {
				sp->do_identity_node(this, level);
			}
	};

} //og

#endif