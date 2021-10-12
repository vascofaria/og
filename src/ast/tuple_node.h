#ifndef __OG_AST_TUPLE_NODE_H__
#define __OG_AST_TUPLE_NODE_H__

#include <cdk/ast/sequence_node.h>
#include "targets/basic_ast_visitor.h"

namespace og {

	/**
	 * Class for describing identity nodes.
	 */
	class tuple_node: public cdk::expression_node {
		
		cdk::sequence_node *_sequence;

		public:

			inline tuple_node(int lineno, cdk::sequence_node *sequence) :
				cdk::expression_node(lineno), _sequence(sequence) {}

		public:

			inline cdk::sequence_node *sequence() {
      			return _sequence;
   	 		}
			
			void accept(basic_ast_visitor *sp, int level) {
				sp->do_tuple_node(this, level);
			}
	};

} //og

#endif