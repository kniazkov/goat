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

#include "Token.h"
#include "Brackets.h"

namespace goat {

	Token::Token() {
		loc = nullptr;
		list = nullptr;
		prev = nullptr;
		next = nullptr;
		list_2nd = nullptr;
		prev_2nd = nullptr;
		next_2nd = nullptr;
	}

	void Token::mark() {
		if (!marked) {
			marked = true;
			if (loc)
				loc->mark();
			trace();
		}
	}

	void Token::trace() {
	}

	void Token::remove() {
		if (list) {
			((TokenList*)list)->remove(this);
		}
	}

	void Token::replace(Token *repl) {
		((TokenList*)list)->replace(this, this, repl);
	}

	void Token::replace(Token *to, Token *repl) {
		((TokenList*)list)->replace(this, to, repl);
	}

	void Token::remove_2nd() {
		if (list_2nd) {
			((Token2ndList*)list_2nd)->remove(this);
		}
	}

	Executable * Token::toExecutable() {
		return nullptr;
	}

	Statement * Token::toStatement() {
		return nullptr;
	}

	Expression * Token::toExpression() {
		return nullptr;
	}

	StatementExpression * Token::toStatementExpression() {
		return nullptr;
	}

	Identifier * Token::toIdentifier() {
		return nullptr;
	}

	Bracket * Token::toBracket() {
		return nullptr;
	}

	StaticString * Token::toStaticString() {
		return nullptr;
	}

	Semicolon * Token::toSemicolon() {
		return nullptr;
	}

	Brackets * Token::toBrackets() {
		return nullptr;
	}

	Brackets * Token::toBrackets(bool round, bool curve, bool square) {
		Brackets *b = toBrackets();
		if (b && ((round && b->symbol == '(') || (curve && b->symbol == '{') || (square && b->symbol == '['))) {
			return b;
		}
		return nullptr;
	}

	Function * Token::toFunction() {
		return nullptr;
	}

	FunctionCall * Token::toFunctionCall() {
		return nullptr;
	}

	Operator * Token::toOperator() {
		return nullptr;
	}

	Binary * Token::toBinary() {
		return nullptr;
	}

	Integer * Token::toInteger() {
		return nullptr;
	}

	BracketExpression * Token::toBracketExpression() {
		return nullptr;
	}

	Keyword * Token::toKeyword() {
		return nullptr;
	}

	DeclareVariable * Token::toDeclareVariable() {
		return nullptr;
	}

	Variable * Token::toVariable() {
		return nullptr;
	}

	Assign * Token::toAssign() {
		return nullptr;
	}

	LeftExpression * Token::toLeftExpression() {
		return nullptr;
	}

	Comma * Token::toComma() {
		return nullptr;
	}

	Undefined * Token::toUndefined() {
		return nullptr;
	}

	TokenObject * Token::toTokenObject() {
		return nullptr;
	}

	Colon * Token::toColon() {
		return nullptr;
	}

	Pair * Token::toPair() {
		return nullptr;
	}

	Dot * Token::toDot() {
		return nullptr;
	}

	Field * Token::toField() {
		return nullptr;
	}

	CreateFunction * Token::toCreateFunction() {
		return nullptr;
	}

	Return * Token::toReturn() {
		return nullptr;
	}

	Boolean * Token::toBoolean() {
		return nullptr;
	}

	If * Token::toIf() {
		return nullptr;
	}

	Nop * Token::toNop() {
		return nullptr;
	}

	Block * Token::toBlock() {
		return nullptr;
	}

	This * Token::toThis() {
		return nullptr;
	}

	New * Token::toNew() {
		return nullptr;
	}

	TokenArray * Token::toTokenArray() {
		return nullptr;
	}

	Index * Token::toIndex() {
		return nullptr;
	}

	Char * Token::toChar() {
		return nullptr;
	}

	While * Token::toWhile() {
		return nullptr;
	}

	Break * Token::toBreak() {
		return nullptr;
	}

	Continue * Token::toContinue() {
		return nullptr;
	}

	DoWhile * Token::toDoWhile() {
		return nullptr;
	}

	For * Token::toFor() {
		return nullptr;
	}

	In * Token::toIn() {
		return nullptr;
	}

	ForIn * Token::toForIn() {
		return nullptr;
	}

	void TokenList::mark() {
		forEach([](Token* tok) {
			tok->mark();
		});
	}
}
