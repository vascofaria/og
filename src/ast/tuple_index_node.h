#ifndef __OG_AST_TUPLE_INDEX_NODE_H__
#define __OG_AST_TUPLE_INDEX_NODE_H__

#include <cdk/ast/lvalue_node.h>
#include "targets/basic_ast_visitor.h"

namespace og {

	/**
	 * Class for describing tuple index nodes.
	 */
	class tuple_index_node: public cdk::lvalue_node {

		std::string _identifier;

		int _index;

		public:

			inline tuple_index_node(int lineno, const std::string &identifier, int index) :
			cdk::lvalue_node(lineno), _identifier(identifier), _index(index) {}

		public:

			inline const std::string &identifier() {
				return _identifier;
			}

			inline int index() {
				return _index;
			}

		public:

			void accept(basic_ast_visitor *sp, int level) {
				sp->do_tuple_index_node(this, level);
			}
	};

} // og

#endif
