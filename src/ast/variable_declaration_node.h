#ifndef __OG_AST_VARIABLE_DECLARATION_NODE_H__
#define __OG_AST_VARIABLE_DECLARATION_NODE_H__

#include <cdk/ast/basic_node.h>
#include <cdk/types/basic_type.h>
#include <cdk/ast/sequence_node.h>
#include <cdk/ast/expression_node.h>
#include "targets/basic_ast_visitor.h"

namespace og {

	/**
	 * Class for describing variable declaration nodes.
	 */
	class variable_declaration_node: public cdk::basic_node {

		int _qualifier;

		std::shared_ptr<cdk::basic_type> _type;

		cdk::sequence_node *_identifiers;

		cdk::expression_node *_initializer;

		public:

			variable_declaration_node(int lineno, int qualifier, std::shared_ptr<cdk::basic_type> type, cdk::sequence_node *identifiers, cdk::expression_node *initializer) :
			cdk::basic_node(lineno), _type(type), _identifiers(identifiers), _initializer(initializer) {}

			variable_declaration_node(int lineno, int qualifier, std::shared_ptr<cdk::basic_type> type, const std::string &identifier, cdk::expression_node *initializer) :
			cdk::basic_node(lineno), _type(type), _initializer(initializer) {
				_identifiers = new cdk::sequence_node(lineno, new cdk::variable_node(lineno, identifier));
			}

		public:

			inline int qualifier() {
				return _qualifier;
			}

			inline std::shared_ptr<cdk::basic_type> type() {
				return _type;
			}

			inline void type(std::shared_ptr<cdk::basic_type> type) {
				_type = type;
			}

			inline cdk::sequence_node *identifiers() {
				return _identifiers;
			}

			inline cdk::expression_node *initializer() {
				return _initializer;
			}

		public:

			void accept(basic_ast_visitor *sp, int level) {
				sp->do_variable_declaration_node(this, level);
			}
	};

} // og

#endif