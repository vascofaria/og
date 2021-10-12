#ifndef __OG_AST_RETURN_NODE_H__
#define __OG_AST_RETURN_NODE_H__

#include <cdk/ast/basic_node.h>
#include <cdk/ast/sequence_node.h>
#include "targets/basic_ast_visitor.h"

namespace og {

	/**
	 * Class for describing break nodes.
	 */
	class return_node: public cdk::basic_node {
		
		cdk::sequence_node *_retvals;

		public:

			inline return_node(int lineno, cdk::sequence_node *retvals) : cdk::basic_node(lineno), _retvals(retvals) {}

		public:
			
			inline cdk::sequence_node *retvals() {
				return _retvals;
			}

			void accept(basic_ast_visitor *sp, int level) {
				sp->do_return_node(this, level);
			}
	};

} //og

#endif