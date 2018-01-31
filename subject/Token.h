/*

Copyright (C) 2017, 2018 Ivan Kniazkov

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

#pragma once

#include "Node.h"
#include "Location.h"
#include "MultiList.h"

namespace goat {

	class Executable;
	class Statement;
	class Expression;
	class StatementExpression;
	class Identifier;
	class Bracket;
	class StaticString;
	class Semicolon;
	class Brackets;
	class Function;
	class FunctionCall;
	class Operator;
	class Binary;
	class Integer;
	class BracketExpression;
	class Keyword;
	class DeclareVariable;
	class Variable;
	class Assign;
	class LeftExpression;
	class Comma;
	class Undefined;
	class TokenObject;
	class Colon;
	class Pair;
	class Dot;
	class Field;
	class CreateFunction;
	class Return;
	class Boolean;
	class If;
	class Nop;
	class Block;
	class This;
	class New;
	class TokenArray;
	class Index;
	class Char;
	class While;
	class Break;
	class Continue;
	class DoWhile;
	class For;
	class In;
	class ForIn;
	class Case;
	class Default;
	class Switch;
	class Try;
	class Throw;
	class Real;
	class InlineIf;
	class Null;
	class Prefix;
	class PrefixIncrement;
	class PostfixIncrement;
	class AssignBy;
	class Debug;

	class Token : public Node {
	protected:
		virtual void trace();

	public:
		Location *loc;
		void *list,	*list_2nd;
		Token *prev, *next, *prev_2nd, *next_2nd;

		Token();
		virtual ~Token() {}
		virtual String toString() = 0;
		virtual String toFullString();
		void mark() override;
		void remove();
		void replace(Token *repl);
		void replace(Token *to, Token *repl);
		void remove_2nd();
		virtual Executable *toExecutable();
		virtual Statement *toStatement();
		virtual Expression *toExpression();
		virtual StatementExpression *toStatementExpression();
		virtual Identifier *toIdentifier();
		virtual Bracket *toBracket();
		virtual StaticString *toStaticString();
		virtual Semicolon *toSemicolon();
		virtual Brackets *toBrackets();
		Brackets *toBrackets(bool round, bool curve, bool square);
		virtual Function *toFunction();
		virtual FunctionCall *toFunctionCall();
		virtual Operator *toOperator();
		virtual Binary *toBinary();
		virtual Integer *toInteger();
		virtual BracketExpression *toBracketExpression();
		virtual Keyword *toKeyword();
		virtual DeclareVariable *toDeclareVariable();
		virtual Variable *toVariable();
		virtual Assign *toAssign();
		virtual LeftExpression *toLeftExpression();
		virtual Comma *toComma();
		virtual Undefined *toUndefined();
		virtual TokenObject *toTokenObject();
		virtual Colon *toColon();
		virtual Pair *toPair();
		virtual Dot *toDot();
		virtual Field *toField();
		virtual CreateFunction *toCreateFunction();
		virtual Return *toReturn();
		virtual Boolean *toBoolean();
		virtual If *toIf();
		virtual Nop *toNop();
		virtual Block *toBlock();
		virtual This *toThis();
		virtual New *toNew();
		virtual TokenArray *toTokenArray();
		virtual Index *toIndex();
		virtual Char *toChar();
		virtual While *toWhile();
		virtual Break *toBreak();
		virtual Continue *toContinue();
		virtual DoWhile *toDoWhile();
		virtual For *toFor();
		virtual In *toIn();
		virtual ForIn *toForIn();
		virtual Case *toCase();
		virtual Default *toDefault();
		virtual Switch *toSwitch();
		virtual Try *toTry();
		virtual Throw *toThrow();
		virtual Real *toReal();
		virtual InlineIf *toInlineIf();
		virtual Null *toNull();
		virtual Prefix *toPrefix();
		virtual PrefixIncrement *toPrefixIncrement();
		virtual PostfixIncrement *toPostfixIncrement();
		virtual AssignBy *toAssignBy();
		virtual Debug *toDebug();
	};

	class TokenList : public MultiList<Token, &Token::prev, &Token::next, &Token::list> {
	public:
		void mark();
	};

	class Token2ndList : public  MultiList<Token, &Token::prev_2nd, &Token::next_2nd, &Token::list_2nd> {
	};
}
