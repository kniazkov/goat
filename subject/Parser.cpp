/*

Copyright (C) 2017 Ivan Kniazkov

This file is part of interpreter of programming language
codenamed "Goat" ("Goat interpreter").

Goat interpreter is free software: you can redistribute it and/or
modify it under the terms of the GNU General Public License as published
by the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Goat interpreter is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with Goat interpreter.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "Parser.h"
#include "WideStringBuilder.h"
#include "Assert.h"
#include "Bracket.h"
#include "Brackets.h"
#include "Identifier.h"
#include "FunctionCall.h"
#include "Semicolon.h"
#include "Statement.h"
#include "StatementExpression.h"
#include "Operator.h"
#include "Binary.h"
#include "BracketExpression.h"
#include "DeclareVariable.h"
#include "Variable.h"
#include "LeftExpression.h"
#include "Assign.h"
#include "Comma.h"
#include "TokenObject.h"
#include "Colon.h"
#include "Pair.h"
#include "Dot.h"
#include "Field.h"
#include "CreateFunction.h"
#include "Return.h"
#include "If.h"
#include "Nop.h"
#include "Block.h"
#include "New.h"
#include "TokenArray.h"
#include "Index.h"
#include "While.h"
#include "Break.h"
#include "Continue.h"
#include "DoWhile.h"
#include "For.h"
#include "Boolean.h"
#include "In.h"
#include "ForIn.h"
#include "Case.h"
#include "Default.h"

namespace goat {

	Root* Parser::parse(Scanner *scan, Root *prev) {
		Parser p(scan);
		p.parse(prev);
		return p.root;
	}

	Parser::Parser(Scanner *_scan) {
		scan = _scan;
		root = nullptr;
	}

	void Parser::parse(Root *prev) {
		root = new Root(prev);
		try {
			parseBrackets(root->raw, '\0');
			parse2ndList(keyword[Keyword::IN], &Parser::parseIn, false);
			parse2ndList(identifier, &Parser::parseVariable, false);
			parse2ndList(keyword[Keyword::FUNCTION], &Parser::parseFunction, false);
			parse2ndList(keyword[Keyword::THREAD], &Parser::parseFunction, false);
			parse2ndList(parenthesis, &Parser::parseBracketExpression, false);
			parse2ndList(expression, &Parser::parseFunctionCall, false);
			parse2ndList(keyword[Keyword::NEW], &Parser::parseNew, false);
			parse2ndList(curveBracket, &Parser::parseBlock, false);
			parse2ndList(curveBracket, &Parser::parseObject, false);
			parse2ndList(squareBracket, &Parser::parseArray, false);
			parse2ndList(dot, &Parser::parseField, false);
			parse2ndList(fcall, &Parser::parseFunctionCall, false);
			parse2ndList(squareBracket, &Parser::parseIndex, false);
			parse2ndList(oper_INHERIT, &Parser::parseBinaryOperator, false);
			parse2ndList(oper_MUL_DIV_MOD, &Parser::parseBinaryOperator, false);
			parse2ndList(oper_PLUS_MINUS, &Parser::parseBinaryOperator, false);
			parse2ndList(oper_EQ_NEQ, &Parser::parseBinaryOperator, false);
			parse2ndList(oper_LESS_GREATER, &Parser::parseBinaryOperator, false);
			parse2ndList(keyword[Keyword::CASE], &Parser::parseCase, false);
			parse2ndList(keyword[Keyword::DEFAULT], &Parser::parseDefault, false);
			parse2ndList(colon, &Parser::parsePair, false);
			parse2ndList(object, &Parser::parseObjectBody, false);
			parse2ndList(array, &Parser::parseArrayBody, false);
			parse2ndList(keyword[Keyword::VAR], &Parser::parseDeclareVariable, false);
			parse2ndList(oper_ASSIGN, &Parser::parseAssign, true);
			parse2ndList(fcall, &Parser::parseFunctionCallArgs, false);
			parse2ndList(keyword[Keyword::RETURN], &Parser::parseReturn, false);
			parse2ndList(keyword[Keyword::BREAK], &Parser::parseBreak, false);
			parse2ndList(keyword[Keyword::CONTINUE], &Parser::parseContinue, false);
			parse2ndList(expression, &Parser::parseStatementExpression, false);
			parse2ndList(semicolon, &Parser::parseNop, false);
			parse2ndList(keyword[Keyword::IF], &Parser::parseIf, true);
			parse2ndList(keyword[Keyword::DO], &Parser::parseDoWhile, true);
			parse2ndList(keyword[Keyword::WHILE], &Parser::parseWhile, true);
			parse2ndList(keyword[Keyword::FOR], &Parser::parseFor, true);
			parse2ndList(index, &Parser::parseIndexBody, false);
			parse2ndList(block, &Parser::parseBlockBody, false);
			parse2ndList(function, &Parser::parseFunctionBody, false);
			parseFunctionBody(root);
			root->collectGarbage();
		}
		catch (...) {
			root->free();
			root = nullptr;
			throw;
		}
	}

	void Parser::parseBrackets(TokenList *list, char closed) {
		Token* prev;
		while (true) {
			Token* tok = scan->getToken();
			if (tok) {
				Bracket *br = tok->toBracket();
				if (br) {
					bool open = false;
					char rev = 0;
					switch (br->symbol)
					{
					case '{':
						open = true; rev = '}'; break;
					case '(':
						open = true; rev = ')'; break;
					case '[':
						open = true; rev = ']'; break;
					case '}':
						rev = '{'; break;
					case ')':
						rev = '('; break;
					case ']':
						rev = '['; break;
					default:
						break;
					}
					if (open) {
						Brackets *bs = new Brackets(br);
						list->pushBack(bs);
						if (br->symbol == '(') {
							parenthesis.pushBack(bs);
						}
						else if (br->symbol == '{') {
							curveBracket.pushBack(bs);
						}
						else if (br->symbol == '[') {
							squareBracket.pushBack(bs);
						}
						parseBrackets(bs->tokens, rev);
					}
					else {
						if (closed != br->symbol) {
							if (closed != '\0')
								throw BracketDoesNotMatch(tok, rev, br->symbol);
							else
								throw MissingOpeningBracket(tok, rev);
						}
						return;
					}
				}
				else {
					list->pushBack(tok);
					pushToAppropriate2ndList(tok);
				}
			}
			else {
				if (closed != '\0') {
					throw MissingClosingBracket(prev, closed);
				}
				return;
			}
			prev = tok;
		}
	}

	void Parser::pushToAppropriate2ndList(Token *tok) {
		if (tok->toIdentifier()) {
			identifier.pushBack(tok);
			return;
		}
		Operator *oper = tok->toOperator();
		if (oper) {
			switch (oper->type) {
			case Operator::PLUS:
			case Operator::MINUS:
				oper_PLUS_MINUS.pushBack(tok);
				break;
			case Operator::MUL:
			case Operator::DIV:
			case Operator::MOD:
				oper_MUL_DIV_MOD.pushBack(tok);
				break;
			case Operator::ASSIGN:
				oper_ASSIGN.pushBack(tok);
				break;
			case Operator::EQUAL:
			case Operator::NOT_EQUAL:
				oper_EQ_NEQ.pushBack(tok);
				break;
			case Operator::INHERIT:
				oper_INHERIT.pushBack(tok);
				break;
			case Operator::LESS:
			case Operator::GREATER:
				oper_LESS_GREATER.pushBack(tok);
				break;
			default:
				break;
			}
			return;
		}
		if (tok->toExpression()) {
			expression.pushBack(tok);
			return;
		}
		Keyword *kw = tok->toKeyword();
		if (kw) {
			keyword[kw->type].pushBack(tok);
			return;
		}
		if (tok->toColon()) {
			colon.pushBack(tok);
			return;
		}
		if (tok->toDot()) {
			dot.pushBack(tok);
			return;
		}
		if (tok->toSemicolon()) {
			semicolon.pushBack(tok);
			return;
		}
	}

	void Parser::parse2ndList(Token2ndList &list, void (Parser::*rule)(Token *), bool right2left) {
		if (!right2left) {
			Token *tok = list.first, *next;
			while (tok) {
				next = tok->next_2nd;
				(this->*rule)(tok);
				if (!next && tok->next_2nd) {
					tok = tok->next_2nd;
				}
				else {
					tok = next;
				}
			}
		}
		else {
			Token *tok = list.last, *prev;
			while (tok) {
				prev = tok->prev_2nd;
				(this->*rule)(tok);
				tok = prev;
			}
		}
	}

	/*
	  EXPRESSION @( TOKEN [@, TOKEN ...] ) => FUNCTION_CALL
	*/
	void Parser::parseFunctionCall(Token *tok) {
		Expression *expr = tok->toExpression();
		assert(expr != nullptr);

		if (!expr->next) {
			return;
		}

		Brackets *args = expr->next->toBrackets();
		if (!args || args->symbol != '(') {
			return;
		}

		FunctionCall *fcall = new FunctionCall(expr, args);
		this->fcall.pushBack(fcall);
		expr->replace(args, fcall);
		if (!expr->toFunctionCall())
			expr->remove_2nd();
		args->remove_2nd();
	}

	void Parser::parseFunctionCallArgs(Token *tok) {
		FunctionCall *fcall = tok->toFunctionCall();
		assert(fcall != nullptr);

		bool even = false;
		Token *arg = fcall->args->first;
		while (arg) {
			Token *next = arg->next;
			if (!even) {
				if (!arg->toExpression()) {
					throw ExpectedExpression(arg);
				}
			}
			else {
				if (!arg->toComma()) {
					throw ExpectedComma(arg);
				}
				arg->remove();
			}
			even = !even;
			arg = next;
		}

		expression.pushBack(fcall);
	}

	/*
	  EXPRESSION @; => STATEMENT_EXPRESSION
	*/

	void Parser::parseStatementExpression(Token *tok) {
		Expression *expr = tok->toExpression();
		assert(expr != nullptr);

		if (!expr->next) {
			return;
		}

		Semicolon *semicolon = expr->next->toSemicolon();
		if (!semicolon) {
			return;
		}

		StatementExpression *stExpr = new StatementExpression(expr);
		expr->replace(semicolon, stExpr);
		tok->remove_2nd();
		semicolon->remove_2nd();
	}

	/*
	  @function @( ... ) @{ ... } => FUNCTION
	*/
	void Parser::parseFunction(Token *tok) {
		Keyword *kw = tok->toKeyword();
		assert(kw != nullptr && (kw->type == Keyword::FUNCTION || kw->type == Keyword::THREAD));
		bool thread = kw->type == Keyword::THREAD;

		if (!kw->next) {
			throw MissedFunctionArguments(kw);
		}

		Brackets *args = kw->next->toBrackets();
		if (!args || args->symbol != '(') {
			throw MissedFunctionArguments(kw->next);
		}

		if (!args->next) {
			throw MissedFunctionBody(args);
		}

		Brackets *body = args->next->toBrackets();
		if (!body || body->symbol != '{') {
			throw MissedFunctionBody(kw->next);
		}

		Function *func = new Function(kw, args, body);
		function.pushBack(func);
		kw->remove_2nd();
		args->remove_2nd();
		body->remove_2nd();

		CreateFunction *cf = new CreateFunction(func, thread);
		kw->replace(body, cf);
	}

	void Parser::parseFunctionBody(Token *tok) {
		Function *func = tok->toFunction();
		assert(func != nullptr);

		if (func->args) {
			bool even = false;
			Token *arg = func->args->first;
			while (arg) {
				Token *next = arg->next;
				if (!even) {
					if (!arg->toIdentifier()) {
						throw ExpectedIdentifier(arg);
					}
				}
				else {
					if (!arg->toComma()) {
						throw ExpectedComma(arg);
					}
					arg->remove();
				}
				even = !even;
				arg = next;
			}
		}

		func->raw->forEach([&](Token *tok) {
			Statement *stmt = tok->toStatement();
			if (stmt) {
				func->code.pushBack(stmt);
				return;
			}
			if (!tok->next) {
				Expression *expr = tok->toExpression();
				if (expr) {
					StatementExpression *stexpr = new StatementExpression(expr);
					func->code.pushBack(stexpr);
					expr->remove();
					return;
				}
			}
			throw UnparsedSequence(tok);
		});

		assert(func->raw->count == 0);
		delete func->raw;
		func->raw = nullptr;

		tok->remove_2nd();
	}

	/*
	  EXPRESSION OPERATOR EXPRESSION => BINARY_OPERATOR
	*/

	void Parser::parseBinaryOperator(Token *tok) {
		Operator *oper = tok->toOperator();
		assert(oper != nullptr);

		if (!oper->prev) {
			return;
		}

		Expression *left = oper->prev->toExpression();
		if (!left) {
			return;
		}

		if (!oper->next) {
			// error ?
			return;
		}

		Expression *right = oper->next->toExpression();
		if (!right) {
			return;
		}

		Binary *bin = new Binary(oper, left, right);
		expression.pushBack(bin);
		left->replace(right, bin);
		oper->remove_2nd();
		if (left->list_2nd == &expression)
			left->remove_2nd();
		if (right->list_2nd == &expression)
			right->remove_2nd();
	}

	/*
	  (NULL || OPERATOR) @( TOKEN ) (NULL || OPERATOR) => BRACKET_EXPRESSION
	*/
	void Parser::parseBracketExpression(Token *tok) {
		Brackets *brackets = tok->toBrackets();
		assert(brackets != nullptr && brackets->symbol == '(');

		Token *p = tok->prev,
			*n = tok->next;
		if ((!p || p->toOperator()) && (!n || n->toOperator())) {
			// TODO: check for at least one token

			BracketExpression *brexpr = new BracketExpression(brackets);
			brackets->replace(brexpr);
			tok->remove_2nd();
		}
	}

	/*
	  @var IDENTIFIER [= EXPRESSION] [, IDENTIFIER [= EXPRESSION] ...] @; => DECLARE_VARIABLE [, DECLARE_VARIABLE]
	*/
	void Parser::parseDeclareVariable(Token *tok) {
		Keyword *kw = tok->toKeyword();
		assert(kw != nullptr && kw->type == Keyword::VAR);

		Token *separator = kw;
		bool done = false;

		while (!done) {
			Token *ref = separator->next;

			if (!ref) {
				throw ExpectedIdentifier(kw);
			}

			Identifier *name = ref->toIdentifier();
			if (!name) {
				throw ExpectedIdentifier(ref);
			}

			if (!ref->next) {
				throw ExpectedAssignmentCommaSemicolon(ref);
			}

			Operator *oper = ref->next->toOperator();
			Expression *initExpr = nullptr;
			if (oper != nullptr && oper->type == Operator::ASSIGN) {
				ref = ref->next;
				oper->remove_2nd();

				if (!ref->next) {
					throw ExpectedExpression(ref);
				}

				initExpr = ref->next->toExpression();
				if (!initExpr) {
					throw ExpectedExpression(ref->next);
				}

				ref = ref->next;
				if (!ref->next) {
					throw ExpectedCommaSemicolon(ref);
				}
				if (ref->next->toComma()) {
					// do nothing
				}
				else if (ref->next->toSemicolon()) {
					done = true;
				}
				else {
					throw ExpectedCommaSemicolon(ref->next);
				}
			}
			else if (ref->next->toComma()) {
				// do nothing
			}
			else if (ref->next->toSemicolon()) {
				done = true;
			}
			else {
				throw ExpectedAssignmentCommaSemicolon(ref);
			}

			Token *next = ref->next;
			DeclareVariable *var = new DeclareVariable(name, initExpr);
			separator->replace(ref, var);
			name->remove_2nd();

			separator = next;
			assert(separator->toComma() || separator->toSemicolon());
		}

		separator->remove();
		tok->remove_2nd();
		separator->remove_2nd();
	}

	/*
	  (@return || OPERATOR | @; | @: | @, | @. | @{} | IN | NULL) IDENTIFIER (OPERATOR | @; | , | . | [] | NULL) => VARIABLE
	*/
	void Parser::parseVariable(Token *tok) {
		Identifier *name = tok->toIdentifier();
		assert(name != nullptr);

		Token *p = tok->prev,
			*n = tok->next;

		if ((!p || p->toOperator() || p->toSemicolon() || p->toColon() || p->toDot() || p->toComma() || p->toKeyword() || p->toBrackets(true, true, false) || p->toIn()) 
			&& (!n || n->toOperator() || n->toSemicolon() || n->toColon() || n->toDot() || n->toComma() || n->toBrackets())) {
			Variable *var = new Variable(name);
			tok->replace(var);
			tok->remove_2nd();
			expression.pushBack(var);
		}
	}

	/*
	  LEFT_EXPRESSION = EXPRESSION => ASSIGN
	*/

	void Parser::parseAssign(Token *tok) {
		Operator *oper = tok->toOperator();
		assert(oper != nullptr && oper->type == Operator::ASSIGN);

		if (!oper->prev) {
			return;
		}

		LeftExpression *left = oper->prev->toLeftExpression();
		if (!left) {
			return;
		}

		if (!oper->next) {
			// error ?
			return;
		}

		Expression *right = oper->next->toExpression();
		if (!right) {
			return;
		}

		Assign *assign = new Assign(oper, left, right);
		left->replace(right, assign);
		oper->remove_2nd();
		left->remove_2nd();
		right->remove_2nd();
		expression.pushBack(assign);
	}

	/*
	  @{ TOKEN } => OBJECT
	*/
	void Parser::parseObject(Token *tok) {
		Brackets *brackets = tok->toBrackets();
		assert(brackets != nullptr && brackets->symbol == '{');

		TokenObject *tokObj = new TokenObject(brackets);
		object.pushBack(tokObj);
		brackets->replace(tokObj);
		tok->remove_2nd();
	}

	void Parser::parseObjectBody(Token *tok) {
		TokenObject *obj = tok->toTokenObject();
		assert(obj != nullptr);

		Token *item = obj->items->first;
		bool even = false;
		while (item) {
			Token *next = item->next;
			if (even) {
				if (!item->toComma()) {
					throw ExpectedComma(tok);
				}
				item->remove();
			}
			else {
				if (!item->toPair()) {
					throw ExpectedPair(tok);
				}
			}
			item = next;
			even = !even;
		}

		expression.pushBack(tok);
	}

	/*
	  IDENTIFIER @: EXPRESSION => PAIR
	*/
	void Parser::parsePair(Token *tok) {
		Colon *colon = tok->toColon();
		assert(colon != nullptr);

		if (!colon->prev) {
			return;
		}

		Expression *left = colon->prev->toExpression();
		if (!left) {
			return;
		}

		if (!colon->next) {
			return;
		}

		Expression *right = colon->next->toExpression();
		if (!right) {
			return;
		}

		Pair *pair = new Pair(left, right);
		left->replace(right, pair);
		left->remove_2nd();
		colon->remove_2nd();
	}

	/*
	  EXPRESSION @. VARIABLE => FIELD
	*/
	void Parser::parseField(Token *tok) {
		Dot *dot = tok->toDot();
		assert(dot != nullptr);

		if (!dot->prev) {
			throw LeftShouldBeExpression(dot);
		}

		Expression *left = dot->prev->toExpression();
		if (!left) {
			throw LeftShouldBeExpression(dot->prev);
		}

		if (!dot->next) {
			throw RightShouldBeIdentifierOrMethod(dot);
		}

		Variable *name = dot->next->toVariable();
		if (name) {
			Field *field = new Field(left, name);
			expression.pushBack(field);
			left->replace(name, field);

			if (left->list_2nd == &expression)
				left->remove_2nd();
			name->remove_2nd();
			dot->remove_2nd();
			return;
		}

		FunctionCall *fcall = dot->next->toFunctionCall();
		if (fcall) {
			fcall->func = new Field(left, fcall->func->toVariable());
			fcall->method = true;
			left->remove();
			if (left->list_2nd == &expression)
				left->remove_2nd();
			dot->remove();
			dot->remove_2nd();
			return;
		}

		throw RightShouldBeIdentifierOrMethod(dot->next);
	}

	/*
	   @return [ EXPRESSION ] [@;] => RETURN
	*/

	void Parser::parseReturn(Token *tok) {
		Keyword *kw = tok->toKeyword();
		assert(kw != nullptr && kw->type == Keyword::RETURN);

		if (!kw->next) {
			Return *ret = new Return(kw, nullptr);
			kw->replace(ret);
			kw->remove_2nd();
			return;
		}

		Semicolon *semicolon = kw->next->toSemicolon();
		if (semicolon) {
			Return *ret = new Return(kw, nullptr);
			kw->replace(semicolon, ret);
			kw->remove_2nd();
			semicolon->remove_2nd();
			return;
		}

		Expression *expr = kw->next->toExpression();
		if (!expr) {
			throw ExpectedExpression(kw->next);
		}

		if (!expr->next) {
			Return *ret = new Return(kw, expr);
			kw->replace(expr, ret);
			kw->remove_2nd();
			expr->remove_2nd();
			return;
		}

		semicolon = expr->next->toSemicolon();
		if (!semicolon) {
			throw ExpectedSemicolon(expr->next);
		}
		else {
			Return *ret = new Return(kw, expr);
			kw->replace(semicolon, ret);
			kw->remove_2nd();
			expr->remove_2nd();
			semicolon->remove_2nd();
		}
	}

	/*
	   @if @( EXPRESSION ) STATEMENT => IF
	*/

	void Parser::parseIf(Token *tok) {
		Keyword *kw = tok->toKeyword();
		assert(kw != nullptr && kw->type == Keyword::IF);

		if (!kw->next) {
			return;
		}

		Brackets *brackets = kw->next->toBrackets();
		assert(brackets != nullptr);
		assert(brackets->symbol == '(');
		assert(brackets->tokens->count == 1);
		if (!brackets || brackets->symbol != '(' || brackets->tokens->count != 1) {
			assert(false);
		}

		Expression *expr = brackets->tokens->first->toExpression();
		if (!expr) {
			return;
		}

		if (!brackets->next) {
			return;
		}

		Statement *stmtIf = brackets->next->toStatement();
		if (!stmtIf) {
			return;
		}

		Statement *stmtElse = nullptr;
		if (stmtIf->next) {
			Keyword *kwElse = stmtIf->next->toKeyword();
			if (kwElse && kwElse->type == Keyword::ELSE) {
				if (!kwElse->next) {
					return; //error
				}

				stmtElse = kwElse->next->toStatement();
				if (!stmtElse) {
					return; //error
				}
			}
		}

		If *i = new If(expr, stmtIf, stmtElse);
		kw->replace(stmtElse ? stmtElse : stmtIf, i);
		kw->remove_2nd();

	}

	/*
   	  @; => NOP
	*/

	void Parser::parseNop(Token *tok) {
		Semicolon *semicolon = tok->toSemicolon();
		assert(semicolon != nullptr);

		Nop *nop = new Nop(semicolon);
		semicolon->replace(nop);
		semicolon->remove_2nd();
	}

	/*
	  @{ TOKEN } => BLOCK
	*/
	void Parser::parseBlock(Token *tok) {
		Brackets *brackets = tok->toBrackets();
		assert(brackets != nullptr && brackets->symbol == '{');

		bool f = false;
		do {
			if (!tok->prev) {
				break;
			}

			Keyword *kw = tok->prev->toKeyword();
			if (kw && kw->type == Keyword::DO) {
				f = true;
				break;
			}

			Brackets *b2 = tok->prev->toBrackets();
			if (!b2 || b2->symbol != '(' || !b2->prev) {
				break;
			}

			kw = b2->prev->toKeyword();
			if (!kw) {
				break;
			}

			f = true;
		} while(0);

		if(f) {
			Block *block = new Block(brackets);
			this->block.pushBack(block);
			brackets->replace(block);
			tok->remove_2nd();
		}
	}

	void Parser::parseBlockBody(Token *tok) {
		Block *block = tok->toBlock();
		assert(block != nullptr);

		block->tokens->forEach([&](Token *tok) {
			Statement *stmt = tok->toStatement();
			if (!stmt) {
				if (!tok->next) {
					Expression *expr = tok->toExpression();
					if (expr) {
						StatementExpression *stexpr = new StatementExpression(expr);
						expr->replace(stexpr);
						return;
					}
					throw UnparsedSequence(tok);
				}
			}
		});

		block->remove_2nd();
	}

	/*
	  @new FUNCTION_CALL => NEW
	*/

	void Parser::parseNew(Token *tok) {
		Keyword *kw = tok->toKeyword();
		assert(kw != nullptr && kw->type == Keyword::NEW);

		if (!kw->next) {
			throw ExpectedNameWithArguments(kw);
		}

		FunctionCall *fcall = kw->next->toFunctionCall();
		if (!fcall) {
			throw ExpectedNameWithArguments(kw->next);
		}

		New *n = new New(kw, fcall);
		kw->replace(fcall, n);
		kw->remove_2nd();
	}

	/*
	  @[ TOKEN ] => ARRAY
	*/
	void Parser::parseArray(Token *tok) {
		Brackets *brackets = tok->toBrackets();
		assert(brackets != nullptr && brackets->symbol == '[');

		Token *p = tok->prev;

		if (!p || p->toOperator() || p->toSemicolon() || p->toComma() || p->toColon() || p->toKeyword()) {
			TokenArray *tokArr = new TokenArray(brackets);
			array.pushBack(tokArr);
			brackets->replace(tokArr);
			tok->remove_2nd();
		}
	}

	void Parser::parseArrayBody(Token *tok) {
		TokenArray *obj = tok->toTokenArray();
		assert(obj != nullptr);

		Token *item = obj->items->first;
		bool even = false;
		while (item) {
			Token *next = item->next;
			if (even) {
				if (!item->toComma()) {
					throw ExpectedComma(tok);
				}
				item->remove();
			}
			else {
				if (!item->toExpression()) {
					throw ExpectedExpression(tok);
				}
			}
			item = next;
			even = !even;
		}

		expression.pushBack(tok);
	}

	/*
	  EXPRESSION @[ TOKENS ] => INDEX
	*/
	void Parser::parseIndex(Token *tok) {
		Brackets *brackets = tok->toBrackets();
		assert(brackets != nullptr && brackets->symbol == '[');

		if (!brackets->prev) {
			throw ExpectedExpression(brackets);
		}

		Expression *expr = brackets->prev->toExpression();
		if (!expr) {
			throw ExpectedExpression(brackets->prev);
		}

		Index *index = new Index(expr, brackets);
		this->index.pushBack(index);
		expr->replace(brackets, index);
		if (expr->list_2nd == &expression)
			expr->remove_2nd();
		tok->remove_2nd();
	}

	void Parser::parseIndexBody(Token *tok) {
		Index *index = tok->toIndex();
		assert(index != nullptr);

		if (index->tokens->count != 1 || !index->tokens->first->toExpression()) {
			throw ExpectedExpression(index);
		}

		expression.pushBack(index);
	}

	/*
	  @while @( EXPRESSION ) STATEMENT => WHILE
	*/

	void Parser::parseWhile(Token *tok) {
		Keyword *kw = tok->toKeyword();
		assert(kw != nullptr && kw->type == Keyword::WHILE);

		if (!kw->next) {
			return;
		}

		Brackets *brackets = kw->next->toBrackets();
		if (!brackets || brackets->symbol != '(' || brackets->tokens->count != 1) {
			assert(false);
		}

		Expression *expr = brackets->tokens->first->toExpression();
		if (!expr) {
			return;
		}

		if (!brackets->next) {
			return;
		}

		Statement *stmt = brackets->next->toStatement();
		if (!stmt) {
			return;
		}

		While *w = new While(expr, stmt);
		kw->replace(stmt, w);
		kw->remove_2nd();
	}

	/*
	  @break [ @; ] => BREAK
	*/

	void Parser::parseBreak(Token *tok) {
		Keyword *kw = tok->toKeyword();
		assert(kw != nullptr && kw->type == Keyword::BREAK);

		Semicolon *semicolon = nullptr;

		if (kw->next) {
			semicolon = kw->next->toSemicolon();
			if (!semicolon) {
				throw ExpectedSemicolon(kw->next);
			}
		}

		Break *brk = new Break(kw);
		if (semicolon) {
			kw->replace(semicolon, brk);
			semicolon->remove_2nd();
		}
		else {
			kw->replace(brk);
		}
		kw->remove_2nd();
	}

	/*
	  @continue [ @; ] => CONTINUE
	*/

	void Parser::parseContinue(Token *tok) {
		Keyword *kw = tok->toKeyword();
		assert(kw != nullptr && kw->type == Keyword::CONTINUE);

		Semicolon *semicolon = nullptr;

		if (kw->next) {
			semicolon = kw->next->toSemicolon();
			if (!semicolon) {
				throw ExpectedSemicolon(kw->next);
			}
		}

		Continue *con = new Continue(kw);
		if (semicolon) {
			kw->replace(semicolon, con);
			semicolon->remove_2nd();
		}
		else {
			kw->replace(con);
		}
		kw->remove_2nd();
	}

	/*
	  @do STATEMENT @while @( EXPRESSION ) => DO_WHILE
	*/

	void Parser::parseDoWhile(Token *tok) {
		Keyword *kwDo = tok->toKeyword();
		assert(kwDo != nullptr && kwDo->type == Keyword::DO);

		if (!kwDo->next) {
			return;
		}

		Statement *stmt = kwDo->next->toStatement();
		if (!stmt) {
			return;
		}

		if (!stmt->next) {
			return;
		}

		Keyword *kwWhile = stmt->next->toKeyword();
		if (!kwWhile || kwWhile->type != Keyword::WHILE) {
			return;
		}

		Brackets *brackets = kwWhile->next->toBrackets();
		if (!brackets || brackets->symbol != '(' || brackets->tokens->count != 1) {
			assert(false);
		}

		Expression *expr = brackets->tokens->first->toExpression();
		if (!expr) {
			return;
		}

		DoWhile *dw = new DoWhile(expr, stmt);
		kwDo->replace(brackets, dw);
		kwDo->remove_2nd();
		kwWhile->remove_2nd();
	}

	/*
	  @for @( STATEMENT_EXPRESSION STATEMENT_EXPRESSION EXPRESSION ) => FOR
	*/

	void Parser::parseFor(Token *tok) {
		Keyword *kw = tok->toKeyword();
		assert(kw != nullptr && kw->type == Keyword::FOR);

		if (!kw->next) {
			return;
		}

 		Brackets *parameters = kw->next->toBrackets();
		if (!parameters || parameters->symbol != '(') {
			assert(false);
		}

		if (!parameters->next) {
			return;
		}

		Statement *body = parameters->next->toStatement();
		if (!body) {
			return;
		}

		if (!parameters->tokens->first) {
			return;
		}

		Statement *init = parameters->tokens->first->toStatement();
		if (!init) {
			In *varin = parameters->tokens->first->toIn();
			if (!varin) {
				return; // error?..
			}

			Expression *object = varin->next->toExpression();
			if (!object) {
				return; // error again?
			}

			ForIn *fi = new ForIn(kw, varin, object, body);
			kw->replace(body, fi);
			kw->remove_2nd();
			return;
		}

		if (!init->next) {
			return;
		}

		Expression *condition = nullptr;
		StatementExpression *stmtExprCondition = init->next->toStatementExpression();
		if (stmtExprCondition) {
			condition = stmtExprCondition->expr;
		}
		else {
			Nop *nop = init->next->toNop();
			if (nop) {
				condition = new Boolean(true);
			}
		}
		if (!condition) {
			return;
		}

		Statement *stmtIncrement;
		if (init->next->next) {
			Expression *  increment = init->next->next->toExpression();
			if (!increment) {
				return;
			}
			stmtIncrement = new StatementExpression(increment);
			}
		else {
			stmtIncrement = new Nop(nullptr);
		}

		For *f = new For(kw, init, condition, stmtIncrement, body);
		kw->replace(body, f);
		kw->remove_2nd();
	}

	/*
	  [@var] IDENTIFIER @in => IN
	*/

	void Parser::parseIn(Token *tok) {
		Keyword *kwIn = tok->toKeyword();
		assert(kwIn != nullptr && kwIn->type == Keyword::IN);

		if (!kwIn->prev) {
			return;
		}

		Identifier *name = kwIn->prev->toIdentifier();
		if (!name) {
			return;
		}

		Keyword *kwVar = nullptr;
		if (name->prev) {
			kwVar = name->prev->toKeyword();
			if (!kwVar || kwVar->type != Keyword::VAR) {
				return;
			}
		}

		In *in = new In(kwVar != nullptr, name);
		if (kwVar) {
			kwVar->replace(kwIn, in);
			kwVar->remove_2nd();
		}
		else {
			name->replace(kwIn, in);
		}
		name->remove_2nd();
		kwIn->remove_2nd();
	}

	/*
	  @case EXPRESSION @: => CASE
	*/

	void Parser::parseCase(Token *tok) {
		Keyword *kw = tok->toKeyword();
		assert(kw != nullptr && kw->type == Keyword::CASE);

		if (!kw->next) {
			throw ExpectedExpression(kw);
		}

		Expression *condition = kw->next->toExpression();
		if (!condition) {
			throw ExpectedExpression(kw->next);
		}

		if (!condition->next) {
			throw ExpectedColon(condition);
		}

		Colon *colon = condition->next->toColon();
		if (!colon) {
			throw ExpectedColon(condition->next);
		}

		Token *instr = colon->next,
			*next;
		Case *cas = new Case(kw, condition);
		kw->replace(colon, cas);
		kw->remove_2nd();
		colon->remove_2nd();
		while (instr) {
			next = instr->next;
			Keyword *kwNext = instr->toKeyword();
			if (kwNext && (kwNext->type == Keyword::CASE || kwNext->type == Keyword::DEFAULT)) {
				break;
			}
			cas->tokens->pushBack(instr);
			instr = next;
		}
	}

	/*
	  @default @: => DEFAULT
	*/

	void Parser::parseDefault(Token *tok) {
		Keyword *kw = tok->toKeyword();
		assert(kw != nullptr && kw->type == Keyword::DEFAULT);

		if (!kw->next) {
			throw ExpectedColon(kw);
		}

		Colon *colon = kw->next->toColon();
		if (!colon) {
			throw ExpectedColon(kw->next);
		}

		Token *instr = colon->next,
			*next;
		Default *def = new Default(kw);
		kw->replace(colon, def);
		kw->remove_2nd();
		colon->remove_2nd();
		while (instr) {
			next = instr->next;
			Keyword *kwNext = instr->toKeyword();
			if (kwNext) {
				if (kwNext->type == Keyword::CASE) {
					break;
				}
				if (kwNext->type == Keyword::DEFAULT) {
					throw OnlyOneDefault(kwNext);
				}
			}
			def->tokens->pushBack(instr);
			instr = next;
		}
	}

	RawString Parser::ParseError::toRawString() {
		return (WideStringBuilder () << loc << ", parse error: " << message()).toRawString();
	}

	WideString Parser::BracketDoesNotMatch::message() {
		return (WideStringBuilder() << "the closing bracket " << (wchar)closing <<  " does not match the opening bracket " << (wchar)opening ).toWideString();
	}

	WideString Parser::MissingOpeningBracket::message() {
		return (WideStringBuilder() << "missing opening bracket: " << (wchar)bracket).toWideString();
	}

	WideString Parser::MissingClosingBracket::message() {
		return (WideStringBuilder() << "missing closing bracket: " << (wchar)bracket).toWideString();
	}

	WideString Parser::UnparsedSequence::message() {
		return L"unparsed sequence of tokens";
	}

	WideString Parser::ExpectedIdentifier::message() {
		return L"here should be an identifier, isn't it?";
	}

	WideString Parser::ExpectedAssignmentCommaSemicolon::message() {
		return L"here should be an assignment, a next variable or a semicolon";
	}

	WideString Parser::ExpectedExpression::message() {
		return L"expected an expression";
	}

	WideString Parser::ExpectedCommaSemicolon::message() {
		return L"here should be a next variable or a semicolon";
	}

	WideString Parser::ExpectedPair::message() {
		return L"here should be a pair (name : expression)";
	}

	WideString Parser::ExpectedComma::message() {
		return L"here should be a separator (comma)";
	}

	WideString Parser::LeftShouldBeExpression::message() {
		return L"left token should be an expression";
	}

	WideString Parser::RightShouldBeIdentifierOrMethod::message() {
		return L"right token should be an identifier or method call";
	}

	WideString Parser::MissedFunctionArguments::message() {
		return L"where is arguments of the function?";
	}

	WideString Parser::MissedFunctionBody::message() {
		return L"where is body of the function?";
	}

	WideString Parser::ExpectedNameWithArguments::message() {
		return L"incorrect 'new' syntax, expected name of a variable with parameters";
	}

	WideString Parser::ExpectedSemicolon::message() {
		return L"here should be a separator (semicolon)";
	}

	WideString Parser::ExpectedColon::message() {
		return L"here should be a separator (colon)";
	}

	WideString Parser::OnlyOneDefault::message() {
		return L"switch statements may only contain one default";
	}
}
