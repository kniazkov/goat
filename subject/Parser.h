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

#pragma once

#include "Exception.h"
#include "Scanner.h"
#include "Token.h"
#include "Root.h"
#include "Keyword.h"
#include "WideString.h"

namespace goat {

	class Parser {
	protected:
		Root *root;
		Scanner *scan;

		Token2ndList identifier;
		Token2ndList fcall;
		Token2ndList expression;
		Token2ndList function;
		Token2ndList oper_PLUS_MINUS;
		Token2ndList oper_MUL_DIV_MOD;
		Token2ndList oper_ASSIGN;
		Token2ndList oper_EQ_NEQ;
		Token2ndList oper_INHERIT;
		Token2ndList oper_LESS_GREATER;
		Token2ndList oper_QUESTION;
		Token2ndList parenthesis;
		Token2ndList keyword[Keyword::UNKNOWN];
		Token2ndList curveBracket;
		Token2ndList colon;
		Token2ndList object;
		Token2ndList dot;
		Token2ndList semicolon;
		Token2ndList block;
		Token2ndList squareBracket;
		Token2ndList array;
		Token2ndList index;

		Parser(Scanner *_scan);
		void parse(Root *prev);
		void parseBrackets(TokenList *list, char closed);
		void pushToAppropriate2ndList(Token *tok);
		void parse2ndList(Token2ndList &list, void (Parser::*rule)(Token *), bool right2left);
		void parseFunctionCall(Token *tok);
		void parseFunctionCallArgs(Token *tok);
		void parseStatementExpression(Token *tok);
		void parseFunctionBody(Token *tok);
		void parseBinaryOperator(Token *tok);
		void parseBracketExpression(Token *tok);
		void parseDeclareVariable(Token *tok);
		void parseVariable(Token *tok);
		void parseAssign(Token *tok);
		void parseObject(Token *tok);
		void parsePair(Token *tok);
		void parseObjectBody(Token *tok);
		void parseField(Token *tok);
		void parseFunction(Token *tok);
		void parseReturn(Token *tok);
		void parseIf(Token *tok);
		void parseNop(Token *tok);
		void parseBlock(Token *tok);
		void parseBlockBody(Token *tok);
		void parseNew(Token *tok);
		void parseArray(Token *tok);
		void parseArrayBody(Token *tok);
		void parseIndex(Token *tok);
		void parseIndexBody(Token *tok);
		void parseWhile(Token *tok);
		void parseBreak(Token *tok);
		void parseContinue(Token *tok);
		void parseDoWhile(Token *tok);
		void parseFor(Token *tok);
		void parseIn(Token *tok);
		void parseCase(Token *tok);
		void parseDefault(Token *tok);
		void parseSwitch(Token *tok);
		void parseTry(Token *tok);
		void parseThrow(Token *tok);
		void parseInlineIf(Token *tok);

	public:
		static Root* parse(Scanner *scan, Root *prev);

		class ParseError : public Exception {
		public:
			String loc;

			ParseError(Token* _tok) : loc(_tok->loc->toString()) {
			}

			RawString toRawString() override;
			virtual WideString message() = 0;
		};

		class BracketDoesNotMatch : public ParseError {
		public:
			char opening,
				closing;
			BracketDoesNotMatch(Token* _tok, char _opening, char _closing) 
				: ParseError(_tok), opening(_opening), closing(_closing) {
			}

			WideString message() override;
		};

		class MissingOpeningBracket : public ParseError {
		public:
			char bracket;
			MissingOpeningBracket(Token* _tok, char _bracket) : ParseError(_tok), bracket(_bracket) {
			}

			WideString message() override;
		};

		class MissingClosingBracket : public ParseError {
		public:
			char bracket;
			MissingClosingBracket(Token* _tok, char _bracket) : ParseError(_tok), bracket(_bracket) {
			}

			WideString message() override;
		};

		class UnparsedSequence : public ParseError {
		public:
			UnparsedSequence(Token* _tok) : ParseError(_tok) {
			}

			WideString message() override;
		};

		class ExpectedIdentifier : public ParseError {
		public:
			ExpectedIdentifier(Token* _tok) : ParseError(_tok) {
			}

			WideString message() override;
		};

		class ExpectedAssignmentCommaSemicolon : public ParseError {
		public:
			ExpectedAssignmentCommaSemicolon(Token* _tok) : ParseError(_tok) {
			}

			WideString message() override;
		};

		class ExpectedExpression : public ParseError {
		public:
			ExpectedExpression(Token* _tok) : ParseError(_tok) {
			}

			WideString message() override;
		};

		class ExpectedCommaSemicolon : public ParseError {
		public:
			ExpectedCommaSemicolon(Token* _tok) : ParseError(_tok) {
			}

			WideString message() override;
		};

		class ExpectedPair : public ParseError {
		public:
			ExpectedPair(Token* _tok) : ParseError(_tok) {
			}

			WideString message() override;
		};

		class ExpectedComma : public ParseError {
		public:
			ExpectedComma(Token* _tok) : ParseError(_tok) {
			}

			WideString message() override;
		};

		class LeftShouldBeExpression : public ParseError {
		public:
			LeftShouldBeExpression(Token* _tok) : ParseError(_tok) {
			}

			WideString message() override;
		};

		class RightShouldBeIdentifierOrMethod : public ParseError {
		public:
			RightShouldBeIdentifierOrMethod(Token* _tok) : ParseError(_tok) {
			}

			WideString message() override;
		};

		class MissedFunctionArguments : public ParseError {
		public:
			MissedFunctionArguments(Token* _tok) : ParseError(_tok) {
			}

			WideString message() override;
		};

		class MissedFunctionBody : public ParseError {
		public:
			MissedFunctionBody(Token* _tok) : ParseError(_tok) {
			}

			WideString message() override;
		};

		class ExpectedNameWithArguments : public ParseError {
		public:
			ExpectedNameWithArguments(Token* _tok) : ParseError(_tok) {
			}

			WideString message() override;
		};

		class ExpectedSemicolon : public ParseError {
		public:
			ExpectedSemicolon(Token* _tok) : ParseError(_tok) {
			}

			WideString message() override;
		};

		class ExpectedColon : public ParseError {
		public:
			ExpectedColon(Token* _tok) : ParseError(_tok) {
			}

			WideString message() override;
		};

		class OnlyOneDefault : public ParseError {
		public:
			OnlyOneDefault(Token* _tok) : ParseError(_tok) {
			}

			WideString message() override;
		};

		class DefaultShouldBeLast : public ParseError {
		public:
			DefaultShouldBeLast(Token* _tok) : ParseError(_tok) {
			}

			WideString message() override;
		};

		class ExpectedStatement : public ParseError {
		public:
			ExpectedStatement(Token* _tok) : ParseError(_tok) {
			}

			WideString message() override;
		};

		class ExpectedCatchFinally : public ParseError {
		public:
			ExpectedCatchFinally(Token* _tok) : ParseError(_tok) {
			}

			WideString message() override;
		};
	};

}
